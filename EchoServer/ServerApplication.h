#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/Acceptor.h"
#include "../MuteNet/IOService.h"

class ServerApplication
{
private:
	std::unique_ptr<Network::IOService> _service;
	std::unique_ptr<Network::Acceptor> _acceptor;

	std::chrono::steady_clock::time_point _timepoint;
	std::atomic_int64_t recvBytes = 0;
	std::atomic_int64_t sendBytes = 0;
public:
	ServerApplication();

	bool Open();
	void Monitoring();
private:
	void OnAccepted(const Network::LinkPtr&);
	void OnRecived(const Network::LinkPtr&, std::shared_ptr<Util::InputMemoryStream>&);
	void OnSended(const Network::LinkPtr&, size_t SendedSize);
};
