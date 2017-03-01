#include "EntityPlayer.h"



CEntityPlayer::CEntityPlayer(std::string textureLocation)
{

	auto surface = HAPI_Sprites.MakeSurface(textureLocation);
	Sprite sprite(surface);
	_sprite = &sprite;

}


CEntityPlayer::~CEntityPlayer()
{
}

void CEntityPlayer::update()
{

}

void CEntityPlayer::render(Point pos, float angle)
{
	//sprite.RenderRotated(SCREEN_SURFACE, pos, angle);
}
