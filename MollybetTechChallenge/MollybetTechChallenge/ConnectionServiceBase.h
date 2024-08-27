#pragma once

class ConnectionServiceBase
{
public:

    virtual ~ConnectionServiceBase() = default;

    virtual bool OpenConnection() = 0;

    virtual void CloseConnection() = 0;

    virtual bool IsConnectionOpen() = 0;
};