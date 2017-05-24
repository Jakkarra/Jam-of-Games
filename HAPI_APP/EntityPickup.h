#pragma once
#include "Entity.h"
class CEntityPickup :
	public CEntity
{
public:
	CEntityPickup();
	~CEntityPickup();
	void initialiseValues();
	void update(World& world);
	void render(Point playerPos);
	void hasCollided(CEntity& other);
private:
};

