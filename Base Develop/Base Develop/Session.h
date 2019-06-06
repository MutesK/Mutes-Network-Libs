#pragma once
/*
	네트워크파트 부분에서, 유저가 가져야될 정보를 나타냄.

	User Content에 대한 코드는 일절 들어가서는 안된다.

	- Overlapped 이중 사용 금지
		IOCP 완료 통지시에, 해당 Overlapped 값이 달라져서, RingBuffer(환형버퍼)에 
		Index 처리가 안될수 있다.
	- 이중 완료통지 금지

	반드시 생성자, 소멸자 호출을 해야됨.
*/

#include "framework.h"


class Session final
{
public:
	Session(TcpSocketPtr& socket);
	~Session();

	inline LinkHandle getHandle();
	inline uint64_t getLastError();
private:
	void IOCallback(LPOVERLAPPED Overlapped, int ioSize);

	void RecvCompletionProcess(int ioSize);
	void SendCompletionProcess(int ioSize);

	void RecvPost();
private:
	TcpSocketPtr			_socket;

	OVERLAPPED				_sendOverlapped;
	OVERLAPPED				_recvOverlapped;

	std::function<void(LPOVERLAPPED, int)> _IOCallback;

	friend class IOCPManager;
	friend class Acceptor;
	friend class SessionManager;
};

inline LinkHandle Session::getHandle()
{
	return reinterpret_cast<LinkHandle>(_socket->get_handle());
}

inline uint64_t Session::getLastError()
{
	return _socket->getLastError();
}
