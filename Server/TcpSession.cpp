#include "TcpSession.h"
#include <iostream>


TcpSession::TcpSession(boost::asio::io_context& ioc, TcpServer* server):
	_socket(ioc),_server(server)
{

}

TcpSession::~TcpSession()
{
	std::cout << "~TcpSession" << '\n';
}

boost::asio::ip::tcp::socket& TcpSession::Socket()
{
	return _socket;
}

void TcpSession::Close()
{
	std::cout << "socket closed" << '\n';
	_socket.close();
}

void TcpSession::Start()
{
	start_read();
}

void TcpSession::start_read()
{
	_socket.async_read_some(
		boost::asio::buffer(_buf),
		[this](boost::system::error_code ec, std::size_t bytes_transferred) {
			if (ec) {
				std::cout << "async_read_some failed , ec is " << ec.what() << '\n';
				return;
			}

			std::cout.write(_buf, bytes_transferred);
			std::cout.flush();

			//原样发回去
			boost::asio::async_write(
				_socket, boost::asio::buffer(_buf, bytes_transferred),
				[this](boost::system::error_code ec, std::size_t bytes_transferred) {
					if (!ec) start_read();
				});

		});
}
