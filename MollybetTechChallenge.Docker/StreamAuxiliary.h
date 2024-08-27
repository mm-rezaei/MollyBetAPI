#pragma once

template <typename StreamType>
class StreamAuxiliary
{
public:

	StreamAuxiliary() = delete;

	StreamAuxiliary(ConnectionInfo& connectionInfo, boost::asio::io_context& iocontext, boost::asio::ssl::context& context)
		: ConnectionInformation(connectionInfo), ioc(iocontext), sslContext(context), Stream(iocontext, context)
	{
	}

	ConnectionInfo& ConnectionInformation;

	beast::ssl_stream<StreamType> Stream;

	bool InitializeStream()
	{
		if (!initialized)
		{
			try
			{
				sslContext.set_default_verify_paths();
				ip::tcp::resolver resolver(Stream.get_executor());
				resolvedEndpoints = resolver.resolve(ConnectionInformation.Host, ConnectionInformation.Port);
				initialized = true;
			}
			catch (beast::system_error& se)
			{
				std::cerr << "Error during stream context initialization: " << se.what() << std::endl;
			}
			catch (std::exception& e)
			{
				std::cerr << "Unexpected error during stream context initialization: " << e.what() << std::endl;
			}
		}

		return initialized;
	}

	bool WasInitialized()
	{
		return initialized;
	}

	boost::asio::io_context& GetIoContext()
	{
		return ioc;
	}

	ip::tcp::resolver::results_type& GetResolvedEndpoints()
	{
		return resolvedEndpoints;
	}

	StreamType& GetLowestLayer()
	{
		StreamType& result = beast::get_lowest_layer(Stream);

		return result;
	}

protected:

	boost::asio::io_context& ioc;
	boost::asio::ssl::context& sslContext;
	bool initialized = false;
	ip::tcp::resolver::results_type resolvedEndpoints;
};