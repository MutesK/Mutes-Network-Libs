#pragma once

#include "MuteNetFoundation.h"

namespace Network
{
	/*
	 *  SOCKADDR Wrapper Class
	 *
	 *	유저 레벨에서는 이 클래스를 통해 쉽게 주소정보를 가져오도록 한다.
	 */
	class ConnectPoint final
	{
	public:		
		ConnectPoint() = default;
		~ConnectPoint() = default;

		ConnectPoint(ConnectPoint&& Point) noexcept;
		ConnectPoint& operator=(ConnectPoint&& Point) noexcept;

		void SetConnectPoint(const std::string& address, uint16_t inPort);
		void SetConnectPoint(const ConnectPoint& ConnectPoint);
		void SetConnectPoint(const sockaddr& sockAddr);
		void SetConnectPoint(const sockaddr_in& sockAddr_in);

		sockaddr* GetSocketConnectPointPtr();
		std::string GetIpConnectPoint() const;
		int	GetPort() const;

		static int GetSize();
	private:
		void CreateSockAddr(const std::string& address, uint16_t port);
	private:
		sockaddr_in _sockAddr{};
	};


	inline ConnectPoint& ConnectPoint::operator=(ConnectPoint&& Point) noexcept
	{
		_sockAddr = std::move(Point._sockAddr);
		return *this;
	}

	inline sockaddr* ConnectPoint::GetSocketConnectPointPtr()
	{
		return reinterpret_cast<sockaddr*>(&_sockAddr);
	}

	inline int ConnectPoint::GetPort() const
	{
		return ntohs(_sockAddr.sin_port);
	}

	inline int ConnectPoint::GetSize()
	{
		return sizeof(sockaddr);
	}

	inline void ConnectPoint::SetConnectPoint(const std::string& ConnectPoint, uint16_t inPort)
	{
		CreateSockAddr(ConnectPoint, inPort);
	}

	inline void ConnectPoint::SetConnectPoint(const ConnectPoint& ConnectPoint)
	{
		memcpy(&_sockAddr, &ConnectPoint._sockAddr, sizeof(sockaddr_in));
	}

	inline void ConnectPoint::SetConnectPoint(const sockaddr& sockAddr)
	{
		memcpy(&_sockAddr, &sockAddr, sizeof(sockaddr_in));
	}

	inline void ConnectPoint::SetConnectPoint(const sockaddr_in& sockAddr_in)
	{
		memcpy(&_sockAddr, &sockAddr_in, sizeof(sockaddr_in));
	}
}
