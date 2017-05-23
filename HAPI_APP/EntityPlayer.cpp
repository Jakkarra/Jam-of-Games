#include "EntityPlayer.h"
#include "World.h"



CEntityPlayer::CEntityPlayer()//:CEntity(textureLocation)
{
	sprite_ = new Sprite("Data\\player.xml", "Data\\");// we could make it so you pass in a value for the texture but we should know what textures are going to be used
	initialiseValues();


}

CEntityPlayer::~CEntityPlayer()
{
}
//delete sprite

void CEntityPlayer::initialiseValues(int health, int speed, int rof, int damage) 
{
	
	pos_ = Point{ 960,540 };
	health_ = health;
	maxHealth_ = health;
	speed_ = speed;
	rof_ = rof;
	reloadTime = 500 / rof;
	attack_ = damage;
	side = player;
	alive_ = true;
	angle_ = 0;
	//if (currentWeapon == staff)
	//	weaponSprite = new Sprite(HAPI_Sprites.MakeSurface("Data\\staff.png"));
	//else
	//	weaponSprite = new Sprite(HAPI_Sprites.MakeSurface("Data\\bow.png"));
	
	
}

void CEntityPlayer::initialiseValues() //this is temp!!
{

	pos_ = Point{ 960,540 };
	health_ = 4;
	speed_ = 4;
	reloadTime = 500 / 2;
	attack_ = 4;
	side = player;
	alive_ = true;
	angle_ = 0;


}


void CEntityPlayer::update(World& world)
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	
	oldPos = pos_;

	if (health_ <= 0)
		alive_ = false;
	

	float xRight = conData.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_X];
	float yRight = conData.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y];

	if (xRight != 0.0f || yRight != 0.0f) {
		currAngle = angle_;
		angle_ = atan2(-yRight, xRight);

	}
	
	if (angle_ > 3.14)
		angle_ -= 3.14;

	if ((conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzone_left_) && (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzone_left_))//right up
	{
		pos_.y -= (speed_ / 1.5);
		pos_.x += (speed_ / 1.5);
		frameOffset = 31;
		numberOfFramesForAnimation = 3;
	}
	else if ((conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzone_left_) && (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_))//right down
	{
		pos_.y += (speed_ / 1.5);
		pos_.x += (speed_ / 1.5);
		frameOffset = 25;
		numberOfFramesForAnimation = 3;
	}
	else if ((conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_) && (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzone_left_))//left up
	{
		pos_.y -= (speed_ / 1.5);
		pos_.x -= (speed_ / 1.5);
		frameOffset = 16;
		numberOfFramesForAnimation = 3;
	}
	else if ((conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_) && (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_))//left down
	{
		pos_.y += (speed_ / 1.5);
		pos_.x -= (speed_ / 1.5);
		frameOffset = 5;
		numberOfFramesForAnimation = 3;
	}
	else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_)//down
	{
		pos_.y += speed_;
		frameOffset = 1;
		numberOfFramesForAnimation = 3;
	}
	else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzone_left_)//up
	{
		pos_.y -= speed_;
		frameOffset = 36;
		numberOfFramesForAnimation = 3;
	}
	else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_)//left
	{
		pos_.x -= speed_;
		frameOffset = 11;
		numberOfFramesForAnimation = 3;
	}
	else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzone_left_)//right
	{
		pos_.x += speed_;
		frameOffset = 21;
		numberOfFramesForAnimation = 3;
	}
	else
	{
		frameOffset = 1;
		numberOfFramesForAnimation = 0;
	}

	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
	{
		shoot(world.getBullets().at(bulletNum));
		bulletNum++;

		if (angle_ > -0.80 && angle_ <= 0.80)	//right							//6.28 is 360 degrees in radians
		{
			frameOffset = 30;
			numberOfFramesForAnimation = 0;
		}
		else if (angle_ > -2.4 && angle_ <= -0.8)				//up					
		{
			frameOffset = 40;
			numberOfFramesForAnimation = 0;
		}
		else if (angle_ > 0.8 && angle_ <= 2.4)	//down								
		{
			frameOffset = 10;
			numberOfFramesForAnimation = 0;
		}
		else 	//left							
		{
			frameOffset = 15;
			numberOfFramesForAnimation = 0;
		}

		if (bulletNum > 499)
			bulletNum = 0;
	}


	
	if (HAPI_Sprites.GetTime() > invunerableTime)
		invunerable_ = false;


	angle_ = renderAngle;
	
	interpValue = 0;
}

void CEntityPlayer::shoot(CEntityBullet* bullet)
{
	if (HAPI_Sprites.GetTime() > timeToShoot)
	{
		bullet->setValues(*this); //need to make the player rotate so i can try shooting at different angles. I need to calc bullet direction from player angle
		timeToShoot = HAPI_Sprites.GetTime() + reloadTime;
	}
}

void CEntityPlayer::hasCollided(CEntity &other)
{
	if (other.getSide() == enemy && invunerable_ == false)
	{
		health_ -= other.getAttack();
		invunerable_ = true;
		invunerableTime = HAPI_Sprites.GetTime() + 200;
	}
	else if(other.getSide() == pickup)
	{
		maxHealth_ += other.getHealth();
		health_ += other.getHealth();
		speed_ += other.getSpeed();
		rof_ += other.getROF();
		reloadTime = 500 / (rof_+1);
		attack_ += other.getAttack();
	}
	/*else if (other.getSide() == wall)
	{
		pos_ = oldPos;
	}
	else if(other.isFinish())
		hasFinished == true then check for this in the world loop
	*/

}

void CEntityPlayer::render(Point playerPos)
{
	if (alive_ == true)
	{
		sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), _frameNum);
		//weaponSprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), _frameNum);

		if (frameTime < HAPI_Sprites.GetTime())
		{
			if (_frameNum >= (frameOffset + numberOfFramesForAnimation))
			{
				_frameNum = frameOffset;
			}
			else if (_frameNum < frameOffset)
			{
				_frameNum = frameOffset;
			}
			else
			{
				_frameNum++;
			}
			frameTime += 100;
		}
	}
}

