#include "Entity.h"



CEntity::CEntity()
{
	
}


CEntity::~CEntity()
{
}

void CEntity::render()
{
	_sprite->Render(SCREEN_SURFACE, pos);
}


