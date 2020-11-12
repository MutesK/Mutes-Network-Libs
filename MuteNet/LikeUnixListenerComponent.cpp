//
// Created by junmkim on 2020-11-12.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "LikeUnixListenerComponent.h"
#include "IoContextThreadPool.hpp"

namespace EventLoop
{
    LikeUnixListenerComponent::LikeUnixListenerComponent ( RawIOContextImplPtr const &ContextEvent,
                                                           ListenerComponent::CallbackDelegate &&Callback, void *Self,
                                                           uint32_t Flag, int backlog, socket_t listenSocket )
            : ListenerComponent ( ContextEvent, std::move(Callback), Self, Flag, backlog, listenSocket )
    {

    }

    void LikeUnixListenerComponent::DoWork ( )
    {
        struct sockaddr address;
        socklen_t addresslength = sizeof(address);

        if(-1 == getsockname(_ListenSocket, &address, &addresslength))
        {
            return;
        }

        struct sockaddr* clientAddress;
        socklen_t clientAddressLength = 0;


        switch(address.sa_family)
        {
            case AF_INET:
                clientAddress = reinterpret_cast<struct sockaddr *>(new sockaddr_in());
                clientAddressLength = sizeof(sockaddr_in);
                break;
            case AF_INET6:
                clientAddress = reinterpret_cast<struct sockaddr *>(new sockaddr_in6());
                clientAddressLength = sizeof(sockaddr_in6);
                break;
        }

        while(!IsStop())
        {
            socket_t clientsocket = accept ( _ListenSocket,
                                             reinterpret_cast<struct sockaddr *>(&clientAddress),
                                             &clientAddressLength );
            if ( -1 == clientsocket )
            {
                continue;
            }

            _ListenCallbackDelegate(this, clientsocket, clientAddress, clientAddressLength, _Self);
        }

        delete clientAddress;
    }
}
