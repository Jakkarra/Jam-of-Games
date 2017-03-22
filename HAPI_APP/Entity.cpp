#include "Entity.h"



CEntity::CEntity()
{
	
}


CEntity::~CEntity()
{
}


void CEntity::initialiseValues()
{


}

void CEntity::render()
{
	sprite_->Render(SCREEN_SURFACE, pos_);
}



