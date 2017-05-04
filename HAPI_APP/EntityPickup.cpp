#include "EntityPickup.h"



CEntityPickup::CEntityPickup(int health, int speed, int rof, int attack)
{	
	health_ = health;
	speed_ = speed;
	rof_ = rof;
	attack_ = attack;

	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\thing.png"));// we could make it so you pass in a value for the texture but we should know what textures are going to be used
	
	initialiseValues();

}


CEntityPickup::~CEntityPickup()
{
}

void CEntityPickup::initialiseValues()
{
	invunerable_ = true;
	side = pickup;
	alive_ = true;
	pos_ =  { (rand()%800) + 200,(rand()%800) + 200 };

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

void CEntityPickup::render()
{
	if (alive_ == true)
	{
		int health = health_;
		int speed = speed_;
		int damage = attack_;

		sprite_->RenderRotated(SCREEN_SURFACE, pos_, angle_, [&](const Point p, HAPI_TColour& dest, const HAPI_TColour& source)
		{
			dest.red = source.alpha*(255 / (1 + speed + damage)) >> 8;
			dest.green = source.alpha * (255 / (1 + health + speed)) >> 8;
			dest.blue = source.alpha * (255 / (1 + speed + health)) >> 8;
		});
	}
}

void CEntityPickup::hasCollided(CEntity & other)
{
	if (other.getSide() == player)
	{
		alive_ = false;
	}
}
