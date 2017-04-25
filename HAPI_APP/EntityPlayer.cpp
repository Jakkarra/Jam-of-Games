#include "EntityPlayer.h"
#include "World.h"



CEntityPlayer::CEntityPlayer()//:CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\thing.png"));// we could make it so you pass in a value for the texture but we should know what textures are going to be used
	pos_ = Point{ 50,50 };
	health_ = 3;
	speed_ = 4;
	attack_ = 10;
	side = player;
	alive_ = true;


}

CEntityPlayer::~CEntityPlayer()
{
}
//delete sprite

void CEntityPlayer::initialiseValues() //feel like sprite data going to be deleted once ou
{
	

	
}


void CEntityPlayer::update(World& world)
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);

	if (health_ <= 0)
		alive_ = false;

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_)
	{
		pos_.y += speed_;

	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y]> deadzone_left_)
	{

		pos_.y -= speed_;

	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_)
	{

		pos_.x -= speed_;

	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzone_left_)
	{

		pos_.x += speed_;
	}
	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
	{
		shoot(world.getBullets().at(bulletNum));
		bulletNum++;

		if (bulletNum > 499)
			bulletNum = 0;
	}
	
	
}

void CEntityPlayer::shoot(CEntityBullet* bullet)
{
	if (HAPI_Sprites.GetTime() > reloadTime)
	{
		bullet->setValues(HAPI_TColour(0, 1, 0), angle_, pos_, side); //need to make the player rotate so i can try shooting at different angles. I need to calc bullet direction from player angle
		reloadTime = HAPI_Sprites.GetTime() + 200;
	}

}

void CEntityPlayer::hasCollided(CEntity* other)
{
	if (other->getSide() == enemy)
		health_ -= 1;

}



