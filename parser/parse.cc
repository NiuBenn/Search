#include"parse.hpp"

std::string g_input_path = "../data/input/html";
std::string g_output_path = "../data/tmp/tmp";

int main()
{
    bool ret = false;
    std::vector<std::string> FileList;

    ret = EnumFile(g_input_path, &FileList);
    if(!ret)
    {
        std::cout<< "EnumFile Error!" << std::endl;
        return 1;
    }

    for( const auto& path : FileList)
    {
        std::cout<<"Path:"<< path <<std::endl;
        Doc_Info doc;
        ret = Parse(path, &doc);
        if(!ret)
        {
            std::cout<< "Parse Error!" << std::endl;
            continue;
        }
        std::cout<<"URL:"<< doc._url <<std::endl;
    }
    return 0;
}
