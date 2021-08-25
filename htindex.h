//Owner: James
#ifndef HTINDEX_H
#define HTINDEX_H
#include "indexinterface.h"
#include "HashTable.h"

class HTIndex : public IndexInterface
{
private:
    //key is the string version, value is the Word version of the word
    HashTable<string,Word> words;
    int totalOpinions;
    int avgNumWords;
    int totalWords;
    string topWords[50];
public:
    HTIndex();
    HTIndex(const HTIndex&);
    HTIndex& operator=(const HTIndex&);
//    HTIndex& operator+=(const HTIndex&);
    void insert(Word);
    bool contains(Word);
    //add a document to the word's documents list
    void addDocForWord(string,FreqInDoc);
    int getTotalNodes();
    int getWordTotalAppearances(string);
    int getWordTotalDocuments(string);
    Word& getWord(Word);
    void save(ofstream&);
    bool empty();
    void clear();

    virtual ~HTIndex(){
        words.clear();
    }
};

#endif // HTINDEX_H
