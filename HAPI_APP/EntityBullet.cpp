#include "EntityBullet.h"



CEntityBullet::CEntityBullet()//CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\rocketUp.png"));
	initialiseValues();
	
}


CEntityBullet::~CEntityBullet()
{
}


void CEntityBullet::initialiseValues()
{
	health_ = 3;
	speed_ = 6;
	attack_ = 2;


}
void CEntityBullet::update(World& world)
{//list hre where bullet starts on different positions
	
	//movement(speed); Function for moving
	movement();
	//need to delete the bullet after a certain amount of time OR we just make it die when it hits a wall, which will always happen

	if (HAPI_Sprites.GetTime() > lifeDuration)
	{
		alive_ = false;
	}
}

void CEntityBullet::setValues(CEntity &other) //when gun is fired, give bullet stats
{						
	//need to get the angle and with that use it to make the bullet move in the wanted direction


	alive_ = true;
	side = other.getSide();
	pos_ = other.getPos();
	angle_ = other.getAngle();
	attack_ = other.getAttack();//instead of their attack maybe have a seprate stats for bullets fired like bullets speed not just player speed
	lifeDuration = HAPI_Sprites.GetTime() + 1500;
	//seperate render function so we can set the colour, maybe if statements in player and if damage is x high, add red colour. speed add yellow etc.
}

void CEntityBullet::resetValues()
{
	alive_ = false;
}