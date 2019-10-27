#pragma once

#include <map>
#include <memory>
#include <string>

namespace mw
{
template<typename ResourceType, typename IdentityType>
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	bool load(IdentityType id, const ::std::string& fileName);

	template<typename SecondType>
	bool load(IdentityType id, const ::std::string& fileName, const SecondType& secondParam);

	ResourceType* get(IdentityType id);
	const ResourceType* get(IdentityType id) const;

	void unload(IdentityType id);

	void unloadAll();

private:
	bool insertResource(IdentityType id, ::std::unique_ptr<ResourceType> resource);

private:
	::std::map<IdentityType, ::std::unique_ptr<ResourceType>> mResourceMap;
};
}

#include "ResourceManager.inl" 