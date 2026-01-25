#include "Crawler.hpp"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using json = nlohmann::json;


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
            std::stringstream ss(entry.text);
            std::string word;
            size_t tokens = 0;

            while (ss >> word){
                tokens++;
            }

            totalTokens += tokens;

            metaData.push_back(entry);

            std::cout<<"Successfully scraped: "<<page["title"]<<" ("<<tokens<<" tokens added to total)"<<std::endl;
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
            



void Crawler::saveToCSV(const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr<<"Error: could not create file "<<filename<<std::endl;
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


    

