#include <stdexcept>
#include <cassert>
#include <iostream>
#include "ResourceManager.h"

template<typename ResourceType, typename IdentityType>
ResourceManager<ResourceType, IdentityType>::ResourceManager()
{
}

template<typename ResourceType, typename IdentityType>
ResourceManager<ResourceType, IdentityType>::~ResourceManager()
{
}

template<typename ResourceType, typename IdentityType>
bool ResourceManager<ResourceType, IdentityType>::load(IdentityType id, const ::std::string &fileName)
{
	// In case of multiple loading
	if (mResourceMap.find(id) != mResourceMap.end())
		return false;

	::std::unique_ptr<ResourceType> resource = ::std::make_unique<ResourceType>();
	if (!resource->loadFromFile(fileName))
	{
		throw ::std::runtime_error("ResourceManager::load - Failed to load " + fileName);
		return false;
	}

	return insertResource(id, ::std::move(resource));
}

template<typename ResourceType, typename IdentityType>
template<typename SecondType>
bool ResourceManager<ResourceType, IdentityType>::load(IdentityType id, const ::std::string & fileName, const SecondType & secondParam)
{
	// In case of multiple loading
	if (mResourceMap.find(id) != mResourceMap.end())
		return false;

	::std::unique_ptr<ResourceType> resource = ::std::make_unique<ResourceType>();
	if (!resource->loadFromFile(fileName, secondParam))
	{
		throw ::std::runtime_error("ResourceManager::load - Failed to load " + fileName);
		return false;
	}

	return insertResource(id, ::std::move(resource));
}

template<typename ResourceType, typename IdentityType>
ResourceType* ResourceManager<ResourceType, IdentityType>::get(IdentityType id)
{
	auto found = mResourceMap.find(id);
	if (found == mResourceMap.end())
	{
		::std::cout << "ResourceManager::get - Failed to get!" << ::std::endl;
		return nullptr;
	}
	return found->second.get();
}

template<typename ResourceType, typename IdentityType>
const ResourceType* ResourceManager<ResourceType, IdentityType>::get(IdentityType id) const
{
	auto found = mResourceMap.find(id);
	if (found == mResourceMap.end())
	{
		::std::cout << "ResourceManager::get - Failed to get!" << ::std::endl;
		return nullptr;
	}
	return found->second.get();
}

template<typename ResourceType, typename IdentityType>
void ResourceManager<ResourceType, IdentityType>::unload(IdentityType id)
{
	auto found = mResourceMap.find(id);
	if (found == mResourceMap.end())
		return;

	mResourceMap.erase(found);
}

template<typename ResourceType, typename IdentityType>
void ResourceManager<ResourceType, IdentityType>::unloadAll()
{
	mResourceMap.clear();
}

template<typename ResourceType, typename IdentityType>
bool ResourceManager<ResourceType, IdentityType>::insertResource(IdentityType id, ::std::unique_ptr<ResourceType> resource)
{
	auto inserted = mResourceMap.insert(::std::make_pair(id, ::std::move(resource)));
	assert(inserted.second);
	return inserted.second;
}