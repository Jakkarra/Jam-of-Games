#include "EntityBullet.h"



CEntityBullet::CEntityBullet(std::string textureLocation)//:CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));
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
	//sides = side::player; enum for what team theyre on

}
void CEntityBullet::update()
{//list hre where bullet starts on different positions
	
	//movement(speed); Function for moving
	movement();
	//need to delete the bullet after a certain amount of time OR we just make it die when it hits a wall, which will always happen

	
}

void CEntityBullet::setValues(std::string bulletType, std::string facing, Point gunPos, int side) //when gun is fired, give bullet stats
{																										
	
	Point gunLocation{ 0,0 };
	alive_ = true;

	/*if (side == 0)
		sides = side::player; //check which team bullet is on
	else
		sides = side::enemy;*/
	

	//We will need to render diffeeently based on stats so we can have different coloured magic balls
	/*if (!HAPI.PlaySound("Audio\\" + bulletType + "Shoot.wav"))
		HAPI.DebugText("Error");*///play audio
	pos_ = gunPos;
}

void CEntityBullet::resetValues()
{
	alive_ = false;
}