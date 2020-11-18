//
// Created by JunMin Kim on 20. 11. 15..
//

#ifndef MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP
#define MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP

#ifdef POSIX_PLATFORM

#include "IoContextImpl.hpp"

namespace EventLoop
{
    using DescriptorPerSocketPtrMapWithLock = LockObject<std::unordered_map<descriptor_t, DescriptorPtr>>;

    class IUnixLikeIOContextImpl : public IOContextImpl
    {
    protected:
        DescriptorPerSocketPtrMapWithLock _SocketMap;
    public:
        IUnixLikeIOContextImpl(IOContextEvent &Event,
                               const uint32_t NumOfWorkerThread, const uint32_t Timeout);

        virtual ListenerPtr
        CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self,
                       descriptor_t listenSocket) override;

        virtual DescriptorPtr CreateDescriptor(descriptor_t descriptor) override;

        virtual bool Enable(const DescriptorPtr descriptor) = 0;
        virtual void Disable(const DescriptorPtr descriptor) = 0;
    };
}

#endif
#endif //MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP
