#include "EntityMenu.h"



CEntityMenu::CEntityMenu(std::string textureLocation)//:CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}


CEntityMenu::~CEntityMenu()
{
}

void CEntityMenu::update()
{
	invunerable_ = true;
}
