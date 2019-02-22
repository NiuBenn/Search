#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

struct Doc_Info{
    std::string _title;
    std::string _conten;
    std::string _url;
};

bool EnumFile(const std::string& file_path, std::vector<std::string>* File_List)
{
    fs::path root_path(file_path);

    if(!fs::exists(root_path))
    {
        std::cout<<"file path error"<<std::endl;
        return false;
    }

    fs::recursive_directory_iterator end_iter;
    for(fs::recursive_directory_iterator iter(root_path);iter!=end_iter;iter++)
    {
        if(fs::is_regular_file(*iter))
        {
            if((*iter).path().extension() == ".html")
            {
                File_List->push_back((*iter).path().string());
            }
        }
    }
    return true;
}

bool ParseTitle(const std::string& file_text, std::string* title)
{
    size_t begin = file_text.find("<title>");
    if(begin == std::string::npos)
        return false;
    begin += strlen("<title>");
    size_t end = file_text.find("</title>");
    if(end == std::string::npos)
        return false;
    if(end < begin)
        return false;

    *title = file_text.substr(begin, end-begin);
    return true;
}

bool ParseConten(const std::string& file_text, std::string* conten)
{
    bool Flag = true;
    for(auto &c : file_text)
    {
        if(c == '<')
        {
            Flag = false;
            continue;
        }
        else if(c == '>')
        {
            Flag = true;
            continue;
        }
        else if(Flag)
        {
            conten->push_back(c);
        }
    }
    return true;
}

bool ParseUrl(const std::string& file_path, std::string* Url)
{
    *Url = *Url + "https://www.boost.org/doc/libs/1_69_0/doc/";
    *Url = *Url + file_path.substr(strlen("../data/input/"));
    return true;
}


bool Parse(const std::string& path, Doc_Info* doc)
{
    std::ifstream file(path.c_str());
    std::string file_text;
    std::string one_line;
    if(!file.is_open())
    {
        std::cout<<"file open error"<<std::endl;
        return false;
    }
    while(std::getline(file,one_line))
    {
        file_text = file_text + one_line;
    }

    file.close();

    int ret = ParseTitle(file_text, &(doc->_title));
    if(!ret)
    {
        std::cout<<"ParseTitle error"<<std::endl;
        return false;
    }

    ret = ParseConten(file_text, &(doc->_conten));
    if(!ret)
    {
        std::cout<<"ParseConten error"<<std::endl;
        return false;
    }

    ret = ParseUrl(path, &(doc->_url));
    if(!ret)
    {
        std::cout<<"ParseURL error"<<std::endl;
        return false;
    }

    return true;
}

bool WriteFile(const Doc_Info& doc,std::string* FilePath )
{
    std::ofstream file;
    file.open(FilePath->c_str(),std::ios::app);
    if(!file)
    {
        std::cout<< "OpenFile Error"<<std::endl;
        return false;
    }
    file << doc._title.c_str();
    file << '\3';
    file << doc._conten.c_str();
    file << '\3';
    file << doc._url.c_str()<<std::endl;
    file.close();
    return true;
}


