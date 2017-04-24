#include "EntityEnemy.h"




EntityEnemy::EntityEnemy(std::string textureLocation)//:CEntity (textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}

EntityEnemy::~EntityEnemy()
{
}
void EntityEnemy::initialiseValues()
{
	health_ = 1;
	alive_ = true;
}
void EntityEnemy::update()
{
}

void EntityEnemy::render(Point pos, float angle)
{
	if(alive_ == true)
		sprite_->RenderRotated(SCREEN_SURFACE, pos, angle);
}

