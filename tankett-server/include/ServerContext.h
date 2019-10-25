#pragma once


struct ServerContext
{
public:
	static ServerContext& getInstance()
	{
		static ServerContext instance;
		return instance;
	}

private:
	ServerContext() {}

public:
	ServerContext(ServerContext const&) = delete;
	void operator=(ServerContext const&) = delete;

public:
	class ServerStateStack* stack{};
	class ServerNetworkManager* networkManager{};


#if 0 
	class ClientStateStack* stack{};
	class sf::RenderWindow* window{};
	TextureManager* textureManager{};
	FontManager* fontManager{};
	MapManager* mapManager{};
	class PlayerController* playerController{};
	class ClientNetworkManager* networkManager{};
#endif
};
