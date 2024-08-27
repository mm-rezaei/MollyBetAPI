#pragma once

class LoginSynchronousResponse : public SynchronousResponseBase
{
public:

	LoginSynchronousResponse()
		: SynchronousResponseBase()
	{
	}

	std::string& GetSessionCode()
	{
		return sessionCode;
	}

private:

	std::string sessionCode;

	void Initialize(http::response<http::dynamic_body>& response) override
	{
		SynchronousResponseBase::Initialize(response);

		sessionCode = responseValue->at("data").as_string().c_str();
	}
};

