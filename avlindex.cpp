#include "avlindex.h"

AVLIndex::AVLIndex()
{
    this->totalOpinions = 0;
    this->avgNumWords = 0;
    this->totalWords = 0;
}

AVLIndex::AVLIndex(const AVLIndex& avl){
    words = avl.words;
}

AVLIndex& AVLIndex::operator=(const AVLIndex &avl){
    words = avl.words;
    return *this;
}

//AVLIndex& AVLIndex::operator+=(const AVLIndex &avl){
//    this->words += avl.words;
//    return *this;
//}

void AVLIndex::insert(Word word){
    words.insert(word);
}

bool AVLIndex::contains(Word word){

    return words.contains(word);
}

void AVLIndex::addDocForWord(string word,FreqInDoc freq){
    Word wordWord(word);
    words.getNode(wordWord).addFreq(freq);
}

int AVLIndex::getTotalNodes(){
    return words.getTotalNodes();
}

int AVLIndex::getWordTotalAppearances(string word){
    Word wordWord(word);
    if(words.contains(word)){
        Word wordToReturn = words.getNode(wordWord);
        return wordToReturn.getTotalAppearance();
    }
    else return 0;
}

int AVLIndex::getWordTotalDocuments(string word){
    Word wordWord(word);
    if(words.contains(word)){
        Word wordToReturn = words.getNode(wordWord);
        return wordToReturn.getNumDocument();
    }
    else return 0;
}

Word& AVLIndex::getWord(Word wordWord){
    if(words.contains(wordWord)){
        return words.getNode(wordWord);
    }
    else{
        throw std::runtime_error("Requested word does not exist!");
    }
}

void AVLIndex::save(ofstream& output){
    words.saveToFile(output);
}

bool AVLIndex::empty(){
    return words.isEmpty();
}

void AVLIndex::clear(){
    words.makeEmpty();
}
