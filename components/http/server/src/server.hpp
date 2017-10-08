
#pragma once

#include "io_service_pool.hpp"
#include "connection.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace timax
{

	class server : private boost::noncopyable
	{
	public:
		explicit server(std::size_t io_service_pool_size);


		enum ssl_method_t
		{
			sslv2 = boost::asio::ssl::context::sslv2_server,
			sslv3 = boost::asio::ssl::context::sslv3_server,
			tlsv1 = boost::asio::ssl::context::tlsv1_server,
			tlsv11 = boost::asio::ssl::context::tlsv11_server,
			tlsv12 = boost::asio::ssl::context::tlsv12_server,
			sslv23 = boost::asio::ssl::context::sslv23_server
		};

		server& listen(const std::string& address, const std::string& port);
		server& listen(const std::string& address, const std::string& port, ssl_method_t ssl_method,
			const std::string& private_key, const std::string& certificate_chain, bool is_file = true);

		void run();

		void request_handler(request_handler_t handler)
		{
			request_handler_ = std::move(handler);
		}

		void stop();

	private:
		void start_accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor);
		void start_accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor,
			boost::shared_ptr<boost::asio::ssl::context> const& ssl_ctx);

		void do_listen(boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor,
			const std::string& address, const std::string& port);

		io_service_pool io_service_pool_;
		request_handler_t request_handler_;
	};

}