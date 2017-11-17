//
// Created by gkubon on 31/07/17.
//

#include "DictionaryLemmatizer.h"
#include "CascadeLemmatizer.h"
#include <fstream>


using namespace std;



DictionaryLemmatizer::DictionaryLemmatizer(std::string pathname,
                                           bool typed) {
    //constructor stores dictionary in its own structure
    ifstream dictFile(pathname);
    this->typed = typed;
    string line;

    while(getline(dictFile,line)){
        UnicodeString dictLemma,dictOrth,dictCat;
        UnicodeString key;
        if(typed){

            dictCat=line.substr(0,line.find('\t')).c_str();
            dictOrth.append(line.substr(line.find('\t')+1).substr(0,line.substr(line.find('\t')+1).find_last_of('\t')).c_str());
            dictLemma=line.substr(line.rfind('\t')+1).c_str();
            key = dictCat.append("#").append(dictOrth.toLower());

        }else{

            dictOrth=line.substr(0,line.find('\t')).c_str();
            UnicodeString temp = line.substr(line.find('\t')+1).c_str();
            dictLemma.append(temp.tempSubString(0,temp.indexOf('\t')));
            key = dictOrth.toLower();
        }

        this->dictionaryItems[key] = dictLemma;

    }
    dictFile.close();

}

icu::UnicodeString DictionaryLemmatizer::lemmatize(std::vector<std::vector<icu::UnicodeString> > keyword,
                                                   std::string category) {


    //dictionary lemmatizer checks dictionary files for a lemma for given orth
    icu::UnicodeString orth ="";
    icu::UnicodeString key = "";

    for(auto& i:keyword){
        orth.append(i[0]);
        if(i[3]=="True"){
            orth.append(" ");
        }
    }
    orth.trim();

    if(category.empty()||category.find("nam_loc")!=0){
        return  "";
    }

    if (this->typed) {
        key = category.c_str();
        key.append("#");
        key.append(orth.toLower().trim());
    } else key.append(orth.toLower().trim());


    if(this->dictionaryItems.count(key)>0) {
        if (this->dictionaryItems[key].indexOf(":") != -1) {
            return this->dictionaryItems[key].tempSubStringBetween(0, this->dictionaryItems[key].indexOf(":"));
        }
        return this->dictionaryItems[key];
    }else return "";


}