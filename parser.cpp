#include "parser.h"


Parser::Parser()
{
}

Parser::Parser(char* argv){
    //    // Using time point and system_clock
    //    std::chrono::time_point<std::chrono::system_clock> start, end;
    //    start = std::chrono::system_clock::now();


    //get stop words and save them in the map
    ifstream fstop;
    fstop.open("../SearchEngine/stopWords.txt");
    if(fstop.is_open()){
        string row;
        while(getline(fstop,row)){
            stopWords.insert({row,row});
        }
    }
    else{
        throw std::runtime_error("File is not open!");
    }
    fstop.close();
    words = new AVLIndex;
    totalOpinions = 0;
    readFile(argv);

    totalWords = words->getTotalNodes();
    avgNumWords = totalWords / totalOpinions;
    //        end = std::chrono::system_clock::now();

    //        std::chrono::duration<double> elapsed_seconds = end - start;
    //        std::time_t end_time = std::chrono::system_clock::to_time_t(end);


    //        std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

Parser::~Parser(){
    delete words;
}

void Parser::readFile(const char* path){

    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(path))!=nullptr){
        while((ent = readdir(dir))!=nullptr){
            //temporary vector to hold the words
            map<string,int> tempMap;
            //go through all the files in the folder
            string filePath = path;
            string fileName = filePath + ent->d_name;
            ifstream file(fileName);
            //account for the edge cases where filename equals to . or ..
            if(fileName!= filePath + "." &&fileName!= filePath + ".."){
                //reading json files
                json j;
                file >> j;
                string htmlContent = j["html"];
                //some htmls are empty
                if(htmlContent == ""){
                    continue;
                }
                //put the string html content into a map, remove stopwords, and stem
                parse(tempMap,htmlContent,stopWords);
                //count the frequency of each words and add them to an AVL tree
                countFreq(words,tempMap,fileName);
                //increment total opinions
                totalOpinions++;
            }
            file.close();
        }
        closedir(dir);

    }
    else{
        //could not open directory
        perror("");
        throw EXIT_FAILURE;
    }
}

void Parser::parse(map<string,int> &tempMap, string origString, map<string,string> stopWords){
    int endIdx = 0;
    int startIdx = 0;
    bool startParsing = false;
    for(int i = origString.size()-1; i >= 0; i--){
        //read everything outside of a tag
        if(origString[i] == '<'){
            endIdx = i - 1;
            startParsing = true;
        }
        else if((origString[i] == '>') && (startParsing == true)){
            //set startParsing back to false
            startParsing = false;
            //if '><' are not adjacent brackets, meaning that there are something between the end and beginning brackets
            //if i and endIdx are equal at this point, there's nothing between the '>' and '<' brackets
            if(i != endIdx){
                startIdx = i + 1;
                int range = endIdx - startIdx + 1;
//                cout << "in else if    " << "i position: " << i  << endl;
//                cout << "start index: " << startIdx << "   end index: " << endIdx << "    range: " << range << endl;
                string temp = origString.substr(startIdx,range);
                istringstream ss(temp);
                //insert the words into the linked list
                do{
                    string tempWord;
                    ss >> tempWord;
                    //make the word lower case
                    transform(tempWord.begin(),tempWord.end(),tempWord.begin(),::tolower);
                    //if the word we're about to add is a stop word, ignore it by continuing the loop
                    if(stopWords.count(tempWord)){
                        continue;
                    }
                    //stem the word
                    Porter2Stemmer::stem(tempWord);
                    string result;
                    //get rid of all punctuations
                    std::remove_copy_if(tempWord.begin(),tempWord.end(),std::back_inserter(result),std::ptr_fun<int,int>(&std::ispunct));
                    if(result != ""){
                        //if a word exists in the map already
                        //increament the frequency
                        if(tempMap.count(result)){
                            tempMap[result]++;
                        }
                        else{
                            //if a word does not exist,
                            //add it to the map and set the frequency to 1
                            tempMap.insert({result,1});
                        }
                    }
                }while(ss);
            }
        }
    }
}


//count the frequencies of words in a list and return a Word object version of the list
void Parser::countFreq(IndexInterface *&resultWords, map<string,int> tempMap, string docName){
    for(auto const &m : tempMap){
        string word = m.first;
        //count the number of occurance of the word
        int frequency = m.second;
        FreqInDoc freq(frequency,docName);

        //if the tree contains the word already
        if(resultWords->contains(word)){
            //add the FreqInDoc to the current Word object
            resultWords->addDocForWord(word,freq);
        }
        //if the tree does not contain the word
        else{
            //create a word object and add it to the tree
            Word wordWord(word,freq);
            parsedWords.push_back(wordWord);
            resultWords->insert(wordWord);
        }
    }
}

vector<Word>& Parser::getParsedWords(){
    return parsedWords;
}

void Parser::displayInfo(){
    cout << "Total Words: " << totalWords << endl;
    cout << "Total Number of Opinions: " << totalOpinions << endl;
    cout << "Average Words Per Opinion: " << avgNumWords << endl;
}

int Parser::getTotalOpinions(){
    return totalOpinions;
}

int Parser::getAvgNumWords(){
    return avgNumWords;
}

int Parser::getTotalWords(){
    return totalWords;
}

IndexInterface* Parser::getInterface(){
    return words;
}

//save the indices to a persistent index text file
void Parser::save(){
    std::ofstream output;
    //append to file instead of overwrite
    output.open("persistentIndex.txt");
    if(output.is_open()){
        output << totalOpinions << endl;
        words->save(output);
    }
    else{
        throw std::runtime_error("Output file could not be opened!");
    }
    output.close();
}
