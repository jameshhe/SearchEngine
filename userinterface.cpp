#include "userinterface.h"

UserInterface::UserInterface(char* path)
{
    //Parser parser(path);
    //parser.save();
    totalOpinions = 0;
    totalWords = 0;
    avgNumWords = 0;
    //in order to fetch the files in the folder
    filePath = path;
    welcome();
}

void UserInterface::welcome(){
    cout << "Welcome to the search engine!" << endl;
    int choice = -1;
    while(choice < 0 || choice > 3){
        cout << "Press 1 for Interactive Mode" << endl;
        cout << "Press 2 for Maintenance Mode" << endl;
        cout << "Press 3 to display key statistics" << endl;
        cout << "Press 0 to Exit" << endl;
        cout << "----------------------------------------" << endl;
        cin >> choice;

        if(choice < 0 || choice > 3){
            cout << "Invalid choice!" << endl;
        }
        else{
            if(choice == 0){
                exit(0);
            }
            else if(choice == 1){
                choice = interactive();
            }
            else if(choice == 2){
                choice = maintenance();
            }
            else if(choice == 3){
                displayStats();
                choice = -1;
            }
        }
    }
}

int UserInterface::interactive(){
    if(totalWords != 0){
        int choice = -1;
        while(choice < 0 || choice > 2){
            cout << "Would you like to reload the index?" << endl;
            cout << "Press 1 for Yes, Press 2 for No" << endl;
            cout << "Press 0 to Return to main menu" << endl;
            cout << "----------------------------------------" << endl;
            cin >> choice;
            if(choice == 1){
                loadIndex();
                return processQuery();
            }
            else if(choice == 2){
                return processQuery();
            }
            else if(choice == 0){
                return -1;
            }
        }
    }

    loadIndex();
    return processQuery();
}

Word UserInterface::toWord(string sWord){
    stringstream ss(sWord);
    string tempWord;
    vector<string> documents;
    vector<int> frequencies;
    int count = 1;
    Word word;
    while(ss >> tempWord){
        if(count == 1){
            //the first word in this string is the string version of the word
            word.setWord(tempWord);
        }

        //every even word is a document
        else if(count % 2 == 0){
            documents.push_back(tempWord);
        }
        else{
            int frequency = stoi(tempWord);
            frequencies.push_back(frequency);
        }
        count++;
    }
    //save all the frequencies to the word
    for(int i = 0; i < documents.size(); i++){
        FreqInDoc freq(frequencies.at(i),documents.at(i));
        word.addFreq(freq);
    }
    return word;
}

void UserInterface::loadIndex(){
    int choice = -1;
    while(choice < 0 || choice > 2){
        cout << "Would you like to load the index into an AVL Tree or a Hash Table?" << endl;
        cout << "Press 1 for AVL Tree" << endl;
        cout << "Press 2 for Hash Table" << endl;
        cout << "Press 0 to Exit" << endl;
        cout << "----------------------------------------" << endl;

        cin >> choice;

        if(choice < 0 || choice > 2){
            cout << "Invalid choice, press 1 for AVL Tree and 2 for Hash Table!" << endl;
            cout << endl;
        }
        else{
        if(choice == 0){
            exit(0);
        }
        ifstream fin;
        fin.open("persistentIndex.txt");
        fin >> totalOpinions;

        if(fin.is_open()){
            if(choice == 1){
                //load into avl tree
                interface = new AVLIndex;

                string row;
                while(getline(fin,row)){
                    Word word = toWord(row);
                    interface->insert(word);
                    totalWords++;
                }
            }
            else if(choice == 2){
                interface = new HTIndex;

                string row;
                while(getline(fin,row)){
                    Word word = toWord(row);
                    interface->insert(word);
                    totalWords++;
                }
            }
            avgNumWords = totalWords / totalOpinions;
        }
        else{
            throw std::runtime_error("Persistent index file could not be opened!");
        }
        fin.close();
        }

    }
}

int UserInterface::processQuery(){
    int choice = -1;
    bool resultFound = false;
    while (choice != 0) {
        cout << "Press 1 to Search" << endl;
        cout << "Press 2 to Return to Main Menu" << endl;
        cout << "Press 0 to Exit" << endl;
        cout << "----------------------------------------" << endl;

        cin >> choice;
        cin.ignore();

        if(choice == 1){
            string buffer;
            cout << "Please enter your search: ";
            getline(cin,buffer);

            vector<Document> totalDocs;
            vector<string> tempTotalDocs;
            vector<string> words;
            stringstream ss(buffer);

            string word;
            ss >> word;

            bool notIndicator = false;


            if(word == "AND"){
                do{
                    ss >> word;


                    if(word == "NOT"){
                        //if there's a not, set the not indicator to true and continue the loop
                        notIndicator = true;
                        continue;
                    }

                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                    Porter2Stemmer::stem(word);

                    words.push_back(word);

                    Word wordWord(word);
                    if(interface->contains(wordWord)){
                        //wordInfo contains all the information of the word
                        Word wordInfo = interface->getWord(wordWord);
                        vector<string> wordDocs = wordInfo.getDocuments();
                        //if there's nothing in the vector yet, add all the elements into the vector
                        if(tempTotalDocs.size() == 0){
                            tempTotalDocs.insert(tempTotalDocs.end(),wordDocs.begin(),wordDocs.end());
                        }
                        //if there's something in the vector
                        else{
                            //if the current Word is a NOT, compare all the elements and get rid of all elements that are in the current Word
                            if(notIndicator == true){
                                for(int i = 0; i < tempTotalDocs.size(); i++){
                                    //if the current document is in the word docs
                                    if(std::count(wordDocs.begin(),wordDocs.end(),tempTotalDocs.at(i))){
                                        //remove it
                                        tempTotalDocs.erase(tempTotalDocs.begin()+i);
                                    }
                                }
                            }
                            //if the current Word is an AND, compare all the elements and get rid of all documents that are not in the current word
                            else if(notIndicator == false){
                                for(int i = 0; i < tempTotalDocs.size(); i++){
                                    //if the current document is not in the word docs
                                    if(!std::count(wordDocs.begin(),wordDocs.end(),tempTotalDocs.at(i))){
                                        //remove it
                                        tempTotalDocs.erase(tempTotalDocs.begin()+i);
                                        i--;
                                    }
                                }
                            }
                        }
                    }
                    //since this is an AND
                    else{
                        if(notIndicator == false){
                            resultFound = false;
                            break;
                        }
                    }
                }while(ss);
                //transform the string documents to document class
                for(auto tempDocs : tempTotalDocs){
                    Document tempDoc(tempDocs);
                    totalDocs.push_back(tempDoc);
                }
                //if there are documents in the totalDocs, meaning that result is found
                if(totalDocs.size() > 0){
                    resultFound = true;
                }
                //reset not indicator to false
                notIndicator = false;
            }

            else if(word == "OR"){
                do{
                    ss >> word;

                    if(word == "NOT"){
                        //if there's a not, set the not indicator to true and continue the loop
                        notIndicator = true;
                        continue;
                    }

                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                    Porter2Stemmer::stem(word);

                    words.push_back(word);

                    Word wordWord(word);
                    if(interface->contains(wordWord)){
                        //wordInfo contains all the information of the word
                        Word wordInfo = interface->getWord(wordWord);
                        vector<string> wordDocs = wordInfo.getDocuments();
                        //if the current Word is a NOT, compare all the elements and get rid of all elements that are in the current Word
                        if(notIndicator == true){
                            for(int i = 0; i < tempTotalDocs.size(); i++){
                                //if the current document is in the word docs
                                if(std::count(wordDocs.begin(),wordDocs.end(),tempTotalDocs.at(i))){
                                    //remove it
                                    tempTotalDocs.erase(tempTotalDocs.begin()+i);
                                    i--;
                                }
                            }
                        }
                        else{
                            //since this is an "or", the result should contain all the documents in every word
                            tempTotalDocs.insert(tempTotalDocs.end(),wordDocs.begin(),wordDocs.end());
                        }

                    }
                }while(ss);
                //get rid of duplicates
                tempTotalDocs.erase(std::unique(tempTotalDocs.begin(),tempTotalDocs.end()),tempTotalDocs.end());
                //transform the string documents to document class
                for(auto tempDocs : tempTotalDocs){
                    Document tempDoc(tempDocs);
                    totalDocs.push_back(tempDoc);
                }
                //if there are documents in the totalDocs, meaning that result is found
                if(totalDocs.size() > 0){
                    resultFound = true;
                }
                //reset not indicator to false
                notIndicator = false;
            }

            //if the first word is neither a AND nor a OR
            else{
                do{
                    if(word == "NOT"){
                        ss >> word;
                        continue;
                    }
                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                    Porter2Stemmer::stem(word);
                    words.push_back(word);
                    Word wordWord(word);

                    if(interface->contains(wordWord)){
                        //wordInfo contains all the information of the word
                        Word wordInfo = interface->getWord(wordWord);
                        vector<string> wordDocs = wordInfo.getDocuments();
                        //if there's a NOT

                        if(notIndicator == true){
                            for(int i = 0; i < tempTotalDocs.size(); i++){
                                if(std::count(wordDocs.begin(),wordDocs.end(),tempTotalDocs.at(i))){
                                    //remove it
                                    tempTotalDocs.erase(tempTotalDocs.begin()+i);
                                    i--;
                                }
                            }

                        }
                        //if there's not a NOT
                        else{
                            tempTotalDocs.insert(tempTotalDocs.end(),wordDocs.begin(),wordDocs.end());
                        }

                    }

                    ss >> word;
                    if(word == "NOT"){
                        notIndicator = true;
                    }
                }while(ss);
                //get rid of duplicates
                tempTotalDocs.erase(std::unique(tempTotalDocs.begin(),tempTotalDocs.end()),tempTotalDocs.end());
                //transform the string documents to document class
                for(auto tempDocs : tempTotalDocs){
                    Document tempDoc(tempDocs);
                    totalDocs.push_back(tempDoc);
                }
                //if there are documents in the totalDocs, meaning that result is found
                if(totalDocs.size() > 0){
                    resultFound = true;
                }
                //reset not indicator to false
                notIndicator = false;
            }


            if(totalDocs.size() > 0 && resultFound == true){
                for(auto word : words){
                    Word wordWord(word);
                    if(interface->contains(wordWord)){
                        rank(word,totalDocs);
                        break;
                    }
                }
                choice = displayExcerpt(totalDocs);

            }
            else{
                cout << "Result not found!" << endl;
                choice = 1;
            }

        }
        else if(choice == 2){
            return -1;
        }
        else if(choice == 0){
            exit(0);
        }
    }
}



int UserInterface::displayExcerpt(vector<Document> tempDocs){
    int choice = -1;
    while(choice < 0 || choice > 15){
        cout << "Would you like to display one of the documents?" << endl;
        cout << "Press 0 to start another search" << endl;
        cout << "Press 1-15 to display one of the documents" << endl;
        cout << "------------------------------------------------" << endl;
        cin >> choice;
        if(choice < 0 || choice > 15){
            cout << "Invalid choice!" << endl;
        }
        //if the user wants to start another search
        else if(choice == 0){
            return 1;
        }
        //if the user wants to display a document
        else{
            string excerpt = tempDocs.at(choice-1).getExcerpt();
            cout << excerpt << endl;
            cout << "------------------------------------------------" << endl;
            //loop again
            choice = -1;
        }
    }
}


int UserInterface::maintenance(){
    int choice = -1;
    while(choice < 0 || choice > 3){
        cout << "Press 1 to add opinions" << endl;
        cout << "Press 2 to clear the index" << endl;
        cout << "Press 3 to parse corpus and populate index" << endl;
        cout << "Press 0 to return to main menu" << endl;
        cout << "----------------------------------------" << endl;

        cin >> choice;
        if(choice < 0 || choice > 3){
            cout << "Invalid choice!" << endl;
        }
        else{
            if(choice == 0){
                return -1;
            }
            else if(choice == 1){
                choice = addOpinions();
            }
            else if(choice  == 2){
                clearIndex();
                choice = -1;
            }
            else if(choice == 3){
                parseCorpus();
                choice = -1;
            }
        }
    }

}

void UserInterface::parseCorpus(){
    Parser parser(filePath);
    parser.save();
}

int UserInterface::addOpinions(){
    char addPath[1000];
    cout << "Enter a file path" << endl;
    cin >> addPath;
    ifstream fin(addPath);
    if(fin.is_open()){
        Parser parse(addPath);
        parse.displayInfo();
//        *interface += *parse.getInterface();
//        cout << "before insert" << interface->getTotalNodes() << endl;
        vector<Word> tempWords = parse.getParsedWords();
        for(auto tempWord : tempWords){
            interface->insert(tempWord);
        }
//        cout << interface->getTotalNodes() << endl;
        //fix this
        totalOpinions += parse.getTotalOpinions();
        avgNumWords += parse.getAvgNumWords();
        totalWords += parse.getTotalWords();
        this->save();

        return -1;
    }
    else{
       cout << "Invalid File Path" << endl;
       return -1;
    }
}

void UserInterface::clearIndex(){
    //if interface is not empty
    //clear the index interface
    if(!interface->empty()){
        interface->clear();
        cout << "Successfully cleared the index!" << endl;
        avgNumWords = totalWords = 0;
    }
}

int UserInterface::returnTFIDF(Word word, int pos, vector<Document> wordDocs){
//    cout << "totO: " << totalOpinions << endl;
//    cout << "totD: " << word.getDocuments().size() << endl;
//    cout << "totf: " << word.getFrequencies().at(pos).getFrequency() << endl;
    int TFIDF = log10(totalOpinions/wordDocs.size()) * word.getFrequencies().at(pos).getFrequency();
    return TFIDF;
}

vector<Document> UserInterface::rank(string word, vector<Document> wordDocs){
    vector<Document> tempDocs;
    Word wordWord(word);
    Document temp;
    int one;
    int two;
    //rank
    Word wordToReturn = interface->getWord(wordWord);
    int size;
    if(wordToReturn.getFrequencies().size() >= wordDocs.size()){
        size = wordDocs.size();
    }
    else{
        size = wordToReturn.getFrequencies().size();
    }
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - i - 1; j++){
            one = returnTFIDF(wordToReturn, j, wordDocs);
            two = returnTFIDF(wordToReturn, (j+1), wordDocs);
            if(one < two){
            //if(wordDocs.at(j).getFrequency() < wordDocs.at(j + 1).getFrequency()){
                temp = wordDocs.at(j);
                wordDocs.at(j) = wordDocs.at(j + 1);
                wordDocs.at(j+1) = temp;
            }
        }
    }
    //display
    if(wordDocs.size() < 15){
        for(int y = 0; y < wordDocs.size(); y++){
            string docName = wordDocs.at(y).getDocument();
            cout << (y+1) << ". " << docName << endl;
            Document tempDoc(wordDocs.at(y).getDocument());
            cout << "Date: " << tempDoc.getDate() << "; Parties: " << tempDoc.getParties() << "; Justice: " << tempDoc.getJustice() << endl;
            cout << "----------------------------------------" << endl;
            tempDocs.push_back(tempDoc);
        }
    }
    else{
        for(int z = 0; z < 15; z++){
        string docName = wordDocs.at(z).getDocument();
        cout << (z+1) << ". " << docName << endl;
        Document tempDoc(wordDocs.at(z).getDocument());
        cout << "Date: " << tempDoc.getDate() << "; Parties: " << tempDoc.getParties() << "; Justice: " << tempDoc.getJustice() << endl;
        cout << "----------------------------------------" << endl;
        tempDocs.push_back(tempDoc);
//            cout << wordDocs.at(z).getFrequency() << endl;
        }
    }

    return tempDocs;
}

void UserInterface::displayStats(){
    cout << "Total Words: " << totalWords << endl;
    cout << "Total Number of Opinions: " << totalOpinions << endl;
    cout << "Average Words Per Opinion: " << avgNumWords << endl;
}

void UserInterface::save(){
    std::ofstream output;
    //append to file instead of overwrite
    output.open("persistentIndex.txt");
    if(output.is_open()){
        output << totalOpinions << endl;
        interface->save(output);
    }
    else{
        throw std::runtime_error("Output file could not be opened!");
    }
    output.close();
}

UserInterface::~UserInterface(){
    interface->clear();
}
