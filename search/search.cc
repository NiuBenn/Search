#include "search.hpp"

std::string File_Path = "../data/tmp/raw_input";

int main()
{
    searcher s;
    bool ret = s.Init(File_Path);
    if(!ret)
    {
        std::cout<<"Init Error!!"<<std::endl;
        return -1;
    }

    std::cout<<"Init Succes!!"<<std::endl;

    std::vector<Doc_Info> doc_v;
    s.Search("配置文件", &doc_v);
    for(const auto& d : doc_v)
    {
        std::cout<<"Title: "<<d._title<<std::endl;
        std::cout<<"Conten: "<<d._conten<<std::endl;
        std::cout<<"Url: "<<d._url<<std::endl<<std::endl;
    }

    return 0;
}
