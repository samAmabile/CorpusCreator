#include "Crawler.hpp"
#include "Utils.hpp"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <regex>


using json = nlohmann::json;

std::string Crawler::processText(const std::string& rawText) {
    std::string cleanText = Utils::stripHTML(rawText);
    Utils::cleanTags(cleanText);

    std::stringstream ss(cleanText);
    std::string word;
    size_t tokens = 0;
    while(ss >> word){
        tokens++;
    }
    totalTokens += tokens;

    auto compressed = Utils::compress(cleanText);
    Utils::saveBinary("corpus.zst", compressed);
    
    return cleanText;
}

void Crawler::scrapeWiki(int numPages) {
    std::cout<<"Scraping Wikipedia for "<<numPages<<" pages..."<<std::endl;
    
    cpr::Response r = cpr::Get(
        cpr::Url{"https://en.wikipedia.org/w/api.php"},
        cpr::Parameters{
            {"action", "query"},
            {"format", "json"},
            {"prop", "extracts|categories"},
            {"explaintext", "1"}, //get plain text, not html
            {"generator", "random"},
            {"grnnamespace", "0"}, //articles only
            {"grnlimit", std::to_string(numPages)}
        }
    );

    if (r.status_code != 200) {
        std::cerr<<"Network Error: "<<r.status_code<<std::endl;
        return;
    }

    json root = json::parse(r.text);
    
    auto& pages = root["query"]["pages"];

    for (auto& [id, page] : pages.items()) {
        Entry entry;
        entry.source = "Wikipedia";
        entry.text = page.value("extract", "");//default to empty str if not found
        entry.category = "General";

        if (!entry.text.empty()) {
            processText(entry.text);
            metaData.push_back(entry);
            std::cout<<"Successfully scraped: "<<page["title"]<<std::endl;
        }
        
    }

    std::cout<<"Crawl complete. New total entries: "<<metaData.size()<<'\n'<<"New total tokens: "<<totalTokens<<std::endl;
}

void Crawler::scrapeReddit(const std::string& subreddit, int limit) {
    std::cout<<"Scraping r/"<<subreddit<<" for "<<limit<<" posts..."<<std::endl;

    cpr::Response r = cpr::Get(
        cpr::Url{"https://www.reddit.com/r/"+subreddit+"/.json"},
        cpr::Parameters{{"limit", std::to_string(limit)}},
        cpr::Header{{"User-Agent", "pc:corpusbuilder:v0.1 (by /u/No_Pollution4106)"}}
    );

    if (r.status_code != 200) {
        std::cerr<<"Reddit Error: "<<r.status_code<<" (Check User-Agent or connection)"<<std::endl;
        return;
    }

    json root = json::parse(r.text);
    auto& posts = root["data"]["children"];

    for (auto& post : posts) {
        auto& postData = post["data"];

        Entry entry;
        entry.source = "Reddit: r/"+subreddit;
        entry.category = postData.value("link_flair_text", "None"); //subreddit tags

        std::string fullText = postData.value("title", "")+" "+postData.value("selfText", "");
        entry.text = fullText;

        if (!entry.text.empty()) {
            std::stringstream ss(entry.text);
            std::string word;
            size_t tokens = 0;

            while (ss >> word){
                tokens++;
            }

            totalTokens += tokens;
            metaData.push_back(entry);

            std::cout<<"Scraped post: "<<postData["title"].get<std::string>().substr(0, 30)<<"..."<<std::endl;

         }
    }
}

void Crawler::scrapeStackExchange(const std::string& tag, int pages) {
    std::cout<<"Scraping Stack Exchange for "<<pages<<" pages under tag: "<<tag<<std::endl;

    for (auto i{1}; i <= pages; ++i) {
        auto response = cpr::Get(
            cpr::Url{"https://api.stackexchange.com/2.3/questions"},
            cpr::Parameters{
                {"page", std::to_string(i)},
                {"pagesize", "50"},
                {"order", "desc"},
                {"sort", "votes"},
                {"tagged", tag},
                {"site", "english"},
                {"filter", "withbody"}
            }
            );
        
        
        if (response.status_code == 200) {
            auto json = nlohmann::json::parse(response.text);
            for (const auto& item : json["items"]) {
                
                std::string body = item["body"];
                std::string title = item["title"];
                
                std::string cleanText = processText(title+" "+body);
               
                Entry entry;
                entry.source = "StackExchange:English";
                entry.category = tag;
                entry.text = cleanText;
                metaData.push_back(entry);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }


}

void Crawler::scrapeGutenberg(int bookID) {
    std::string URL = "https://gutenberg.org/cache/epub/"+std::to_string(bookID)+"/pg"+std::to_string(bookID)+".txt";
    std::cout<<"Downloading Book ID: "<<bookID<<" from Gutenberg.org..."<<std::endl;

    auto response = cpr::Get(cpr::Url{URL});

    if (response.status_code == 200) {
        std::string rawText = response.text;
        
        size_t startPos = rawText.find("*** START OF THE PROJECT GUTENBERG EBOOK");
        size_t endPos = rawText.find("*** END OF THE PROJECT GUTENBERG EBOOK");

        if (startPos != std::string::npos && endPos != std::string::npos) {
            startPos = rawText.find("***", startPos + 40) + 3;
            std::string bookContent = rawText.substr(startPos, endPos - startPos);
            std::regex pattern(R"(\[[^\]]*\])");
            std::string preCleanText = std::regex_replace(bookContent, pattern, "");
            
            std::string cleanText = processText(preCleanText);
            
            Entry entry;
            entry.source = "Project Gutenberg";
            entry.category = "Ebook No. "+std::to_string(bookID);
            entry.text = cleanText.substr(0, 2000);
            
            metaData.push_back(entry);

            std::cout<<"Gutenberg Book ID: "<<bookID<<" succesfully added to corpus."<<std::endl;

        }else{
            std::cerr<<"Error accessing Project Gutenberg Book ID="<<bookID<<"; check book ID or try another."<<std::endl;
        }
    }
}
            
void Crawler::saveToCSV(const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr<<"Error: could not create file "<<filename<<std::endl;
	return;
    }

    file<<"Source,Category,Text\n";

    for (const auto& entry : metaData) {
        file<<entry.source<<",";
        file<<entry.category<<",";

        file<<"\"";
        for (char c : entry.text) {
            if (c == '"') file<<"\"\"";
            else if (c == '\n' || c == '\r') file<<" ";
            else file<<c;
        }
        file<<"\"\n";
    }
    
    file.close();
    std::cout<<"Data successfully written to "<<filename<<std::endl;
}

void Crawler::verifyFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string result = Utils::decompress(buffer);
    std::cout<<"Verified Data (first 100 char): "<<result.substr(0, 100)<<std::endl;
}

    

