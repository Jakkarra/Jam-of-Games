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
	void render();
	void hasCollided(CEntity& other);
private:
};

