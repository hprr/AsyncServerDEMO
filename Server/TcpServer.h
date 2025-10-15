#pragma once
#include <boost/asio.hpp>
#include <memory>
class TcpServer:public std::enable_shared_from_this<TcpServer>
{
public:
	explicit TcpServer(boost::asio::io_context& ioc, short& port);
	//TcpServer(const TcpServer&) = delete;
	//const TcpServer operator=(const TcpServer&) = delete;
	~TcpServer();

private:
	boost::asio::io_context& _ioc;
	boost::asio::ip::tcp::acceptor _acceptor;
	short _port;

	void start_accept();
};

