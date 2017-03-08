#include "EntityPlayer.h"



CEntityPlayer::CEntityPlayer(std::string textureLocation)
{

	auto surface = HAPI_Sprites.MakeSurface(textureLocation);
	Sprite sprite(surface);
	sprite_ = &sprite;
	health_ = 3;
	speed_ = 1;
	attack_ = 10;
}


CEntityPlayer::~CEntityPlayer()
{
}
//delete sprite
void CEntityPlayer::update()
{
	if (conData.digitalButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_)
	{
		pos_.y - speed_;

	}

	if (conData.digitalButtons[HK_ANALOGUE_LEFT_THUMB_Y]> deadzone_left_)
	{

		pos_.y + speed_;

	}

	if (conData.digitalButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_)
	{

		pos_.x - speed_;

	}

	if (conData.digitalButtons[HK_ANALOGUE_LEFT_THUMB_X]> deadzone_left_)
	{

		pos_.x + speed_;

	}
}

void CEntityPlayer::render(Point pos, float angle)
{
	sprite_->RenderRotated(SCREEN_SURFACE, pos, angle);
}



