
#include "Link.h"
#include "IOContext.h"

using namespace Util;
namespace Network
{
	Util::TL::ObjectPool<SendContext> SendContext::OverlappedPool;
	Util::TL::ObjectPool<RecvContext> RecvContext::OverlappedPool;

	Link::Link()
		:_Socket(AF_INET)
	{
		_CallBack = std::bind(&Link::IOCompletion, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3);
	}

	Link::~Link()
	{
	}

	void Link::Recv()
	{
		const auto Overlapped = RecvContext::OverlappedPool(shared_from_this(), _CallBack);

		// WSABUF�� �ӽ÷� nullptr
		_Socket.OverlappedIORecv(nullptr, 0, &Overlapped->Overlapped);
	}

	void Link::Send()
	{
		const auto Overlapped = SendContext::OverlappedPool(shared_from_this(), _CallBack);

		_Socket.OverlappedIOSend(nullptr, 0, &Overlapped->Overlapped);
	}

	void Link::IOCompletion(IOContext* pContext, DWORD TransferredBytes, void* CompletionKey)
	{
		// IO Completion Process
		if(TransferredBytes == 0)
		{
			_Socket.Shutdown(ShutdownBlockMode::BothBlock);
		}

		pContext->~IOContext();

		switch(pContext->Type)
		{
		case IO_RECV:
			RecvContext::OverlappedPool.Free(reinterpret_cast<RecvContext *>(pContext));
			break;
		case IO_SEND:
			SendContext::OverlappedPool.Free(reinterpret_cast<SendContext*>(pContext));
			break;
		default:
			// Crash & Logger
			break;
		}
	}
}
