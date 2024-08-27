#pragma once

#include "SynchronousRequestBase.h"
#include "SynchronousResponseBase.h"

class SynchronousConnectionService : public ConnectionServiceBase
{
public:

	SynchronousConnectionService() = delete;

	SynchronousConnectionService(StreamAuxiliary<beast::tcp_stream>& streamContext)
		: StreamContext(streamContext)
	{
	}

	bool OpenConnection() override
	{
		bool result = false;

		if (StreamContext.WasInitialized())
		{
			try
			{
				beast::tcp_stream& lowestLayer = StreamContext.GetLowestLayer();

				lowestLayer.connect(StreamContext.GetResolvedEndpoints());

				if (SSL_set_tlsext_host_name(StreamContext.Stream.native_handle(), StreamContext.ConnectionInformation.Host.c_str()))
				{
					StreamContext.Stream.handshake(ssl::stream_base::client);

					result = lowestLayer.socket().is_open();
				}
				else
				{
					beast::error_code error(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category());

					std::cerr << "Failed to set the SNI hostname. Please verify the hostname and ensure the SSL/TLS configuration is correct: " << error.what() << std::endl;
				}
			}
			catch (beast::system_error& se)
			{
				std::cerr << "Error in the synchronous connection: " << se.what() << std::endl;
			}
			catch (std::exception& e)
			{
				std::cerr << "Error during the synchronous connection: " << e.what() << std::endl;
			}
		}

		return result;
	}

	void CloseConnection() override
	{
		try
		{
			beast::error_code error;

			StreamContext.Stream.shutdown(error);

			if (error == net::error::eof)
			{
				std::cerr << "Remote host closed the connection without performing the SSL shutdown handshake." << std::endl;
			}
			else if (error == ssl::error::stream_truncated)
			{
				std::cerr << "The connection was closed abruptly without SSL shutdown." << std::endl;
			}
			else if (error)
			{
				std::cerr << "Failed to shutdown the SSL stream properly." << std::endl;
			}
		}
		catch (std::exception& e)
		{
			std::cerr << "Unexpected error during connection closure: " << e.what() << std::endl;
		}
	}

	bool IsConnectionOpen() override
	{
		beast::tcp_stream& lowestLayer = StreamContext.GetLowestLayer();

		return lowestLayer.socket().is_open();
	}

	bool ExecuteRequest(SynchronousRequestBase& synchronousRequest, SynchronousResponseBase& synchronousResponse)
	{
		bool result = false;

		try
		{
			http::write(StreamContext.Stream, synchronousRequest.GetRequest());

			beast::flat_buffer buffer;
			http::response<http::dynamic_body> response;

			http::read(StreamContext.Stream, buffer, response);

			synchronousResponse.Initialize(response);

			result = true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error during executing the request: " << e.what() << std::endl;
		}

		return result;
	}

private:

	StreamAuxiliary<beast::tcp_stream>& StreamContext;
};