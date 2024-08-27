#pragma once

class SynchronousResponseBase
{
public:

	http::status GetHttpStatus()
	{
		return httpStatus;
	}

	ResponseStatus GetResponseStatus()
	{
		return responseStatus;
	}

protected:

	std::unique_ptr<boost::json::value> responseValue;
	http::status httpStatus;
	ResponseStatus responseStatus;

	virtual void Initialize(http::response<http::dynamic_body>& httpResponse)
	{
		responseValue = std::make_unique<boost::json::value>(json::parse(beast::buffers_to_string(httpResponse.body().data())));

		httpStatus = httpResponse.result();

		std::string status = responseValue->at("status").as_string().c_str();

		if (status == "ok")
		{
			responseStatus = ResponseStatus::Ok;
		}
		else
		{
			responseStatus = ResponseStatus::Error;
		}
	}

	friend class SynchronousConnectionService;
};

