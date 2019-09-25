#include "pch.h"
#include "LinkManager.h"
#include "Link.h"

using namespace Util;
namespace Network
{
	TL::ObjectPool<Link>                   linkPool;
	std::map<Link*, std::shared_ptr<Link>> linkMap;

	std::shared_ptr<Link> LinkManager::make_shared()
	{
		auto Link = linkPool.make_shared();
		linkMap[Link.get()] = Link;

		return Link;
	}

	void LinkManager::removesession(const std::shared_ptr<Link>& link)
	{
		auto iter = linkMap.find(link.get());

		if (iter == linkMap.end())
			return;

		linkMap.erase(iter);
	}

	size_t LinkManager::usersize()
	{
		return linkMap.size();
	}

	std::map<Link*, std::shared_ptr<Link>>::iterator LinkManager::begin()
	{
		return linkMap.begin();
	}

	std::map<Link*, std::shared_ptr<Link>>::iterator LinkManager::end()
	{
		return linkMap.end();
	}
}
