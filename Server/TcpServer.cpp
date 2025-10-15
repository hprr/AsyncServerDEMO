#include "TcpServer.h"
#include "TcpSession.h"
#include "AsioIOServicePool.h"

TcpServer::~TcpServer()
{
}

void TcpServer::start_accept()
{
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	auto new_session = std::make_shared<TcpSession>(io_context, this);
	_acceptor.async_accept(new_session->Socket(), [new_session,this](const boost::system::error_code& ec) {
		if (!ec) {
			new_session->Start();
			std::cout << "a new_session connected and started to recv data" << '\n';
		}
		else {
			new_session->Close();
			std::cout << "TcpSession accept failed, error is " << ec.what() << '/n';
		}
		//¼ÌÐø¼àÌýÁ¬½Ó
		start_accept();
		});

}



TcpServer::TcpServer(boost::asio::io_context& ioc, short& port):
_ioc(ioc),_acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))
{
	start_accept();
}
