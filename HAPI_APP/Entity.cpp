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

void CEntity::render(Point playerPos)
{	//i want to interp all but not sure how to 
	if (alive_ == true)
	{
		sprite_->RenderRotated(SCREEN_SURFACE, pos_ - (playerPos - Point(960,540)), angle_);
	}
	
}



