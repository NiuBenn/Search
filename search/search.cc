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
    std::cout<<"请输入搜索内容：";
    std::string str;
    std::string json_result;
    std::cin>>str;

    s.Search("配置文件", &json_result);

    std::cout<<json_result<<std::endl;

    return 0;
}
