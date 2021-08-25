//Owner: Amory
#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "HashTable.h"
#include <fstream>
#include "dirent.h"
#include "json.hpp"
#include "indexinterface.h"
#include "avlindex.h"
#include "htindex.h"
#include "Porter2Stemmer.h"
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

class Parser
{
private:
    //read in all the stop words and use a HashTable to hold them
    map<string,string> stopWords;
    //used to hold all the indices
    IndexInterface *words;
    vector<Word> parsedWords;
    //make copy constructor and overloaded assignment operator private to prevent user from using
    Parser(const Parser&);
    Parser& operator=(const Parser&);

    int totalOpinions;
    int avgNumWords;
    int totalWords;
    string topWords[50];
public:
    Parser();
    Parser(char*);
    ~Parser();

    void parse(map<string,int>&,string,map<string,string>);
    void readFile(const char*);
    void countFreq(IndexInterface*&,map<string,int>,string);
    void displayInfo();
    int getTotalOpinions();
    int getAvgNumWords();
    int getTotalWords();
    IndexInterface* getInterface();
    vector<Word>& getParsedWords();
    void save();
};

#endif // PARSER_H
