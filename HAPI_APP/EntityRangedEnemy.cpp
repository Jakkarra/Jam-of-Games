#include "EntityRangedEnemy.h"
#include "World.h"


void CEntityRangedEnemy::initialiseValues()
{
	health_ = 1;
	pos_ = { (rand() % 800) + 200,(rand() % 800) + 200 };
	alive_ = false;
	speed_ = 3;
	side = enemy;
	attack_ = 1;
	myclass = eRanged;

	max_range_ = 60;
	min_range_ = 30;
	angle_ = 0;
}

void CEntityRangedEnemy::update(World& world)
{
	playerpos_ = world.getPlayerPos();

	if (health_ <= 0) {
		alive_ = false;
	}
	if (alive_ == true) {
		//working out the angle to the player
		Point line_to_player;
		line_to_player.x = -(pos_.x - playerpos_.x);
		line_to_player.y = -(pos_.y - playerpos_.y);
		angle_ = atan2f(line_to_player.y, line_to_player.x);

		if (angle_ >= 6.28)//6.28 is 360 degrees in radians
		{
			angle_ -= 6.28;
		}

		distance_.X = playerpos_.x - pos_.x;
		distance_.Y = playerpos_.y - pos_.y;

		totaldistance = distance_.X*distance_.X + distance_.Y*distance_.Y;

		if (totaldistance > max_range_ * max_range_)
		{

			if (playerpos_.x > pos_.x)
			{
				pos_.x += speed_;
			}
			else
			{
				pos_.x -= speed_;
			}

			if (playerpos_.y > pos_.y)
			{

				pos_.y += speed_;
			}
			else
			{
				pos_.y -= speed_;
			}
		}

		//For Shooter Enemies only!
		else if (totaldistance < min_range_ * min_range_)
		{

			if (playerpos_.x < pos_.x)
			{
				pos_.x += speed_;
			}
			else
			{
				pos_.x -= speed_;
			}

			if (playerpos_.y < pos_.y)
			{

				pos_.y += speed_;
			}
			else
			{
				pos_.y -= speed_;
			}

		}

		if (totaldistance < max_range_ * max_range_)
		{
			NPCshoot(world.getBullets().at(bulletNum));
			bulletNum++;

			if (bulletNum > 499)
			{
				bulletNum = 0;
			}
		}

		oldPos = pos_;
		interpValue = 0;
	}
}

CEntityRangedEnemy::CEntityRangedEnemy(std::string textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}

CEntityRangedEnemy::~CEntityRangedEnemy()
{
}
