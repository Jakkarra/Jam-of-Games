#include "EntityBullet.h"



CEntityBullet::CEntityBullet()//CEntity(textureLocation)
{
	
	initialiseValues();
	
}


CEntityBullet::~CEntityBullet()
{
}


void CEntityBullet::initialiseValues()
{
	health_ = 3;
	health_ = 1;
	speed_ = 170;
	attack_ = 2;


}
void CEntityBullet::update(World& world)
{//list hre where bullet starts on different positions
	

	
	//need to delete the bullet after a certain amount of time OR we just make it die when it hits a wall, which will always happen

	if (HAPI_Sprites.GetTime() > lifeDuration)
	{
		alive_ = false;
	}

	xVector = cosf(angle_);
	VectorY = sinf(angle_);

	newPos.x = oldPos.x + (xVector*speed_);
	newPos.y = oldPos.y + (VectorY*speed_);
	

	oldPos = pos_;
	interpValue = 0;
}

void CEntityBullet::setValues(CEntity &other, int weapon, Point offset) //when gun is fired, give bullet stats
{						
	//need to get the angle and with that use it to make the bullet move in the wanted direction

	interpValue = 0;
	
	invunerable_ = false; //testing
	
	if (weapon == 1)
	{
		sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\Fireball2.png"));
		HAPI_Sprites.PlaySound("Data\\Fireball.wav");
	}
	else 
	{
		sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\Arrow.png")); 
		HAPI_Sprites.PlaySound("Data\\Arrow_Shot.wav");
	}
	
	shootOffset = offset;
	alive_ = true;
	speed_ = 10 * other.getSpeed();
	side = other.getSide();
	pos_ = other.getPos();
	oldPos = other.getPos();
	angle_ = other.getAngle(); // i can just do a check to see if angle is between an 8-directional area and then send bullet that direction
	attack_ = other.getAttack();//instead of their attack maybe have a seprate stats for bullets fired like bullets speed not just player speed
	lifeDuration = HAPI_Sprites.GetTime() + 2500;

	//seperate render function so we can set the colour, maybe if statements in player and if damage is x high, add red colour. speed add yellow etc.
}

void CEntityBullet::resetValues()
{
	alive_ = false;
}


void CEntityBullet::render(Point playerPos)
{	//i want to interp all but not sure how to 
	if (alive_ == true)
	{
		if (interpValue > 1.f)
			interpValue = 1.f;
		else
			interpValue += 0.1f;

		pos_.x = oldPos.x + (newPos.x - oldPos.x)*interpValue;
		pos_.y = oldPos.y + (newPos.y - oldPos.y)*interpValue;

		//interpolation testing, not sure how to do it as i do not know the bullets end location cus it is always moving
		

		sprite_->RenderRotated(SCREEN_SURFACE, pos_  + shootOffset - (playerPos - Point(960, 540)), angle_);
	}
}

	void CEntityBullet::hasCollided(CEntity &other)
	{
		if (other.getSide() != side)
		{
			alive_ = false;
		}
	}

	void CEntityBullet::setangle(float angle)
	{
		angle_ = angle;
	}



