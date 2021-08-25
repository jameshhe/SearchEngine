#include "document.h"

Document::Document(){

}

Document::Document(string document)
{
    string file = document;
    parse(file);
    this->docName = document;
}

Document::Document(const Document& d){
    this->docName = d.docName;
    this->date = d.date;
    this->parties = d.parties;
    this->justice = d.justice;
    this->excerpt = d.excerpt;
}

Document& Document::operator=(const Document& d){
    this->docName = d.docName;
    this->date = d.date;
    this->parties = d.parties;
    this->justice = d.justice;
    this->excerpt = d.excerpt;
    return *this;
}

Document::~Document(){

}

void Document::parse(string file){
    string excerpt = "";
    int wordCount = 0;
    ifstream fin(file);
    if(fin.is_open()){
        json j;
        fin >> j;
        string htmlContent = j["html"];


        if(htmlContent.find("parties") != std::string::npos){
            //get Parties
            size_t foundParties = htmlContent.find("parties");
            int partiesStart = static_cast<int>(foundParties) + 8;
            int k = partiesStart;
            while(htmlContent[k] != '/'){
                k++;
            }
            int partiesEnd = k-2;
            int partiesRange = partiesEnd - partiesStart;
            string tempParties = htmlContent.substr(partiesStart,partiesRange);

            int startIdx = 0;
            int endIdx = 0;
            bool startParsing = false;
            for(int i = 0; i < tempParties.size(); i++){
                //read everything outside of a tag
                if(tempParties[i] == '>'){
                    startIdx = i + 1;
                    startParsing = true;
                }
                else if((tempParties[i] == '<') && (startParsing == true)){
                    //set startParsing back to false
                    startParsing = false;
                    //if '><' are not adjacent brackets, meaning that there are something between the end and beginning brackets
                    //if i and endIdx are equal at this point, there's nothing between the '>' and '<' brackets
                    if(i != startIdx){
                        endIdx = i - 1;
                        int range = endIdx - startIdx + 1;
        //                cout << "in else if    " << "i position: " << i  << endl;
        //                cout << "start index: " << startIdx << "   end index: " << endIdx << "    range: " << range << endl;
                        string temp = tempParties.substr(startIdx,range);
                        this->parties += temp;
                    }
                }
            }

        }
        else this->parties = "";

        if(htmlContent.find("Justice") != std::string::npos){
            //get justice
            size_t foundJustice = htmlContent.find("Justice");
            int justiceStart = static_cast<int>(foundJustice);
            int spaceCount = 0;
            int k = justiceStart;
            while(spaceCount < 2){
                if(htmlContent[k] == ' '){
                    spaceCount++;
                }
                k++;
            }
            int justiceEnd = k-1;
            this->justice = htmlContent.substr(justiceStart,justiceEnd - justiceStart);
        }
        else this->justice = "";

        //get date
        if(htmlContent.find("date") != std::string::npos){
            //get date
            size_t foundDate = htmlContent.find("date");
            int dateStart = static_cast<int>(foundDate) + 6;
            int k = dateStart;
            while(htmlContent[k] != '<'){
                k++;
            }
            int dateEnd = k-1;
            int dateRange = dateEnd - dateStart + 1;
            this->date = htmlContent.substr(dateStart,dateRange);
        }
        else this->date = "";

        //parsing
        int endIdx = 0;
        int startIdx = 0;
        bool startParsing = false;
        for(int i = 0; i < htmlContent.size(); i++){
            //if there are more than 300 words in the excerpt already, break out of the loop
            if(wordCount > 300){
                break;
            }
            //read everything outside of a tag
            if(htmlContent[i] == '>'){
                startIdx = i + 1;
                startParsing = true;
            }
            else if((htmlContent[i] == '<') && (startParsing == true)){
                //set startParsing back to false
                startParsing = false;
                //if '><' are not adjacent brackets, meaning that there are something between the end and beginning brackets
                //if i and endIdx are equal at this point, there's nothing between the '>' and '<' brackets
                if(i != startIdx){
                    endIdx = i - 1;
                    int range = endIdx - startIdx + 1;
    //                cout << "in else if    " << "i position: " << i  << endl;
    //                cout << "start index: " << startIdx << "   end index: " << endIdx << "    range: " << range << endl;
                    string temp = htmlContent.substr(startIdx,range);
                    //use stringstream to count number of words
                    stringstream ss(temp);
                    string word;
                    while(ss>>word){
                        wordCount++;
                    }
                    excerpt += temp;
                }
            }
        }

    }
    else{
        throw std::runtime_error("File for displaying excerpt cannot be opened!");
    }
    fin.close();
    this->excerpt = excerpt;

}



string Document::getDocument(){
    return docName;
}

string Document::getDate(){
    return date;
}

string Document::getJustice(){
    return justice;
}

string Document::getExcerpt(){
    return excerpt;
}

string Document::getParties(){
    return parties;
}
