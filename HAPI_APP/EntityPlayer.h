#pragma once
#include "Entity.h"
// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntityPlayer :
	public CEntity
{
public:
	CEntityPlayer(std::string textureLocation);
	~CEntityPlayer();
	void update();
	void render(Point pos, float angle);

private:
	
	Sprite* _sprite; //sprite needs to be created with one of the 4 constructors, needs a surface initally

	//need ask keith about how to do this as you cant create a blank sprite and add information on later, needs to be done all at once and that cant be done anywhere in a class
};

