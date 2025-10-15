#pragma once
#include <boost/asio.hpp>
#include <memory>

class TcpServer;
class TcpSession:public std::enable_shared_from_this<TcpSession>
{
public:
	TcpSession(boost::asio::io_context& ioc, TcpServer* server);
	~TcpSession();
	boost::asio::ip::tcp::socket& Socket();
	void Close();
	void Start();
private:
	void start_read();
	boost::asio::ip::tcp::socket _socket;
	TcpServer* _server;
	char _buf[1024]{};
};

