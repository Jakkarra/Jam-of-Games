#pragma once
#include "Entity.h"
#include "EntityBullet.h"

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntityPlayer :
	public CEntity
{
public:

	CEntityPlayer();
	~CEntityPlayer();
	void initialiseValues();
	void update(World& world);
	void shoot(CEntityBullet* bullet);
	void hasCollided(CEntity* other);

private:


	int deadzone_left_ = HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	int reloadTime = 0;
	int bulletNum= 0;
	
protected:



};

