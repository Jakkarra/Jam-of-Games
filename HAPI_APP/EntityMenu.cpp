#include "EntityMenu.h"



CEntityMenu::CEntityMenu(std::string textureLocation)//:CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}


CEntityMenu::~CEntityMenu()
{
}

void CEntityMenu::update(World &world)
{
	invunerable_ = true;

}

void CEntityMenu::initialiseValues()
{
	alive_ = true;
}

