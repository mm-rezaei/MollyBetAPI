#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <deque>
#include <iostream>
#include <set>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace ip = boost::asio::ip;
namespace json = boost::json;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
namespace websocket = beast::websocket;

#include "ConnectionInfo.h"
#include "SessionInfo.h"
#include "ResponseStatus.h"
#include "StreamAuxiliary.h"
#include "ConnectionServiceBase.h"
#include "AsynchronousConnectionService.h"
#include "SynchronousConnectionService.h"
#include "AsynchronousNotificationAuxiliaryBase.h"
#include "SyncNotificationAuxiliary.h"
#include "SynchronousRequestBase.h"
#include "SynchronousResponseBase.h"
#include "LoginSynchronousRequest.h"
#include "LoginSynchronousResponse.h"

std::string GetSessionCode(ConnectionInfo& connectionInfo, std::string& username, std::string& password)
{
	std::string result = "";

	net::io_context iocontext;
	ssl::context ctx(ssl::context::tlsv12_client);

	StreamAuxiliary<beast::tcp_stream> stream(connectionInfo, iocontext, ctx);

	//
	if (!stream.InitializeStream())
	{
		return result;
	}

	//
	SynchronousConnectionService connection(stream);

	if (!connection.OpenConnection())
	{
		return result;
	}

	//
	LoginSynchronousRequest request(connectionInfo, username, password);
	LoginSynchronousResponse response;

	if (!connection.ExecuteRequest(request, response) || response.GetHttpStatus() != http::status::ok || response.GetResponseStatus() != ResponseStatus::Ok)
	{
		std::cerr << "Failed to login." << std::endl;

		connection.CloseConnection();

		return result;
	}

	//
	result = response.GetSessionCode();

	return result;
}

std::set<std::string> GetCompetitionNames(ConnectionInfo& connectionInfo, SessionInfo& sessionInfo)
{
	std::set<std::string> result;

	net::io_context iocontext;
	ssl::context ctx(ssl::context::tlsv12_client);

	StreamAuxiliary<ip::tcp::socket> stream(connectionInfo, iocontext, ctx);

	//
	if (!stream.InitializeStream())
	{
		return result;
	}

	//
	AsynchronousConnectionService connection(stream, sessionInfo);

	if (!connection.OpenConnection())
	{
		return result;
	}

	//
	SyncNotificationAuxiliary syncNotification;

	connection.ListenToAsynchronousNotification(syncNotification);

	iocontext.run();

	//
	result = std::move(syncNotification.CompetitionNames);

	//
	connection.CloseConnection();

	return result;
}

int main()
{
	try
	{
		std::string host = "api.mollybet.com";
		std::string port = "443";
		std::string username;
		std::string password;

		std::cout << "Enter username: ";
		std::getline(std::cin, username);

		std::cout << "Enter password: ";
		std::getline(std::cin, password);

		ConnectionInfo connectionInfo(host, port);

		std::string sessionCode = GetSessionCode(connectionInfo, username, password);

		if (!sessionCode.empty())
		{
			SessionInfo sessionInfo(sessionCode);

			auto competitionNames = GetCompetitionNames(connectionInfo, sessionInfo);

			std::cout << std::endl << "Distinct competition names received:" << std::endl << std::endl;

			for (const auto& name : competitionNames)
			{
				std::cout << name << std::endl;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
