#include "Crawler.hpp"
#include <iostream>

using namespace std;

int main(){

    Crawler myCrawler;

cout<<"----- Starting Corpus Collection -----"<<endl;
cout<<"----Sources: Wikipedia and Reddit-----"<<endl;

myCrawler.scrapeWiki(5);

myCrawler.scrapeReddit("cpp", 8);

myCrawler.scrapeReddit("linguistics", 8);

cout<<"Total tokens in stack rn: "<<myCrawler.getTokenCount()<<endl;

myCrawler.saveToCSV("testcorpus.csv");

cout<<"----------Workflow Complete----------"<<endl;

return 0;

}

