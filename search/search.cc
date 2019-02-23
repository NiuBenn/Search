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
    std::cout<<s._index->Forword_Index[0]._title<<std::endl;
    std::cout<<s._index->Forword_Index[0]._conten<<std::endl;
    std::cout<<s._index->Forword_Index[0]._url<<std::endl;
    return 0;
}
