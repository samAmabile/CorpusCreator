#include "Crawler.hpp"
#include <iostream>

using namespace std;

int main(){

    Crawler myCrawler;

    cout<<"------------------ Starting Corpus Collection -------------------"<<endl;
    cout<<"----Sources: Wikipedia, StackExchange, and Project Gutenberg-----"<<endl;

    myCrawler.scrapeWiki(5);
    myCrawler.scrapeWiki(5);
    myCrawler.scrapeWiki(5);
    myCrawler.scrapeWiki(5);
    myCrawler.scrapeWiki(10);

    //myCrawler.scrapeReddit("cpp", 8);
    //myCrawler.scrapeReddit("linguistics", 8);

    myCrawler.scrapeStackExchange("etymology", 5);
    myCrawler.scrapeStackExchange("philosophy", 5);


    myCrawler.scrapeGutenberg(77776);
    myCrawler.scrapeGutenberg(77775);

    cout<<"Total tokens in stack rn: "<<myCrawler.getTokenCount()<<endl;

    myCrawler.saveToCSV("testcorpus_b.csv");

    cout<<"------------------------Workflow Complete-------------------------"<<endl;

    return 0;

}

