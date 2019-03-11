#pragma once
#include<iostream>
#include<unistd.h>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<jsoncpp/json/json.h>
#include<boost/algorithm/string.hpp>
#include "cppjieba/Jieba.hpp"

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

struct Doc_Info
{
    uint32_t _id;
    std::string _title;
    std::string _conten;
    std::string _url;
};


struct Weight
{
    uint32_t _weight;
    uint32_t _doc_id;
    std::string _word;
};


bool cmp(std::pair<uint32_t, uint32_t> a, std::pair<uint32_t, uint32_t> b)
{
    return a.second > b.second;
}

class Index
{
public:
    void MakeDocInfo(const std::string &input, Doc_Info* doc)
    {
        std::vector<std::string> output;
        boost::split(output, input, boost::is_any_of("\3"), boost::token_compress_off);
        doc->_title = output[0];
        doc->_conten = output[1];
        doc->_url = output[2];
    }

    void CutString(const std::string& s, std::vector<std::string>* words)
    {
        jieba.CutForSearch(s, *words);
    }

    bool Build(const std::string& path)
    {
        std::ifstream file(path.c_str());
        std::string one_line;
        if(!file.is_open())
        {
            std::cout<<"[Build]:file open error"<<std::endl;
            return false;
        }

        uint32_t count = 0;
        while(std::getline(file,one_line))
        {
            Doc_Info doc;
            doc._id = Forword_Index.size();
            MakeDocInfo(one_line, &doc);
            Forword_Index.push_back(doc);
        
            Weight weight;
            std::vector<std::string> TitleWords;
            std::vector<std::string> ContenWords;

            std::map<std::string, uint32_t> tmp;

            CutString(doc._title, &TitleWords);
            CutString(doc._conten, &ContenWords);

            for(const auto& word : TitleWords)
            {
                if(word=="_"||word==" "||word==";"||word=="?"||word==":"||word=="."||word==","||word=="/"||word=="*"||word=="$"||word=="["||word=="]"||word=="("||word==")")
                    continue;
                tmp[word] += 6;
            }

            for(const auto& word : ContenWords)
            {
                if(word=="_"||word==" "||word==";"||word=="?"||word==":"||word=="."||word==","||word=="/"||word=="*"||word=="$"||word=="["||word=="]"||word=="("||word==")")
                    continue;
                tmp[word] += 1;
            }

            for(const auto& word : tmp)
            {
                weight._weight = word.second;
                weight._doc_id = doc._id;
                weight._word = word.first;

                InvertedIndex[word.first].push_back(weight);
                //std::cout<<"[weight:"<<weight._weight<<"][doc_id:"<<weight._doc_id<<"][word:"<<weight._word<<"]\n";
            }

            count++;
            if(count%500 == 0)
                std::cout << "Build："<< count <<std::endl;

        }
        std::cout << "Build Finish" << std::endl;
        return true;
    }


    void MakeRDoc(uint32_t doc_id, Doc_Info* doc)
    {
        doc->_id = doc_id;
        doc->_title = Forword_Index[doc_id]._title;
        doc->_conten = Forword_Index[doc_id]._conten.substr(0, 140) + "...";
        doc->_url = Forword_Index[doc_id]._url;
    }

    void Search(const std::string& query, std::vector<Doc_Info>* doc_v)
    {
        std::vector<std::string> words;
        CutString(query, &words);
        std::map<uint32_t , uint32_t> result;
        for(const auto& word : words)
        {
            if(word=="_"||word==" "||word==";"||word=="?"||word==":"||word=="."||word==","||word=="/"||word=="*"||word=="$"||word=="["||word=="]"||word=="("||word==")")
                continue;
            if(InvertedIndex.count(word) < 0)
                continue;
            for(const auto& wei : InvertedIndex[word])
            {
                result[wei._doc_id] += wei._weight;
            }
        }

        std::vector<std::pair<uint32_t, uint32_t>> doc;
        for(const auto& r : result)
        {
            doc.push_back(r);
        }

        sort(doc.begin(), doc.end(), cmp);
        for( const auto& d : doc)
        {
            Doc_Info DocObject;
            MakeRDoc(d.first, &DocObject);
            doc_v->push_back(DocObject);
        }
    }


    Index():jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
    {}

private:
    std::vector<Doc_Info> Forword_Index;
    std::map<std::string, std::vector<Weight>> InvertedIndex;
    cppjieba::Jieba jieba;
    

};


class searcher
{
public:
    bool Init(const std::string& File_Path)
    {
        _index = new Index;
        int ret = _index->Build(File_Path);
        if(!ret)
        {
            std::cout<<"Build Error!!"<<std::endl;
            return false;
        }
        return true;
    }

    bool Search(const std::string& query, std::string* json_result)
    {
        Json::Value results;
        std::vector<Doc_Info> doc_v;
        _index->Search(query, &doc_v);
        int count = 0;
        for(auto doc : doc_v)
        {
            Json::Value result;
            result["title"] = doc._title;
            result["desc"] = doc._conten;
            result["url"] = doc._url;
            results.append(result);
            count++;
            if(count >= 20)
                break;
        }
        Json::FastWriter writer;
        *json_result = writer.write(results);
        return true;
    }

private:
    Index* _index;
};
