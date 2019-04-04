# Search
[PHP官方网站帮助文档搜索](http://39.96.48.21:9000/index.html)

一个基于PHP官方帮助文档的小型站内搜索引擎

开发环境：Centos7、vim、g++

第三方库：boost库、cppjieba(结巴分词)、jsoncpp、cpp-httplib

data: input 用于存放 html 文件, output 用于存放解析后数据

parser: 用于对html原始文件解析

search: 核心搜索模块, 构建正排索、引倒排索引, 进行搜索

server: 服务模块,用于接收用户请求, 调用搜索模块进行处理, 将处理结果返回至用户

