// mqtt-publisher.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <iomanip>
#include <map>
#include <random>
#include <boost/asio.hpp>
//#include <mqtt_client_cpp.hpp>
#include "const.h"
#include "TcpServer.h"
#include <memory>
#include "AsioIOServicePool.h"

using boost::asio::ip::tcp;

//同步服务器写法示例
void syncServer() {
    try {


        boost::asio::io_context ioc;

        //acceptor 监听连接事件
        boost::asio::ip::tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 60000));

        std::cout << "Listening on port: 60000" << std::endl;
        //不断把客户端连接交付到tcp::socket
        while (true) {
            tcp::socket sock(ioc);
            acceptor.accept(sock);

            // 3. 读取一行并回写
            std::array<char, 128> buf{};
            size_t n = sock.read_some(boost::asio::buffer(buf));
            std::cout << "server recv: " << buf.data() << '\n';
            boost::asio::write(sock, boost::asio::buffer(buf, n), IgnoreError);
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main()
{
    try {
        //初始化ioc池
        auto pool = AsioIOServicePool::GetInstance();

        //创建ioc并开启异步服务器
        boost::asio::io_context ioc;
        short port = 60000;
        auto async_server = std::make_shared<TcpServer>(ioc, port);
        std::cout << "SERVER LISTENING ON PORT 60000" << '\n';
        ioc.run();
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

