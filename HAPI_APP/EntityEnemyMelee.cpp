#include "EntityEnemyMelee.h"
#include "World.h"


void CEntityEnemyMelee::initialiseValues()
{
	health_ = 1;
	pos_ = { (rand() % 800) + 200,(rand() % 800) + 200 };
	alive_ = true;
	speed_ = 2;
	side = enemy;
	attack_ = 1;

	max_range_ = 60;
	angle_ = 0;
}

void CEntityEnemyMelee::update(World& world)
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
	}
}




CEntityEnemyMelee::CEntityEnemyMelee(std::string textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}

CEntityEnemyMelee::~CEntityEnemyMelee()
{
}
