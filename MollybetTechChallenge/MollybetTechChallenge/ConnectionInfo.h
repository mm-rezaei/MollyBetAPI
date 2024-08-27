#pragma once

class ConnectionInfo
{
public:

	ConnectionInfo() = delete;

	ConnectionInfo(const std::string host, const std::string port)
		: Host(host), Port(port)
	{
	}

	const std::string Host;

	const std::string Port;
};