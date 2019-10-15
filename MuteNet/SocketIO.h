#pragma once

#include "Socket.h"
#include "SocketFunctionInvoker.h"

namespace MuteNet
{
	typedef LPFN_ACCEPTEX						AcceptExPtr;
	typedef LPFN_CONNECTEX						ConnectExPtr;
	typedef LPFN_GETACCEPTEXSOCKADDRS			GetAcceptExSockAddrsPtr;

	typedef void(*IOCPCallbackPtr)(struct Overlapped*, uintptr_t, int, int success);

	struct ExtensionFunctions
	{
		AcceptExPtr					_AcceptEx;
		ConnectExPtr				_ConnectEx;
		GetAcceptExSockAddrsPtr		_GetAcceptExSockaddrs;
	};

	struct Overlapped
	{
		OVERLAPPED				_Overlapped;
		IOCPCallbackPtr			_CallbackPtr;
	};

	inline void* GetExtensionFunction(SOCKET s, const GUID* fn)
	{
		void* ptr = nullptr;
		DWORD bytes = 0;

		SocketDelegateInvoker::Invoke(WSAIoctl, SIO_GET_EXTENSION_FUNCTION_POINTER,
			(GUID*)fn, sizeof(*fn), &ptr, sizeof(ptr), &bytes, nullptr, nullptr);

		return ptr;
	}

	inline void InitExtensionFunctions(struct ExtensionFunctions* ext)
	{
		const GUID acceptex = WSAID_ACCEPTEX;
		const GUID connectex = WSAID_CONNECTEX;
		const GUID getaccpetexsockaddrs = WSAID_GETACCEPTEXSOCKADDRS;

		SOCKET serviceProvider = socket(AF_INET, SOCK_STREAM, 0);

		if (serviceProvider == INVALID_SOCKET)
			return;

		ext->_AcceptEx = 
			reinterpret_cast<AcceptExPtr>(GetExtensionFunction(serviceProvider, &acceptex));
		ext->_ConnectEx =
			reinterpret_cast<ConnectExPtr>(GetExtensionFunction(serviceProvider, &connectex));
		ext->_GetAcceptExSockaddrs =
			reinterpret_cast<GetAcceptExSockAddrsPtr>(GetExtensionFunction(serviceProvider, &getaccpetexsockaddrs));


		closesocket(serviceProvider);


	}
}
