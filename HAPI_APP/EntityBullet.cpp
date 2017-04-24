#include "EntityBullet.h"



CEntityBullet::CEntityBullet()//CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\fireBall.png"));
	initialiseValues();
	
}


CEntityBullet::~CEntityBullet()
{
}


void CEntityBullet::initialiseValues()
{
	health_ = 3;
	speed_ = 1;
	attack_ = 10;


}
void CEntityBullet::update(World& world)
{//list hre where bullet starts on different positions
	
	//movement(speed); Function for moving
	movement();
	//need to delete the bullet after a certain amount of time OR we just make it die when it hits a wall, which will always happen

	
}

void CEntityBullet::setValues(HAPI_TColour colour, float angle, Point gunPos, sides team) //when gun is fired, give bullet stats
{						
	//need to get the angle and with that use it to make the bullet move in the wanted direction


	alive_ = true;
	side = team;
	pos_ = gunPos;
	angle_ = angle;
	//seperate render function so we can set the colour, maybe if statements in player and if damage is x high, add red colour. speed add yellow etc.
}

void CEntityBullet::resetValues()
{
	alive_ = false;
}