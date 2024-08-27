#pragma once

class SynchronousRequestBase
{
public:

	SynchronousRequestBase(ConnectionInfo& connectionInfo)
		: ConnectionInformation(connectionInfo)
	{

	}

	virtual ~SynchronousRequestBase() = default;

	http::request<http::string_body> GetRequest()
	{
		http::request<http::string_body> result = CreateRequest();

		PreProcessRequest(result);
		ProcessRequest(result);
		PostProcessRequest(result);

		return std::move(result);
	}

protected:

	ConnectionInfo& ConnectionInformation;

	virtual http::request<http::string_body> CreateRequest() = 0;
	virtual void ProcessRequest(http::request<http::string_body>& request) = 0;

	virtual void PreProcessRequest(http::request<http::string_body>& request)
	{
		request.set(http::field::host, ConnectionInformation.Host);
		request.set(http::field::accept, "application/json");
		request.set(http::field::content_type, "application/json");
		request.set(http::field::connection, "keep-alive");
	}

	virtual void PostProcessRequest(http::request<http::string_body>& request)
	{
		request.prepare_payload();
	}
};

