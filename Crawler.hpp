#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <string>
#include <vector>

struct Entry{
    std::string source;
    std::string category;
    std::string text;
};

class Crawler{
public:
    void scrapeWiki(int pages);
    void scrapeReddit(const std::string& subreddit, int limit = 25);
    void saveToCSV(const std::string& filename);
    size_t getTokenCount() const{ return totalTokens;}
private:
    std::vector<Entry> metaData;
    size_t totalTokens = 0;
};

#endif 
