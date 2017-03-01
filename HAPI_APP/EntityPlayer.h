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
	void render();

private:
	

};

