#include "EntityPlayer.h"



CEntityPlayer::CEntityPlayer(std::string textureLocation)
{

	auto surface = HAPI_Sprites.MakeSurface(textureLocation);
	_sprite = new Sprite(surface);

}


CEntityPlayer::~CEntityPlayer()
{
	delete _sprite;
}

void CEntityPlayer::update()
{

}

void CEntityPlayer::render()
{
	_sprite->Render(SCREEN_SURFACE, pos);
}
