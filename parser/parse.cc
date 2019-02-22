#include"parse.hpp"

//两个string变量来存放输入输出文件路径
std::string g_input_path = "../data/input/html";
std::string g_output_path = "../data/tmp/raw_input";

int main()
{
    bool ret = false;

    //用FileList来存放每一个需要读取文件路径
    std::vector<std::string> FileList;

    //通过EnumFile函数调用，将输入文件目录下所有的文件路径存放至FileList中
    ret = EnumFile(g_input_path, &FileList);
    if(!ret)
    {
        std::cout<< "EnumFile Error!" << std::endl;
        return 1;
    }

    //依次遍历每个文件，对单个html文件做解析
    for( const auto& path : FileList)
    {
        std::cout<<"Path:"<< path <<std::endl;

        //创建Doc_Info对象来存放单个html文件的标题、正文、以及对应在php官网中的Url
        Doc_Info doc;

        //对单个html文件进行解析，将解析的结果以输出参数的形式存放在doc对象中
        ret = Parse(path, &doc);
        if(!ret)
        {
            std::cout<< "Parse Error!" << std::endl;
            continue;
        }
        std::cout<<"Title:"<< doc._title << std::endl;
        std::cout<<"URL:"<< doc._url <<std::endl;

        //当对单个html文件解析完毕后，将解析结果写入至输出文件
        ret =  WriteFile(doc, &g_output_path);
        if(!ret)
        {
            std::cout<< " WriteFile Error"<<std::endl;
            continue;
        }
    }
    return 0;
}
