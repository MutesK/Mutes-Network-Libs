#pragma once

#include "MuteNetFoundation.h"
#include "EndPoint.h"

namespace Network
{

	enum class ShutdownBlockMode
	{
		RecvBlock = SD_RECEIVE,
		SendBlock = SD_SEND,
		BothBlock = SD_BOTH,
	};

	class SocketAddress;
	class Socket
	{
	protected:
		Socket(ADDRESS_FAMILY);
	public:
		virtual ~Socket();

		virtual bool Bind(const EndPoint& endpoint) = 0;
		virtual bool Bind(const std::string& connection) = 0;
		virtual bool Bind(const std::string& ip, uint16_t port) = 0;

		int SetIoMode(bool ioflag);

		int GetLastError() const;

		int SetRecvSocketBufferSize(int size) const;
		int SetSendSocketBufferSize(int size) const;

		int SetReUseAddress(bool toggle) const;

		int SetRecvCallLimitTime(DWORD time) const;
		int SetSendCallLimitTime(DWORD time) const;

		int SetUseKeepAlive(bool toggle) const;
		int Shutdown(ShutdownBlockMode Mode);

		HANDLE native_handle() const;
		void set_native_handle(HANDLE handle);

		friend class Link;

	protected:
		SOCKET			_handle;
		DWORD			_lastError;
		ADDRESS_FAMILY	_address_family;
		EndPoint		_endPoint;
	};


	inline int Socket::GetLastError() const
	{
		return _lastError;
	}

	inline int Socket::SetRecvSocketBufferSize(int size) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&size), sizeof(int));
	}

	inline int Socket::SetSendSocketBufferSize(int size) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&size), sizeof(int));
	}

	inline int Socket::SetReUseAddress(bool toggle) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&toggle), sizeof(bool));
	}

	inline int Socket::SetRecvCallLimitTime(DWORD time) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&time), sizeof(DWORD));
	}

	inline int Socket::SetSendCallLimitTime(DWORD time) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&time), sizeof(DWORD));
	}

	inline int Socket::SetUseKeepAlive(bool toggle) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<const char*>(&toggle), sizeof(bool));
	}

	inline HANDLE Socket::native_handle() const
	{
		return reinterpret_cast<HANDLE>(_handle);
	}

	inline void Socket::set_native_handle(HANDLE handle)
	{
		_handle = reinterpret_cast<SOCKET>(handle);
	}
}
