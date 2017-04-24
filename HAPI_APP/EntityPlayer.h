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
	void initialiseValues();
	void update();
	bool isAlive() { return alive_; }
	

private:


	int deadzone_left_ = HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	
protected:



};

