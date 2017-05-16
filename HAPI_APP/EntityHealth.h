#pragma once
#include <vector>
#include "Entity.h"
#include "World.h"
class EntityHealth : public CEntity
{
public:
	EntityHealth();
	~EntityHealth();

	void initialiseValues();
	void update(World& world);

	void render(Point playerPos);

private:
	int maxHealth = 8;
	int playerHealth = 0;
	bool is2 = true;
	bool is1 = false;
	bool is0 = false;
	Sprite* spriteHalf_{ nullptr };
	Sprite* spriteEmpty_{ nullptr };

	int offsetHeart = 0;

};

