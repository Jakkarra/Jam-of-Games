#include "EntityRangedEnemy.h"
#include "World.h"


void CEntityRangedEnemy::initialiseValues()
{
	health_ = 3;
	maxHealth_ = 3;
	alive_ = false;
	speed_ = 2;
	side = enemy;
	attack_ = 1;
	myclass = eRanged;
	side = enemy;

	max_range_ = 300;
	min_range_ = 150;
	angle_ = 0;
	frameTime = HAPI_Sprites.GetTime();
}

void CEntityRangedEnemy::update(World& world)
{
	invunerable_ = false;
	playerpos_ = world.getPlayerPos();
	oldPos = pos_;

	if (health_ <= 0) {
		alive_ = false;
		world.PowerUp(pos_);
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

		if (angle_ > -0.80 && angle_ <= 0.80)    //right                            //6.28 is 360 degrees in radians
		{
			frameOffset = 22;
		}
		else if (angle_ > -2.4 && angle_ <= -0.8)                //up                    
		{
			
			frameOffset = 11;


		}
		else if (angle_ > 0.8 && angle_ <= 2.4)    //down                                
		{
			frameOffset = 32;

		}
		else     //left                            
		{
			frameOffset = 1;
		}
		numberOfFramesForAnimation = 3;

		if (totaldistance < max_range_ * max_range_)
		{
			NPCshoot(world.getBullets().at(bulletNum));
			bulletNum++;

			if (bulletNum > 499)
			{
				bulletNum = 0;
			}
		}

		interpValue = 0;
	}
}

CEntityRangedEnemy::CEntityRangedEnemy(std::string textureLocation)
{
	sprite_ = new Sprite("Data\\goblin.xml", "Data\\");

	initialiseValues();
}

CEntityRangedEnemy::~CEntityRangedEnemy()
{
}
