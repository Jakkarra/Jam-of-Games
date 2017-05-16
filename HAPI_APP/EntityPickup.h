#pragma once
#include "Entity.h"
class CEntityPickup :
	public CEntity
{
public:
	CEntityPickup(int health, int speed,int rof, int attack);
	~CEntityPickup();
	void initialiseValues();
	void update(World& world);
	void render(Point playerPos);
	void hasCollided(CEntity& other);
private:
};

