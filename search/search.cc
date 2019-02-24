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
    return 0;
}
