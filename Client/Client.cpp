// Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
    try {
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc);
        
        //连接到服务器
        sock.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 60000));
        

        //发送数据
        std::string msg = "Hello motherfucker";
        msg += '\n';
        boost::asio::write(sock, boost::asio::buffer(msg));

        //等待回包
        std::array<char, 128> buf{};
        std::size_t n = sock.read_some(boost::asio::buffer(buf));
        std::cout << "server replied: " << std::string(buf.data(), n);
        system("pause");
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }
}


