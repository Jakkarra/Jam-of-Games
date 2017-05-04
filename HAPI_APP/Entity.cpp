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
{	//i want to interp all but not sure how to 
	if (alive_ == true)
	{

		sprite_->RenderRotated(SCREEN_SURFACE, pos_, angle_);
	}
	
}



