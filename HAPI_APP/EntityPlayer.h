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


	int deadzone_left_ = HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData();
	Sprite* sprite_; //sprite needs to be created with one of the 4 constructors, needs a surface initally

protected:



};

