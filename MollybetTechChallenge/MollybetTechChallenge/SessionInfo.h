#pragma once

class SessionInfo
{
public:

	SessionInfo() = delete;

	SessionInfo(const std::string sessionCode)
		: SessionCode(sessionCode)
	{
	}

	const std::string SessionCode;
};