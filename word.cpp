#include "word.h"

Word::Word()
{
}

Word::Word(string word){
    this->word = word;
}

Word::Word(string word, FreqInDoc freq){
    this->word = word;
    frequencies.push_back(freq);
    documents.push_back(freq.getDocument());
}

Word::Word(const Word &w){
    word = w.word;
    frequencies = w.frequencies;
    documents = w.documents;
}

Word& Word::operator=(const Word &w){
    this->word = w.word;
    this->frequencies = w.frequencies;
    this->documents = w.documents;
    return *this;
}

//overload three comparison operators so that they can be used in the avl tree
bool Word::operator==(const Word &w) const{
    return this->word == w.word;
}

bool Word::operator<(const Word &w) const{
    return this->word < w.word;
}

bool Word::operator>(const Word &w) const{
    return this->word > w.word;
}

Word::~Word(){
}

string Word::getWord(){
    return word;
}

void Word::setWord(string sWord){
    word = sWord;
}

void Word::addFreq(FreqInDoc freq){
    frequencies.push_back(freq);
    documents.push_back(freq.getDocument());
}

/*void Word::incrementFrequency(string document){

}*/

vector<FreqInDoc>& Word::getFrequencies(){
    return frequencies;
}

vector<string>& Word::getDocuments(){
    return documents;
}

int Word::getNumDocument(){
    return documents.size();
}

int Word::getTotalAppearance(){
    int totalAppearances = 0;
    for(int i = 0; i < frequencies.size(); i++){
        totalAppearances += frequencies.at(i).getFrequency();
    }
    return totalAppearances;
}

void Word::sortByFreq(){
    //stl function sort
    std::sort(frequencies.begin(),frequencies.end());
}

std::ostream& operator<<(std::ostream& output,const Word& word){
    output << word.word << " ";
    for(auto freq : word.frequencies){
        output << freq.getDocument() << " " << freq.getFrequency() << " ";
    }
    return output;
}
