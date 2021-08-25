//Owner: Amory
#ifndef WORD_H
#define WORD_H
#include "freqindoc.h"
#include <vector>
#include <algorithm>
#include <iostream>

class Word
{
private:
    //private members are the string version of the word,
    //a vector of frequencies in various documents,
    //and a vector of documents in which the word appears
    string word;
    vector<FreqInDoc> frequencies;
    vector<string> documents;
public:
    Word();
    Word(string);
    //parameters are the string representation of the word and its frequency in a certain document
    Word(string,FreqInDoc);
    Word(const Word&);
    Word& operator=(const Word&);
    bool operator==(const Word&) const;
    bool operator<(const Word&) const;
    bool operator>(const Word&) const;
    friend std::ostream& operator<<(std::ostream&,const Word&);
    ~Word();

    //list of getters
    string getWord();
    vector<FreqInDoc>& getFrequencies();
    void incrementFrequency();
    void setWord(string);
    void addFreq(FreqInDoc);
    vector<string>& getDocuments();
    int getNumDocument();
    int getTotalAppearance();
    bool compareFreq(FreqInDoc,FreqInDoc);
    void sortByFreq();

};

#endif // WORD_H
