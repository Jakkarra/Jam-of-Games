#include "Entity.h"



CEntity::CEntity()
{

}


CEntity::~CEntity()
{
	delete sprite_;

}




void CEntity::hasCollided(CEntity &other)
{
}

void CEntity::initialiseValues()
{


}

void CEntity::render()
{
	if (alive_ == true)
	{
		sprite_->RenderRotated(SCREEN_SURFACE, pos_, angle_);
	}
	
}



