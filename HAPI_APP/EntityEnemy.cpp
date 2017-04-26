#include "EntityEnemy.h"
#include "World.h"





EntityEnemy::EntityEnemy(std::string textureLocation)//:CEntity (textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface(textureLocation));

	initialiseValues();
}

EntityEnemy::~EntityEnemy()
{
}


void EntityEnemy::initialiseValues()
{
	health_ = 1;
	pos_ = { (rand()%800) + 200,(rand()%800) + 200 };
	alive_ = true;
	speed_ = 3;
	side = enemy;
}


void EntityEnemy::update(World& world)
{	
	playerpos_ =  world.getPlayerPos();

	if (health_ <= 0) {
		alive_ = false;
	}

	if (alive_ == true) {

		if (playerpos_.x > pos_.x) {
			//distance.x = playerpos_.x - pos_.x;

			pos_.x += speed_;


		}
		else  {
			//	distance.x = mypos_.x - playerpos_.x;

			//if (distance.x > 0) {
			pos_.x -= speed_;
			//}


		}

		if (playerpos_.y > pos_.y) {
			//distance.y = playerpos_.y - pos_.y;

			//if (distance.y > 0) {
			pos_.y += speed_;
			//}


		}
		else  {
			//	distance.y = mypos_.y - playerpos_.y;
			//	if (distance.y > 0) {
			pos_.y -= speed_;
			//	}


		}
	}
}

void EntityEnemy::hasCollided(CEntity* other)
{
	if (other->getSide() == player)
	{		
		health_ -= other->getAttack();
	}

}




