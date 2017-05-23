#define _USE_MATH_DEFINES
#include "EntityEnemyBOSS.h"
#include "World.h"
#include <cmath>


void CEntityEnemyBOSS::initialiseValues()
{
	health_ = 40;
	pos_ = { (rand() % 800) + 200,(rand() % 800) + 200 };
	alive_ = false;
	speed_ = 0.5;
	side = enemy;
	attack_ = 2;
	myclass = eBoss;

	max_range_ = 240;
	min_range_ = 100;
	angle_ = 0;
}

void CEntityEnemyBOSS::update(World& world)
{
	playerpos_ = world.getPlayerPos();

	if (health_ <= 0) {
		alive_ = false;
	}
	if (alive_ == true) {

		if (angle_ >= 6.28)//6.28 is 360 degrees in radians
		{
			angle_ -= 6.28;
		}

		if (shooting_timer_ < 60.f)
		{
			float reverse_angle_ = angle_ - M_PI;

			BOSSshoot(world.getBullets().at(bulletNum), 0);
			bulletNum++;


			BOSSALTshoot(world.getBullets().at(bulletNum), reverse_angle_);
			bulletNum++;

			

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
	}
}

CEntityEnemyBOSS::CEntityEnemyBOSS(std::string textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}

CEntityEnemyBOSS::~CEntityEnemyBOSS()
{
}
