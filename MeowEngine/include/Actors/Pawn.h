#pragma once
#include "SpriteActor.h"
namespace mw
{
class Pawn : public SpriteActor
{
public:
	Pawn() {}
	Pawn(int HP, const ::sf::Texture& texture, Rendering::Layer layer = Rendering::Default)
		: mHP(HP)
		, SpriteActor(texture, layer)
	{
	}

	void setHP(int HP) { mHP = HP; }
	void changeHP(int deltaHP) { (mHP + deltaHP < 0) ? mHP = 0 : mHP += deltaHP; }
	int getHP() { return mHP; }
	bool isDestroyed() { return mHP == 0; }

private:
	int mHP = 1;
};
}