#include"../search/search.hpp"
#include"httplib.h"

int main()
{
    searcher s;
    bool ret = s.Init("../data/tmp/raw_input");
    if(!ret)
    {
        std::cout << "Searcher Init Failed" << std::endl;
        return -1;
    }

    using namespace httplib;
    Server server;

    server.Get("/search", [&s](const Request& req,
                Response& res){
                std::string query = req.get_param_value("query");
                std::string result;
                std::cout << "Get query: " << query <<std::endl;
                s.Search(query, &result);
                res.set_content(result, "application/json");
            });
    
    server.Get("/index.html", [](const Request& req,
                Response& res){
                std::ifstream file("./index.html");
                std::string one_line;
                std::string result;
                
                if(!file.is_open())
                {
                    res.set_content("file open error", "text/plain");
                }
                else
                {
                    while(std::getline(file,one_line))
                    {
                        result = result + one_line + "\n";
                    }
                    file.close();
                    res.set_content(result, "text/html");
                }
            });
    
    server.listen("0.0.0.0", 9000);
    
    return 0;
}

