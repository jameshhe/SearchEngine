#include "htindex.h"

HTIndex::HTIndex()
{
    this->totalOpinions = 0;
    this->avgNumWords = 0;
    this->totalWords = 0;
}

HTIndex::HTIndex(const HTIndex& ht){
    words = ht.words;
}

HTIndex& HTIndex::operator=(const HTIndex& ht){
    words = ht.words;
    return *this;
}


void HTIndex::insert(Word word){
    words.insert(word.getWord(),word);
}

bool HTIndex::contains(Word word){
    return words.contains(word.getWord());
}

//add a document to the word's documents list
void HTIndex::addDocForWord(string word,FreqInDoc freq){
    words.get(word).addFreq(freq);
}

int HTIndex::getTotalNodes(){
    return words.getSize();
}

int HTIndex::getWordTotalAppearances(string word){
    return words.get(word).getTotalAppearance();
}

int HTIndex::getWordTotalDocuments(string word){
    return words.get(word).getNumDocument();
}

//return the Word object that holds information
Word& HTIndex::getWord(Word wordWord){
    string word = wordWord.getWord();
    if(words.contains(word)){
        return words.get(word);
    }
    else{
        throw std::runtime_error("Requested word does not exist!");
    }
}

void HTIndex::save(ofstream& output){
    words.saveToFile(output);
}

bool HTIndex::empty(){
    return words.isEmpty();
}

void HTIndex::clear(){
    words.clear();
}
