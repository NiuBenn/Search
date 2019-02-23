#pragma once
#include<iostream>
#include<unistd.h>
#include<fstream>
#include<string>
#include<vector>
#include<boost/algorithm/string.hpp>

struct Doc_Info
{
    uint32_t _id;
    std::string _title;
    std::string _conten;
    std::string _url;
};

class Index
{
public:
    void MakeDocInfo(const std::string &input, Doc_Info& doc)
    {
        std::vector<std::string> output;
        boost::split(output, input, boost::is_any_of("\3"), boost::token_compress_off);
        doc._title = output[0];
        doc._conten = output[1];
        doc._url = output[2];
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

        while(std::getline(file,one_line))
        {
            Doc_Info doc;
            doc._id = Forword_Index.size();
            MakeDocInfo(one_line, doc);
            Forword_Index.push_back(doc);
        }
        return true;
    }

private:
    std::vector<Doc_Info> Forword_Index;

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


private:
    Index* _index;

};
