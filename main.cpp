#include "Crawler.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <random>
#include <thread>
#include <chrono>

using namespace std;

namespace fs = std::filesystem;

/*string makeDate(){
    auto now = std::time(nullptr);
    auto time = *std::localtime(&now); 

    ostringstream oss; 
    oss << std::put_time(&time, "%Y-%m-%d");

    return oss.str();
}*/


int main(int argc, char* argv[]){

    Crawler c; 

    int stackex, wiki, books;
    string date = c.makeDate();
    string ofilename = "corpus_"+date+".csv";
    if (argc == 5){
        ofilename = argv[4];
        fs::path p(ofilename); 
        if (p.extension() != ".csv"){
            p.replace_extension(".csv");
            ofilename = p.string();
        }
    }
    if (argc >= 4){
        stackex = stoi(argv[3]);
    }
    if (argc >= 3){
        wiki = stoi(argv[2]);
    }
    if (argc >= 2){
        books = stoi(argv[1]);
    }
    
    //add path to filename to save in data/text/ dir:
    fs::path datapth = fs::path("..") / "data" / "text";
    fs::path saveas = datapth / ofilename;


    cout<<"------------------ Starting Corpus Collection -------------------"<<endl;
    cout<<"----Sources: Wikipedia, StackExchange, and Project Gutenberg-----"<<endl;
    cout<<"------------------------Date:"<<date<<"-------------------------"<<endl;

    for (int i=0; i<wiki; i++){
        c.scrapeWiki(10); 
    }
    //string categories[10] = {"etymology", "philosophy", "literature", "programming", "finance", "film", "pop culture", "television", "news"};
    string sites[11] = {
        "english",
        "etymology",
        "philosophy",
        "literature",
        "codereview",
        "money",
        "movies",
        "history",
        "politics",
        "scifi",
        "worldbuilding"
    };
    int j = 0;
    for (int i=0; i<stackex; i++){
        if (j <= 11){
            c.scrapeStackExchange(sites[j], 10);
            j++;
        }else{
            j = 0; 
            c.scrapeStackExchange(sites[j], 10);
        }
    }
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(70000, 77773);
    uniform_int_distribution<> pause(5, 8);
    int bookID = dist(gen); 
    for (int i=0; i<books; i++){
        c.scrapeGutenberg(bookID);
        bookID = dist(gen); 
        int pause_time = pause(gen);
        this_thread::sleep_for(chrono::seconds(pause_time));
    }

    cout<<"Total tokens in stack rn: "<<c.getTokenCount()<<endl;
    

    c.saveToCSV(saveas.string());

    cout<<"------------------------Workflow Complete-------------------------"<<endl;

    return 0;
}


