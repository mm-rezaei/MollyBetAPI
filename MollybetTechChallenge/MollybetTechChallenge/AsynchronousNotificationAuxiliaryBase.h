#pragma once

class AsynchronousNotificationAuxiliaryBase
{
public:

	virtual bool ProcessNotification(boost::json::value& message) = 0;
};

