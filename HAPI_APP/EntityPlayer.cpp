#include "EntityPlayer.h"



CEntityPlayer::CEntityPlayer(std::string textureLocation)//:CEntity(textureLocation)
{

	sprite_ =  new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}

CEntityPlayer::~CEntityPlayer()
{
}
//delete sprite

void CEntityPlayer::initialiseValues() //feel like sprite data going to be deleted once ou
{
	pos_ = Point{ 50,50 };
	health_ = 3;
	speed_ = 1;
	attack_ = 10;
}


void CEntityPlayer::update()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);


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

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X]> deadzone_left_)
	{

		pos_.x += speed_;

	}
}

//void CEntityPlayer::render()
//{
//	sprite_->Render(SCREEN_SURFACE, pos_);
//	/*Sprite newSprite = *sprite_;
//	newSprite.Render(SCREEN_SURFACE, pos_);*/
//}




