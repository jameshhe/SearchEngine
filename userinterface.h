#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "indexinterface.h"
#include "avlindex.h"
#include "htindex.h"
#include "Porter2Stemmer.h"
//Owner: James
#include <algorithm>
#include <sstream>
#include "parser.h"
#include "document.h"

class UserInterface
{
public:
    UserInterface(char*);
    ~UserInterface();

private:
    IndexInterface* interface;
    char* filePath;
    int totalOpinions;
    int avgNumWords;
    int totalWords;

    void welcome();
    int interactive();
    int maintenance();
    void parseCorpus();
    void loadIndex();
    Word toWord(string);
    int addOpinions();
    void clearIndex();
    int processQuery();
    int returnTFIDF(Word, int, vector<Document>);
    int displayExcerpt(vector<Document>);
    string getExcerpt(string);
    vector<Document> rank(string,vector<Document>);
    void save();
    void displayStats();
};

#endif // USERINTERFACE_H
