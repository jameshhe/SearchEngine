#include "freqindoc.h"

FreqInDoc::FreqInDoc()
{
    frequency = 0;
    document = "";
    excerpt = "";
}

FreqInDoc::FreqInDoc(int frequency, string documentTitle){
    this->frequency = frequency;
    this->document = documentTitle;
}

FreqInDoc::FreqInDoc(const FreqInDoc& f){
    frequency = f.frequency;
    document = f.document;
}

FreqInDoc& FreqInDoc::operator=(const FreqInDoc& f){
    this->frequency = f.frequency;
    this->document = f.document;
    return *this;
}

bool FreqInDoc::operator<(const FreqInDoc &f){
    return frequency < f.frequency;
}

FreqInDoc::~FreqInDoc(){

}

int FreqInDoc::getFrequency(){
    return frequency;
}

string FreqInDoc::getDocument(){
    return document;
}


string FreqInDoc::getExcerpt(){
    return excerpt;
}
