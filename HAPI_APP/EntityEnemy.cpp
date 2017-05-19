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
		sprite_->RenderRotated(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), angle_);//, [&](const Point p, HAPI_TColour& dest, const HAPI_TColour& source)
		//{
		//	if (source.alpha == 255)
		//	{
		//		dest.red = source.red;//attempt at making enemies go more red if damaged, maybe not make it a scale just if elss than 2 health etc
		//		dest.green = source.green;
		//		dest.blue = source.blue;
		//	}
		//	else if(source.alpha > 0)
		//	{
		//		
		//		dest.red	= /*dest.red + source.alpha	**/	((source.red  - dest.red)) >> 8;
		//		dest.green	= ((source.green- dest.green)/redMult) >> 8;
		//		dest.blue	= 	((source.blue - dest.blue)/redMult) >> 8;
		//	}
		//});
	}

}





