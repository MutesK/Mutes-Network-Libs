#include "ServerApplication.h"
#include "Logger.h"


class ConsoleLogListener : public Logger::Listener
{
public:
	virtual void Log(const std::string logFmt, LogLevel level) override
	{
		std::cout << logFmt << std::endl;
	}
};

ServerApplication::ServerApplication()
	:_ListenCallback(std::make_shared<ListenCallback>())
{
	Util::Logger::Get().AttachListener(new ConsoleLogListener());

	Util::Logger::Get().StartRun();
}

void ServerApplication::Listen(const uint16_t Port)
{
	_ServerHandlePtr = Network::Listen(Port, _ListenCallback);
}
