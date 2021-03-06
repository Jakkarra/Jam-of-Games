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
	void initialiseValues(int health, int speed, int rof, int damage, int weapon);
	void initialiseValues();
	void update(World& world);
	void shoot(CEntityBullet* bullet);
	void hasCollided(CEntity &other);

	void render(Point playerPos);
	int getMaxHealth() { return maxHealth_; }
	void setOutOfBounds(bool value) { outOfBounds = value; }
	bool getOutOfBounds() { return outOfBounds;}
private:
	int deadzone_left_ = HK_GAMEPAD_LEFT_THUMB_DEADZONE; //numberofKeys
	float timeToShoot = 0.f;
	float reloadTime = 500.f;
	unsigned int bulletNum= 0;
	float invunerableTime = 0;
	int maxHealth_ = 8;
	float currAngle = 0;
	bool outOfBounds = false;
	int currentWeapon;
	Sprite* weaponSprite;
	bool hasMoved = false;

};

