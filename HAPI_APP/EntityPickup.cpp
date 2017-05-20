#include "EntityPickup.h"



CEntityPickup::CEntityPickup(int health, int speed, int rof, int attack)
{	
	health_ = health;
	speed_ = speed;
	rof_ = rof;
	attack_ = attack;

	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\tranImage.png"));// we could make it so you pass in a value for the texture but we should know what textures are going to be used
	
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
	pos_ =  { (rand()%800) + 400,(rand()%800) + 400 };

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


		sprite_->RenderRotated(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), angle_, [&](const Point p, HAPI_TColour& dest, const HAPI_TColour& source)
		{
			if (source.alpha == 255)
			{
				dest.red = (255 / (1 + speed_ + attack_));
				dest.green = (255 / (1 + health_ + attack_));
				dest.blue = (255 / (1 + health_ + attack_));
			}
			else if (source.alpha > 0)
			{

				dest.red =	 source.alpha*((255 / (1 + speed_ + attack_)) - dest.red) >> 8;//not working needs fix!
				dest.green = source.alpha*((255 / (1 + health_ + attack_)) - dest.green) >> 8;
				dest.blue =  source.alpha*((255 / (1 + health_ + attack_)) - dest.blue) >> 8;
			}
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
