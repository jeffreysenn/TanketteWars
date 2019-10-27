#pragma once

namespace server
{

struct Context
{
public:
	static Context& getInstance()
	{
		static Context instance;
		return instance;
	}

private:
	Context() {}

public:
	Context(Context const&) = delete;
	void operator=(Context const&) = delete;

public:
	class ServerStateStack* stack{};
	class NetworkManager* networkManager{};
};

}