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
	if (other.getSide() != side)
	{
		health_ -= other.getAttack();
	}
}

void CEntity::initialiseValues()
{


}

void CEntity::render(Point playerPos)
{	//i want to interp all but not sure how to 
	if (alive_ == true)
	{
		sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960,540)), _frameNum);
		if (_frameNum >= frameOffset + numerOfFramesForAnimation)
			_frameNum = frameOffset;
	}
	
}



