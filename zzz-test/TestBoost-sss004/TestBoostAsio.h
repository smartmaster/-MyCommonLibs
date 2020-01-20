#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

namespace Smartlib
{
	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		explicit Session(tcp::socket socket)
			: socket_(std::move(socket)),
			timer_(socket_.get_io_context()),
			strand_(socket_.get_io_context())
		{
		}

		void go()
		{
			auto self(shared_from_this());
			boost::asio::spawn(strand_,
				[this, self](boost::asio::yield_context yield)
			{
				try
				{
					char data[128];
					for (;;)
					{
						timer_.expires_from_now(std::chrono::seconds(10));
						std::size_t n = socket_.async_read_some(boost::asio::buffer(data), yield);
						boost::asio::async_write(socket_, boost::asio::buffer(data, n), yield);
					}
				}
				catch (std::exception& e)
				{
					socket_.close();
					timer_.cancel();
				}
			});

			boost::asio::spawn(strand_,
				[this, self](boost::asio::yield_context yield)
			{
				while (socket_.is_open())
				{
					boost::system::error_code ignored_ec;
					timer_.async_wait(yield[ignored_ec]);
					if (timer_.expires_from_now() <= std::chrono::seconds(0))
						socket_.close();
				}
			});
		}

	private:
		tcp::socket socket_;
		boost::asio::steady_timer timer_;
		boost::asio::io_context::strand strand_;
	};


	class Server
	{
	public:
		static int Main()
		{
			try
			{
				boost::asio::io_context io_context;

				boost::asio::spawn(io_context,
					[&](boost::asio::yield_context yield)
				{
					tcp::acceptor acceptor(io_context,
						tcp::endpoint(tcp::v4(), 60080));

					for (;;)
					{
						boost::system::error_code ec;
						tcp::socket socket(io_context);
						acceptor.async_accept(socket, yield[ec]);
						if (!ec) std::make_shared<Session>(std::move(socket))->go();
					}
				});

				io_context.run();
			}
			catch (std::exception& e)
			{
				std::cerr << "Exception: " << e.what() << "\n";
			}

			return 0;
		}
	};

} //namespace Smartlib