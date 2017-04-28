#include "EntityPlayer.h"
#include "World.h"



CEntityPlayer::CEntityPlayer()//:CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\thing.png"));// we could make it so you pass in a value for the texture but we should know what textures are going to be used
	initialiseValues();


}

CEntityPlayer::~CEntityPlayer()
{
}
//delete sprite

void CEntityPlayer::initialiseValues() //feel like sprite data going to be deleted once ou
{
	
	pos_ = Point{ 50,50 };
	health_ = 9;
	speed_ = 4;
	attack_ = 10;
	side = player;
	alive_ = true;
	angle_ = 0;
	
}


void CEntityPlayer::update(World& world)
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);

	if (health_ <= 0)
		alive_ = false;

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_ )
	{
		pos_.y += speed_;

	}

	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y] < -deadzone_left_)
	{
		angle_ += 0.1;// just test code, i think we should go with 8 directional shooting and also limit enemy to that

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
	
	if (HAPI_Sprites.GetTime() > invunerableTime)
		invunerable_ = false;

	if (angle_ >= 6.28)//6.28 is 360 degrees in radians
	{
		angle_ -= 6.28;
	}
	
}

void CEntityPlayer::shoot(CEntityBullet* bullet)
{
	if (HAPI_Sprites.GetTime() > reloadTime)
	{
		bullet->setValues(*this); //need to make the player rotate so i can try shooting at different angles. I need to calc bullet direction from player angle
		reloadTime = HAPI_Sprites.GetTime() + 1500;
	}

}

void CEntityPlayer::hasCollided(CEntity &other)
{
	if (other.getSide() == enemy)
	{
		health_ -= other.getAttack();
		invunerable_ = true;
		invunerableTime = HAPI_Sprites.GetTime() + 200;
	}

}



