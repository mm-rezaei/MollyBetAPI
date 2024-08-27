#pragma once

class LoginSynchronousRequest : public SynchronousRequestBase
{
public:

	LoginSynchronousRequest(ConnectionInfo& connectionInfo, const std::string username, const std::string password)
		: SynchronousRequestBase(connectionInfo), Username(username), Password(password)
	{
	}

	http::request<http::string_body> CreateRequest() override
	{
		http::request<http::string_body> result{ http::verb::post, "/v1/sessions/", 11 };

		return std::move(result);
	}

	void ProcessRequest(http::request<http::string_body>& request) override
	{
		json::object body;

		body["username"] = Username;
		body["password"] = Password;

		request.body() = json::serialize(body);
	}

private:

	std::string Username;
	std::string Password;
};

