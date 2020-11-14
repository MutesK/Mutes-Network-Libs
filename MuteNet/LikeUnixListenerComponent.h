//
// Created by junmkim on 2020-11-12.
//

#ifndef MUTENET_LIKEUNIXLISTENERCOMPONENT_H
#define MUTENET_LIKEUNIXLISTENERCOMPONENT_H

#include "ListenerComponent.hpp"
#include <Runnable.hpp>

#if defined(UNIX)
namespace EventLoop
{
    class LikeUnixListenerComponent : public ListenerComponent, public Util::Runnable
    {
        virtual ~LikeUnixListenerComponent();
        
        friend class SelectIOContext;
    private:
    	LikeUnixListenerComponent(const RawIOContextImplPtr &ContextEvent,
                                  ListenerComponent::CallbackDelegate &&Callback,
                                  void *Self, uint32_t Flag, int backlog, socket_t listenSocket);
    	        
        virtual void DoWork ( ) override;
    };
}
#endif


#endif //MUTENET_LIKEUNIXLISTENERCOMPONENT_H
