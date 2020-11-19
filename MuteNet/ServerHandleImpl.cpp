//
// Created by Mute on 2020-11-18.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "NetworkHelpers.hpp"
#include "ServerHandleImpl.hpp"
#include "ListenerComponent.hpp"
#include "SocketDescriptorHelper.hpp"
#include "IoContextEvent.hpp"
#include "TCPLinkImpl.hpp"
namespace MuteNet
{
    ServerHandleImpl::ServerHandleImpl(EventLoop::IOContextEvent& EventBase, NetworkHelpers::ListenCallbacksPtr _ListenCallbacks)
        :_ListenCallbacks(_ListenCallbacks), _EventBase(EventBase)
    {

    }

    void ServerHandleImpl::Close()
    {
        if(_Listener != nullptr)
        {
            _Listener->Disable();
        }
    }

    bool ServerHandleImpl::IsListening() const
    {
        return false;
    }

    ServerHandleImpl::~ServerHandleImpl()
    {

    }

    ServerHandleImplPtr ServerHandleImpl::Listen(EventLoop::IOContextEvent& EventBase, uint16_t Port, NetworkHelpers::ListenCallbacksPtr ListenCallbacks)
    {
        ServerHandleImplPtr Ptr{new ServerHandleImpl(EventBase, std::move(ListenCallbacks))};
        Ptr->_Self = Ptr;
        if (Ptr->Listen(Port))
        {

        }
        else
        {
            Ptr->_ListenCallbacks->OnError(Ptr->_ErrorCode, Ptr->_ErrorMsg);
            Ptr->_Self.reset();
        }
        return Ptr;
    }


    bool ServerHandleImpl::Listen(uint16_t a_Port)
    {
       descriptor_t listenfd = INVALID_SOCKET;

        listenfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
       if(listenfd == INVALID_SOCKET)
       {
           listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
           if (listenfd == INVALID_SOCKET)
           {
               return false;
           }

           if (0 != SocketDescriptorHelper::SetListenSocketReuseable(listenfd))
           {
                return false;
           }

           sockaddr_in name;
           memset(&name, 0, sizeof(name));
           name.sin_family = AF_INET;
           name.sin_port = ntohs(a_Port);

           if (bind(listenfd, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
           {
               return false;
           }

       }
       else
       {
           uint32_t Zero = 0;
#ifdef _WIN32
			int res = setsockopt(listenfd, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&Zero), sizeof(Zero));
			err = EVUTIL_SOCKET_ERROR();
			NeedsTwoSockets = ((res == SOCKET_ERROR) && (err == WSAENOPROTOOPT));
#else
           setsockopt(listenfd, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&Zero), sizeof(Zero));
#endif
           if (0 != SocketDescriptorHelper::SetListenSocketReuseable(listenfd))
           {
               return false;
           }

           sockaddr_in6 name;
           memset(&name, 0, sizeof(name));
           name.sin6_family = AF_INET6;
           name.sin6_port = ntohs(a_Port);
           if (bind(listenfd, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
           {
               return false;
           }
       }

        if (0 != SocketDescriptorHelper::SetSocketNonblock(listenfd))
        {
            return false;
        }

        if(0 != listen(listenfd, SOMAXCONN))
        {
            return false;
        }

        _Listener = _EventBase.CreateListener(Callback, this, listenfd);
    }

    void
    ServerHandleImpl::Callback(EventLoop::ListenerComponent *pRawListener, DescriptorPtr Socket, sockaddr *Addr, int Len, void *a_Self)
    {
        ServerHandleImpl * Self = static_cast<ServerHandleImpl *>(a_Self);
        assert(Self != nullptr);
        assert(Self->_Self != nullptr);

        char IPAddress[128];
        uint16_t Port = 0;
        switch (Addr->sa_family)
        {
            case AF_INET:
            {
                sockaddr_in * sin = reinterpret_cast<sockaddr_in *>(Addr);

                Port = ntohs(sin->sin_port);
                break;
            }
            case AF_INET6:
            {
                sockaddr_in6 * sin6 = reinterpret_cast<sockaddr_in6 *>(Addr);

                Port = ntohs(sin6->sin6_port);
                break;
            }
        }

        auto LinkCallbacks = Self->_ListenCallbacks->OnInComingConnection(IPAddress, Port);
        if (LinkCallbacks == nullptr)
        {
            // Drop the connection:
            delete Socket;
            return;
        }

        TCPLinkImplPtr Link = std::make_shared<TCPLinkImpl>(Self->_EventBase,
                                                            Socket, LinkCallbacks,
                                                            Self->_Self, Addr, Len);

        LinkCallbacks->OnCreated(Link.get());
        Link->Enable(Link);

        // Call the OnAccepted callback:
        Self->_ListenCallbacks->OnAccepted(*Link);
    }

    void ServerHandleImpl::RemoveLink(const TCPLinkImpl *a_Link)
    {

    }
}