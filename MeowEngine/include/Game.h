#pragma once
namespace mw
{
	
	class Game
	{
	public:
		static Game* create();
		virtual void run() = 0;
	#if 0
		Game();
		~Game();
	
	
	private:
		virtual void registerStates();
	
		virtual void handleInputs();
	
		virtual void update(float deltaSeconds);
	
		virtual void render();
	
	private:
		::sf::RenderWindow mWindow;
		PlayerController mPlayerController;
	
		StateStack mStateStack;
	
		TextureManager mTextureManager;
		FontManager mFontManager;
		MapManager mMapManager;
	
	
		::sf::Text mStatsText;
		FPSMeter mFPSMeter;
	
		bool mbPaused;
	#endif
	};
	
}
