//Owner: Amory
#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
//document class is used to hold important information such as year, parties, and justice of the case
class Document
{
private:
    string docName;
    string date;
    string parties;
    string justice;
    string excerpt;
public:
    Document();
    Document(string);
    Document(const Document&);
    Document& operator=(const Document&);
    ~Document();
    string getDocument();
    string getDate();
    string getParties();
    string getJustice();
    string getExcerpt();
    void parse(string);
};

#endif // DOCUMENT_H
