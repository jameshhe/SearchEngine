//Owner: Amory
#ifndef FREQINDOC_H
#define FREQINDOC_H
#include <iostream>
#include <string>
using namespace std;

//this class holds the frequency of a word in a certain document
class FreqInDoc
{
private:
    int frequency;
    string document;
    string excerpt;
public:
    FreqInDoc();
    FreqInDoc(int, string);
    //copy constructor
    FreqInDoc(const FreqInDoc&);
    //overloaded assignment operator
    FreqInDoc& operator=(const FreqInDoc&);
    bool operator<(const FreqInDoc&);
    ~FreqInDoc();
    //getters of frequency and document
    int getFrequency();
    string getDocument();
    string getExcerpt();
};

#endif // FREQINDOC_H
