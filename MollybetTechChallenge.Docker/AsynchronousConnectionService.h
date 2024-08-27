#pragma once

#include "AsynchronousNotificationAuxiliaryBase.h"

class AsynchronousConnectionService : public ConnectionServiceBase
{
public:

	AsynchronousConnectionService() = delete;

	AsynchronousConnectionService(StreamAuxiliary<ip::tcp::socket>& streamContext, SessionInfo& sessionInfo)
		: StreamContext(streamContext), SessionInformation(sessionInfo)
	{
	}

	std::unique_ptr<websocket::stream<beast::ssl_stream<ip::tcp::socket>>> WebSocket;

	bool OpenConnection() override
	{
		bool result = false;

		if (StreamContext.WasInitialized())
		{
			try
			{
				ip::tcp::socket& lowestLayer = StreamContext.GetLowestLayer();

				auto resolvedEndpoints = StreamContext.GetResolvedEndpoints();

				net::connect(lowestLayer, resolvedEndpoints.begin(), resolvedEndpoints.end());

				StreamContext.Stream.handshake(ssl::stream_base::client);

				WebSocket = std::make_unique<websocket::stream<beast::ssl_stream<ip::tcp::socket>>>(std::move(StreamContext.Stream));

				std::string target = "/v1/stream/?token=" + SessionInformation.SessionCode;

				WebSocket->handshake(StreamContext.ConnectionInformation.Host + ":" + StreamContext.ConnectionInformation.Port, target);

				result = WebSocket->is_open();
			}
			catch (std::exception& e)
			{
				std::cerr << "Error during the asynchronous connection: " << e.what() << std::endl;
			}
		}

		return result;
	}

	void CloseConnection() override
	{
		if (WebSocket)
		{
			try
			{
				WebSocket->close(websocket::close_code::normal);
			}
			catch (websocket::close_code& e)
			{
				std::cerr << "Error during WebSocket closure (specific WebSocket error): " << e << std::endl;
			}
			catch (boost::system::system_error& e)
			{
				std::cerr << "System error during WebSocket closure: " << e.what() << std::endl;
			}
			catch (std::exception& e)
			{
				std::cerr << "Unexpected error during WebSocket closure: " << e.what() << std::endl;
			}
		}
		else
		{
			std::cerr << "WebSocket is not initialized, cannot close connection." << std::endl;
		}
	}

	bool IsConnectionOpen() override
	{
		return WebSocket->is_open();
	}

	void ListenToAsynchronousNotification(AsynchronousNotificationAuxiliaryBase& asynchronousNotificationAuxiliary)
	{
		//
		auto buffer = std::make_shared<beast::flat_buffer>();
		auto messageQueue = std::make_shared<std::deque<std::string>>();
		auto queueMutex = std::make_shared<std::mutex>();

		boost::asio::io_context& ioc = StreamContext.GetIoContext();
		websocket::stream<beast::ssl_stream<ip::tcp::socket>>& currentwebSocket = *WebSocket;
		AsynchronousNotificationAuxiliaryBase& notificationAuxiliary = asynchronousNotificationAuxiliary;

		//
		auto processMessages = [messageQueue, queueMutex, &ioc, &notificationAuxiliary]()
			{
				std::lock_guard<std::mutex> lock(*queueMutex);

				while (!messageQueue->empty())
				{
					std::string message = std::move(messageQueue->front());
					messageQueue->pop_front();

					boost::json::value parsedMessage = boost::json::parse(message);

					if (!parsedMessage.is_object() || !notificationAuxiliary.ProcessNotification(parsedMessage))
					{
						ioc.stop();

						return;
					}
				}
			};

		//
		auto readHandler = std::make_shared<std::function<void(boost::system::error_code, std::size_t)>>();

		*readHandler = [buffer, &currentwebSocket, &ioc, messageQueue, queueMutex, processMessages, readHandler](boost::system::error_code ec, std::size_t bytes_transferred)
			{
				if (!ec)
				{
					auto message = beast::buffers_to_string(buffer->data());

					{
						std::lock_guard<std::mutex> lock(*queueMutex);
						messageQueue->emplace_back(std::move(message));
					}

					buffer->consume(buffer->size());

					net::post(ioc, processMessages);

					currentwebSocket.async_read(*buffer, *readHandler);
				}
				else
				{
					std::cerr << "Error in receiving notifications: " << ec.message() << std::endl;
				}
			};

		//
		currentwebSocket.async_read(*buffer, *readHandler);
	}

private:

	StreamAuxiliary<ip::tcp::socket>& StreamContext;
	SessionInfo& SessionInformation;
};