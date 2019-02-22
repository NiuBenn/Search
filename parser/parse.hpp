#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/path.hpp>

//对boost::filesystem这个namespace重命名，方便后面代码编写
namespace fs = boost::filesystem;

//我们可以通过观察搜狗、谷歌等钩锁引擎的搜索结果
//基本上搜索结果里面都包含着这三部分内容：标题、正文的摘要、以及URL
//所以定义Doc_Info对象，用来存放htlm文件中的标题、正文、以及对应在php官网中的URL

struct Doc_Info{
    std::string _title;
    std::string _conten;
    std::string _url;
};

//EnumFile函数用来将输入文件目录下的所有文件的文件路径添加至FileList，便于后续对各个文件进行解析
bool EnumFile(const std::string& file_path, std::vector<std::string>* File_List)
{
    //以输入文件目录构建path对象
    fs::path root_path(file_path);

    if(!fs::exists(root_path))
    {
        std::cout<<"file path error"<<std::endl;
        return false;
    }

    //创建为初始化的迭代器end_iter俩标记结尾
    fs::recursive_directory_iterator end_iter;
    for(fs::recursive_directory_iterator iter(root_path);iter!=end_iter;iter++)
    {
        //用来判断该路径锁对应的文件是否是普通文件，因为存在目录文件等
        if(fs::is_regular_file(*iter))
        {
            //用来判断该文件是否为html
            if((*iter).path().extension() == ".html")
            {
                //将文件路径写入到File_List中
                File_List->push_back((*iter).path().string());
            }
        }
    }
    return true;
}

//解析标题
bool ParseTitle(const std::string& file_text, std::string* title)
{
    //找到<title>和</title>将两部分中间位置提取出来就是给html文件的标题
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

//用来解析正文
bool ParseConten(const std::string& file_text, std::string* conten)
{
    //因为一个html文件中除了我们需要的文本内容以外，还有很多标签、或者注释一些其他的字符
    //这些字符或者字符串会影响我们后续对正文内容进行分词统计
    //所以该步骤的目的是将html文件中的正文部分提取出来，
    //去注释以及标签的方法：我们不难发现在html中这些没有“意义的字符串”都是被'<' 和 '>' 包裹在中间
    //所以我们要去注释就会很简单，将<>之间的内容舍弃，将其他内容提取至conten中
    //这样就完成了我们对于正文的初步处理

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

//解析Url，
bool ParseUrl(const std::string& file_path, std::string* Url)
{
    //通过观察我们可以发现，php官方文档中的URL与我们input文件目录之间有一定的关系
    //通过对我们file_path的裁剪拼接我们就可以得到对应在php官网中的URL
    //
    *Url = *Url + "http://php.net/manual/zh/";
    *Url = *Url + file_path.substr(strlen("../data/input/html/"));
    (*Url).pop_back();
    size_t size = (*Url).size();
    (*Url)[size-3] = 'p';
    (*Url)[size-2] = 'h';
    (*Url)[size-1] = 'p';
    return true;
}

//解析html
bool Parse(const std::string& path, Doc_Info* doc)
{
    //这部分是对html文件内容进行一个解析
    //这一模块可以分为4大部分
    //1、通过path路径我们对对应的html文件进行读取，将htlm中的所有内容读取出来，以便后续的处理
    //2、从读取的内容中解析出对应php官网中html页面的文档标题
    //3、去掉读到的html内容中的注释、html标签、保留有用的文本信息
    //4、构建出对应php官网中对应文档的URL

    std::ifstream file(path.c_str());
    std::string file_text;
    std::string one_line;
    if(!file.is_open())
    {
        std::cout<<"file open error"<<std::endl;
        return false;
    }

    //循环按行读取html文件中的内容
    while(std::getline(file,one_line))
    {
        file_text = file_text + one_line;
    }

    //对文件操作完毕后记得关闭
    file.close();

    //解析标题
    int ret = ParseTitle(file_text, &(doc->_title));
    if(!ret)
    {
        std::cout<<"ParseTitle error"<<std::endl;
        return false;
    }

    //解析正文
    ret = ParseConten(file_text, &(doc->_conten));
    if(!ret)
    {
        std::cout<<"ParseConten error"<<std::endl;
        return false;
    }

    //构造URL
    ret = ParseUrl(path, &(doc->_url));
    if(!ret)
    {
        std::cout<<"ParseURL error"<<std::endl;
        return false;
    }

    return true;
}


//写入文件将Doc_Info对象中的内容写入到输出文件，便于后面构造正排索引、倒排索引
bool WriteFile(const Doc_Info& doc,std::string* FilePath )
{
    //在写入文件时候我们将Doc_Info中的标题、正文以及URL写入到同一行
    //为了后面解析时候能够区分出来对应的标题、正文以及URL，所以我们在他们之间用特殊标志进行间隔
    //为了不使特殊标志与html文件中的字符弄混，所以我们选取了Assic码中的不可见字符作为特殊标志字符
    //所以在这块我们在标题、正文、以及URL之间随便选择了3号字符进行一个内容分隔

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


