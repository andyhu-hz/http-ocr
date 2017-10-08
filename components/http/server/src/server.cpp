
#include "server.hpp"

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>

namespace timax
{
	server::server(std::size_t io_service_pool_size)
		: io_service_pool_(io_service_pool_size)
	{
	}

	timax::server& server::listen(const std::string& address, const std::string& port)
	{
		auto acceprot = boost::make_shared<boost::asio::ip::tcp::acceptor>(io_service_pool_.get_io_service());
		do_listen(acceprot, address, port);
		start_accept(acceprot);
		return *this;
	}

	timax::server& server::listen(const std::string& address, const std::string& port, ssl_method_t ssl_method,
		const std::string& private_key, const std::string& certificate_chain, bool is_file /*= true*/)
	{
		auto ssl_ctx = boost::make_shared<boost::asio::ssl::context>(static_cast<boost::asio::ssl::context::method>(ssl_method));
		if (is_file)
		{
			ssl_ctx->use_private_key_file(private_key, boost::asio::ssl::context::pem);
			ssl_ctx->use_certificate_chain_file(certificate_chain);
		}
		else
		{
			ssl_ctx->use_private_key(boost::asio::buffer(private_key), boost::asio::ssl::context::pem);
			ssl_ctx->use_certificate_chain(boost::asio::buffer(certificate_chain));
		}

		//HTTP2???
		//configure_tls_context_easy(ec, tls);
		auto acceprot = boost::make_shared<boost::asio::ip::tcp::acceptor>(io_service_pool_.get_io_service());
		do_listen(acceprot, address, port);
		start_accept(acceprot, ssl_ctx);
		return *this;
	}

	void server::run()
	{
		io_service_pool_.run();
	}

	void server::start_accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor)
	{
		auto new_conn = boost::make_shared<connection<boost::asio::ip::tcp::socket>>(
			io_service_pool_.get_io_service(), request_handler_);
		acceptor->async_accept(new_conn->socket(), [this, new_conn, acceptor](const boost::system::error_code& e)
		{
			if (!e)
			{
				new_conn->socket().set_option(boost::asio::ip::tcp::no_delay(true));
				new_conn->start();
			}
			else
			{
				std::cout << "server::handle_accept: " << e.message() << std::endl;
			}

			start_accept(acceptor);
		});
	}

	void server::start_accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor,
		boost::shared_ptr<boost::asio::ssl::context> const& ssl_ctx)
	{
		auto new_conn = boost::make_shared<connection<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>>(
			io_service_pool_.get_io_service(), request_handler_, *ssl_ctx);
		acceptor->async_accept(new_conn->socket().lowest_layer(), [this, new_conn, acceptor, ssl_ctx](const boost::system::error_code& e)
		{
			if (!e)
			{
				new_conn->socket().lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
				new_conn->reset_timer();
				new_conn->socket().async_handshake(boost::asio::ssl::stream_base::server,
					[new_conn](const boost::system::error_code &e)
				{
					if (e)
					{
						return;
					}
					//HTTP2???
// 					if (!tls_h2_negotiated(new_conn->socket()))
// 					{
// 						return;
// 					}
					new_conn->start();
				});
			}
			else
			{
				std::cout << "server::handle_accept: " << e.message() << std::endl;
			}

			start_accept(acceptor, ssl_ctx);
		});
	}

	void server::do_listen(boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor,
		const std::string& address, const std::string& port)
	{
		boost::asio::ip::tcp::resolver resolver(acceptor->get_io_service());
		boost::asio::ip::tcp::resolver::query query(address, port);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		acceptor->open(endpoint.protocol());
		acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor->bind(endpoint);
		acceptor->listen();
	}

	void server::stop()
	{
		io_service_pool_.stop();
	}

}
