#pragma once

#include "Network.h"
#include "NetworkManager.h"
#include "ServerHandleImpl.h"

using namespace MuteNet;

class ClientHandle : public Link::Callbacks
{
private:
	Link* _link;
public:
	ClientHandle() = default;
	virtual ~ClientHandle() {}

	virtual void OnCreated(Link* Link) override
	{
		_link = Link;
	}

	virtual void OnReceivedData(const char* data, size_t Length) override
	{
		if (Length <= 0)
		{
			_link->Shutdown();
		}

		_link->Send(data, Length);
	}

	virtual void OnRemoteClosed(void) override
	{

	}

	virtual void OnTLSHandShakeCompleted() override
	{

	}

	virtual void OnError(int ErrorCode, const std::string& ErrorMsg) override
	{
		std::cout << "Error : " << ErrorCode << " " << ErrorMsg << std::endl;

		_link->Shutdown();
	}
};

class ListenCallback : public Network::ListenCallbacks
{
public:
	ListenCallback() = default;

	virtual Link::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress, uint16_t RemotePort) override
	{
		printf("InComingConnection (%s:%d)", RemoteIPAddress.c_str(), RemotePort);

		return std::shared_ptr<ClientHandle>();
	}

	virtual void OnAccepted(Link& link) override
	{

	}

	virtual void OnError(int ErrorCode, const std::string ErrorMsg) override
	{
		std::cout << "Error : " << ErrorCode << " " << ErrorMsg << std::endl;
	}

};

typedef std::shared_ptr<ListenCallback> ListenCallbackPtr;


class ServerApplication
{
private:
	ListenCallbackPtr		_ListenCallback;
	ServerHandlePtr			_ServerHandlePtr;

public:
	ServerApplication();

	void Listen(const uint16_t Port);
};