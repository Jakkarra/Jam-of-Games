#define _USE_MATH_DEFINES
#include "EntityEnemyBOSS.h"
#include "World.h"
#include <cmath>


void CEntityEnemyBOSS::initialiseValues()
{
	health_ = 40;
	maxHealth_ = 40;
	speed_ = 4;
	side = enemy;
	attack_ = 1;
	myclass = eBoss;
	max_range_ = 240;
	min_range_ = 100;
	angle_ = 0;
	frameTime = HAPI_Sprites.GetTime();
	shootOffset = Point(50, 50);
}

void CEntityEnemyBOSS::update(World& world)
{
	playerpos_ = world.getPlayerPos();

	if (health_ <= 0) {
		world.bossDeath();
		alive_ = false;
	}
	if (alive_ == true) {

		Point line_to_player;
		line_to_player.x = -(pos_.x - playerpos_.x);
		line_to_player.y = -(pos_.y - playerpos_.y);
		float angle_to_player = atan2f(line_to_player.y, line_to_player.x);

		if (angle_ >= 6.28)//6.28 is 360 degrees in radians
		{
			angle_ -= 6.28;
		}

		if (shooting_timer_ < 80.f)
		{
			float reverse_angle_ = angle_ - M_PI;

			BOSSshoot(world.getBullets().at(bulletNum), 0);
			bulletNum++;
			float tempAngle = angle_;
			angle_ = reverse_angle_;

			BOSSALTshoot(world.getBullets().at(bulletNum), reverse_angle_);
			bulletNum++;
			angle_ = tempAngle;
			

			if (bulletNum > 499)
			{
				bulletNum = 0;
			}

			shooting_timer_ ++ ;
		}
		else if (shooting_timer_ >= 120.f)
		{
			shooting_timer_ = 0;
		}
		else
		{
			shooting_timer_++;
		}

		angle_ += 0.1;

		oldPos = pos_;
		interpValue = 0;

		if (angle_to_player > -0.80 && angle_to_player <= 0.80)    //right                            //6.28 is 360 degrees in radians
		{
			frameOffset = 10;
		}
		else if (angle_to_player > -2.4 && angle_to_player <= -0.8)                //up                    
		{

			frameOffset = 5;
		}
		else if (angle_to_player > 0.8 && angle_to_player <= 2.4)    //down                                
		{
			frameOffset = 15;
		}
		else     //left                            
		{
			frameOffset = 0;
		}
		numberOfFramesForAnimation = 3;
	}
}

CEntityEnemyBOSS::CEntityEnemyBOSS(std::string textureLocation)
{
	sprite_ = new Sprite("Data\\boss.xml", "Data\\");

	initialiseValues();
}

CEntityEnemyBOSS::~CEntityEnemyBOSS()
{
}
