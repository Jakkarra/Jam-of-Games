#include "EntityEnemy.h"




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
	alive_ = true;
}
void EntityEnemy::update()
{	

	if (health_ >= 0) {
		alive_ = false;
	}

	if (alive_ == true) {

		if (playerpos_.x > mypos_.x) {
			//distance.x = playerpos_.x - pos_.x;

			mypos_.x += speed_;


		}
		else if (playerpos_.x < mypos_.x) {
			//	distance.x = mypos_.x - playerpos_.x;

			//if (distance.x > 0) {
			mypos_.x -= speed_;
			//}


		}

		if (playerpos_.y > mypos_.y) {
			//distance.y = playerpos_.y - pos_.y;

			//if (distance.y > 0) {
			mypos_.y += speed_;
			//}


		}
		else if (playerpos_.y < mypos_.y) {
			//	distance.y = mypos_.y - playerpos_.y;


			//	if (distance.y > 0) {
			mypos_.y -= speed_;
			//	}


		}
	}
}

void EntityEnemy::render(Point pos, float angle)
{

	if (alive_ = true) {
	sprite_->RenderRotated(SCREEN_SURFACE, pos, angle);
	}
}

void EntityEnemy::getplayerpos(Point playerpos)
{
	playerpos_ = playerpos;
}
