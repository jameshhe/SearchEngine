//Owner: James
#ifndef AVLINDEX_H
#define AVLINDEX_H
#include "indexinterface.h"
#include "AVLTree.h"

class AVLIndex : public IndexInterface
{
private:
    AVLTree<Word> words;
    int totalOpinions;
    int avgNumWords;
    int totalWords;
    string topWords[50];
public:
    AVLIndex();
    AVLIndex(const AVLIndex&);
    AVLIndex& operator=(const AVLIndex&);
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

    virtual ~AVLIndex(){
        words.makeEmpty();
    }


};

#endif // AVLINDEX_H
