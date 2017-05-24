#include "EntityBruteEnemy.h"
#include "World.h"



void CEntityBruteEnemy::initialiseValues()
{
	health_ = 6;
	maxHealth_ = 6;
	alive_ = false;
	speed_ = 1;
	side = enemy;
	attack_ = 2;
	myclass = eBrute;
	side = enemy;
	max_range_ = 60;
	angle_ = 0;
	frameTime = HAPI_Sprites.GetTime();
}

void CEntityBruteEnemy::update(World& world)
{
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

		if (pos_ != playerpos_)
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
	}
}




CEntityBruteEnemy::CEntityBruteEnemy(std::string textureLocation)
{
	sprite_ = new Sprite("Data\\ogre.xml", "Data\\");

	initialiseValues();
}


CEntityBruteEnemy::~CEntityBruteEnemy()
{
}
