#include "EntityPickup.h"



CEntityPickup::CEntityPickup()
{	
	
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data//powerUp.png"));// we could make it so you pass in a value for the texture but we should know what textures are going to be used
	
	initialiseValues();

}


CEntityPickup::~CEntityPickup()
{
}

void CEntityPickup::initialiseValues()
{
	health_ = rand() % 2;
	speed_ = rand() % 2;
	rof_ = rand() % 2;
	attack_ = rand() % 2;
	invunerable_ = true;
	side = pickup;
	alive_ = false;

}

void CEntityPickup::update(World & world)
{
	
	if (alive_ == true)
	{
		angle_ += 0.03;
	
		if (angle_ >= 6.28)
			angle_ = 0;
	}

}

void CEntityPickup::render(Point playerPos)
{
	if (alive_ == true)
	{
		sprite_->RenderRotated(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), angle_);
	}
		
}

void CEntityPickup::hasCollided(CEntity& other)
{
	if (other.getSide() == player)
	{
		alive_ = false;
	}
}
