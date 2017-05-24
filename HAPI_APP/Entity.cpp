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
	if (other.getSide() != side && other.isInvunerable() == false)
	{
		health_ -= other.getAttack();
	}
	else if (other.getSide() == side)
	{
		pos_ = oldPos;
	}
}

void CEntity::initialiseValues()
{


}

void CEntity::render(Point playerPos)
{	//i want to interp all but not sure how to 
	if (alive_ == true)
	{
		int redMult = 1 + (maxHealth_ - health_);

		if (side == enemy)
		{

			sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), [&](const Point p, HAPI_TColour& dest, const HAPI_TColour& source) {
				if (source.alpha == 255)
				{
					dest.red = source.red;//attempt at making enemies go more red if damaged, maybe not make it a scale just if elss than 2 health etc
					dest.green = source.green / redMult;
					dest.blue = source.blue / redMult;
				}
				else if (source.alpha > 0)
				{

					dest.red = dest.red + source.alpha*((source.red - dest.red)) >> 8;
					dest.green = dest.green + source.alpha*((source.green - dest.green) / redMult) >> 8;
					dest.blue = dest.blue + source.alpha*((source.blue - dest.blue) / redMult) >> 8;
				}
			}, _frameNum);
	
		}
		else 
			sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), _frameNum);
			

		if (HAPI_Sprites.GetTime() > frameTime)
		{
			if (_frameNum >= frameOffset + numberOfFramesForAnimation)
				_frameNum = frameOffset;
			else if (_frameNum < frameOffset)
				_frameNum = frameOffset;
			else
				_frameNum++;
			frameTime += 100;
		}
	}
	
}



