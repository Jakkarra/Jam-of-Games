#include "Entity.h"



CEntity::CEntity(std::string textureLocation)
{
	auto surface = HAPI_Sprites.MakeSurface(textureLocation);
	_sprite = new Sprite(surface);
}


CEntity::~CEntity()
{
}


void CEntity::initialise()
{


}

void CEntity::render()
{
	sprite_->Render(SCREEN_SURFACE, pos_);
}


