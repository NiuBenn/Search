#include"parse.hpp"

struct Doc_Info{
    std::string _title;
    std::string _text;
    std::string _url;
};

std::string g_input_path = "../data/input/html";
std::string g_output_path = "../data/tmp/tmp";

int main()
{
    bool ret = false;
    std::vector<std::string> FileList;

    ret = Tools::EnumFile(g_input_path, &FileList);
    if(!ret)
    {
        std::cout<< "EnumFile Error!" << std::endl;
        return 1;
    }

    for( const auto& path : FileList)
    {
        std::cout<<path<<std::endl;
        Doc_Info doc;
        ret = Parse(path, &doc);
        if(!ret)
        {
            std::cout<< "Parse Error!" << std::endl;
            continue;
        }
    }
    return 0;
}
