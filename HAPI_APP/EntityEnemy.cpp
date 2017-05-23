#include "EntityEnemy.h"
#include "World.h"





EntityEnemy::EntityEnemy(std::string textureLocation)//:CEntity (textureLocation)
{
	sprite_ = new Sprite("Data\\goblin.xml", "Data\\");
	initialiseValues();
}

EntityEnemy::~EntityEnemy()
{
}


void EntityEnemy::initialiseValues()
{
	health_ = 10;
	maxHealth_ = health_;
	pos_ = { (rand()%800) + 200,(rand()%800) + 200 };
	alive_ = true;
	speed_ = 3;
	side = enemy;
	attack_ = 1;

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
			eframeOffset = 22;
			enumberOfFramesForAnimation = 3;

		}
		else  {
			//	distance.x = mypos_.x - playerpos_.x;

			//if (distance.x > 0) {
			pos_.x -= speed_;
			eframeOffset = 1;
			enumberOfFramesForAnimation = 3;
			//}


		}

		if (playerpos_.y > pos_.y) {
			//distance.y = playerpos_.y - pos_.y;

			//if (distance.y > 0) {
			pos_.y += speed_;
			eframeOffset = 32;
			enumberOfFramesForAnimation = 3;
			//}


		}
		else  {
			//	distance.y = mypos_.y - playerpos_.y;
			//	if (distance.y > 0) {
			pos_.y -= speed_;
			eframeOffset = 11;
			enumberOfFramesForAnimation = 3;
			//	}


		}
	}

	oldPos = pos_;
	interpValue = 0;
}

void EntityEnemy::hasCollided(CEntity &other)
{
	if (other.getSide() == player)
	{		
		health_ -= other.getAttack();

		if (health_ <= 0) {
			alive_ = false;
		}

	}
	
}

void EntityEnemy::render(Point playerPos)
{	//i want to interp all but not sure how to 
	if (alive_ == true)
	{
		int redMult =1+( maxHealth_ - health_);
		sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), _eframeNum);
			if (eframeTime < HAPI_Sprites.GetTime())
			{
				if (_eframeNum >= (eframeOffset + enumberOfFramesForAnimation))
				{
					_eframeNum = eframeOffset;
				}
				else if (_eframeNum < eframeOffset)
				{
					_eframeNum = eframeOffset;
				}
				else
				{
					_eframeNum++;
				}
				eframeTime += 100;
			}
		}
}





