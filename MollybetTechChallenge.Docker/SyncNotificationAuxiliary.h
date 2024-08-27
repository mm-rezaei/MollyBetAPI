#pragma once

class SyncNotificationAuxiliary : public AsynchronousNotificationAuxiliaryBase
{
public:
    std::set<std::string> CompetitionNames;

    bool ProcessNotification(boost::json::value& message) override
    {
        if (message.is_object())
        {
            auto object = message.as_object();

            if (object.contains("data"))
            {
                const auto& dataobj = message.as_object().at("data");

                if (dataobj.is_array())
                {
                    return ProcessDataArray(dataobj.as_array());
                }
            }
        }
        return true;
    }

private:

    bool ProcessDataArray(const boost::json::array& dataarray)
    {
        for (const auto& item : dataarray)
        {
            if (item.is_array())
            {
                if (!ProcessEventArray(item.as_array()))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool ProcessEventArray(const boost::json::array& eventArray)
    {
        if (!eventArray.empty() && eventArray[0].is_string())
        {
            const std::string type = eventArray[0].as_string().c_str();

            if (type == "sync")
            {
                return false;
            }
            else if (type == "event" && eventArray.size() > 1)
            {
                ProcessEventObject(eventArray[1]);
            }
        }
        return true;
    }

    void ProcessEventObject(const boost::json::value& eventObject)
    {
        if (eventObject.is_object())
        {
            const auto& obj = eventObject.as_object();

            if (obj.contains("competition_name"))
            {
                const auto& nameValue = obj.at("competition_name");

                if (nameValue.is_string())
                {
                    CompetitionNames.insert(nameValue.as_string().c_str());
                }
            }
        }
    }
};


//class SyncNotificationAuxiliary : public AsynchronousNotificationAuxiliaryBase
//{
//public:
//
//	std::set<std::string> CompetitionNames;
//
//	bool ProcessNotification(boost::json::value& message) override
//	{
//		bool result = true;
//
//		auto obj = message.as_object();
//
//		if (obj.contains("data"))
//		{
//			auto dataobj = obj["data"];
//
//			if (dataobj.is_array())
//			{
//				auto dataarray = dataobj.as_array();
//
//				for (auto& dataarrayitem : dataarray)
//				{
//					if (dataarrayitem.is_array())
//					{
//						auto dataarrayitemarray = dataarrayitem.as_array();
//
//						if (!dataarrayitemarray.empty())
//						{
//							auto dataarrayitemarrayobj1 = dataarrayitemarray[0];
//
//							if (dataarrayitemarrayobj1.is_string())
//							{
//								std::string dataarrayitemarrayobj1sync = dataarrayitemarrayobj1.as_string().c_str();
//
//								if (dataarrayitemarrayobj1sync == "sync")
//								{
//									result = false;
//
//									break;
//								}
//								else if (dataarrayitemarrayobj1sync == "event")
//								{
//									if (dataarrayitemarray.size() == 2)
//									{
//										auto dataarrayitemarrayobj2 = dataarrayitemarray[1];
//
//										if (dataarrayitemarrayobj2.is_object())
//										{
//											auto dataarrayitemarrayobj2eventobj = dataarrayitemarrayobj2.as_object();
//
//											if (dataarrayitemarrayobj2eventobj.contains("competition_name"))
//											{
//												auto dataarrayitemarrayobj2eventobjcompetitionname = dataarrayitemarrayobj2eventobj["competition_name"];
//
//												if (dataarrayitemarrayobj2eventobjcompetitionname.is_string())
//												{
//													std::string competition_name = dataarrayitemarrayobj2eventobjcompetitionname.as_string().c_str();
//
//													CompetitionNames.insert(competition_name);
//												}
//											}
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//
//		return result;
//	}
//};

