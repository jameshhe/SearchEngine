//Owner: Amory
#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H
#include "word.h"
#include "document.h"

class IndexInterface
{

public:
    IndexInterface(){}
    IndexInterface(const IndexInterface&){}
    virtual IndexInterface& operator=(const IndexInterface&){}
//    virtual IndexInterface& operator+=(const IndexInterface&){}
    virtual void addDocForWord(string,FreqInDoc) = 0;
    virtual void insert(Word) = 0;
    virtual bool contains(Word) = 0;
    virtual int getTotalNodes() = 0;
    virtual int getWordTotalAppearances(string) = 0;
    virtual int getWordTotalDocuments(string) = 0;
    virtual Word& getWord(Word) = 0;
    virtual void save(ofstream&) = 0;
    virtual bool empty() = 0;
    virtual void clear() = 0;
    virtual ~IndexInterface(){}
};

#endif // INDEXINTERFACE_H
