#include "EntityPlayer.h"
#include "World.h"



CEntityPlayer::CEntityPlayer()
{

	initialiseValues();

}

CEntityPlayer::~CEntityPlayer()
{
}
//delete sprite

void CEntityPlayer::initialiseValues(int health, int speed, int rof, int damage, int weapon)
{
	
	pos_ = Point{ 960,540 };
	health_ = 2 + health;
	maxHealth_ = 2 + health;
	speed_ = 3 + speed;
	rof_ = rof;
	reloadTime = 1000 / rof;
	attack_ = 1 + damage;
	side = player;
	myclass = eplayer;
	alive_ = true;
	angle_ = 0;
	
	currentWeapon = (weapon*weapon) % 2;//for now, should be currentWeapon = weapon;

	
	if(currentWeapon == 0)
		sprite_ = new Sprite("Data\\bowman.xml", "Data\\");
	else 
		sprite_ = new Sprite("Data\\staffman.xml", "Data\\");
	
	frameTime = HAPI_Sprites.GetTime();
	
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
	hasMoved = false;
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
	
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_)//down
	{
		pos_.y += speed_;
		frameOffset = 31;
		
		world.PowerUp(pos_ + Point(50, 50));
		hasMoved = true;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzone_left_)//up
	{
		pos_.y -= speed_;	
		frameOffset = 10;

		hasMoved = true;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_)//left
	{
		pos_.x -= speed_;
		frameOffset = 1;
	
			
		hasMoved = true;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzone_left_)//right
	{
		pos_.x += speed_;
		frameOffset = 22;
		hasMoved = true;
	}
	

	if (hasMoved == false)
	{
		frameOffset = 31;
		numberOfFramesForAnimation = 0;
	}
	else
			numberOfFramesForAnimation = 3;
	
	


	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
	{


		
			if (angle_ > -0.80 && angle_ <= 0.80)    //right                            //6.28 is 360 degrees in radians
			{
				if (currentWeapon == 0)
				{
					frameOffset = 21;
					shootOffset = Point(30, 40);
				}

			}
			else if (angle_ > -2.4 && angle_ <= -0.8)                //up                    
			{
				if (currentWeapon == 0)
				{
					frameOffset = 15;
					shootOffset = Point(10, 20);
				}
			}
			else if (angle_ > 0.8 && angle_ <= 2.4)    //down                                
			{
				if (currentWeapon == 0)
				{
					frameOffset = 35;
					shootOffset = Point(5, 60);
				}
			}
			else     //left                            
			{
				if (currentWeapon == 0)
				{
					frameOffset = 41;
					shootOffset = Point(0, 40);
				}
			}
			numberOfFramesForAnimation = 0;

			if (HAPI_Sprites.GetTime() > timeToShoot)
			{
				shoot(world.getBullets().at(bulletNum));
				timeToShoot = HAPI_Sprites.GetTime() + reloadTime;
				bulletNum++;
				if (bulletNum > 499)
					bulletNum = 0;
			}
	}

	if (HAPI_Sprites.GetTime() > invunerableTime)
		invunerable_ = false;

	
}

void CEntityPlayer::shoot(CEntityBullet* bullet)
{
	bullet->setValues(*this, currentWeapon, shootOffset); //need to make the player rotate so i can try shooting at different angles. I need to calc bullet direction from player angle
}

void CEntityPlayer::hasCollided(CEntity &other)
{
	if (other.getSide() == enemy && invunerable_ == false)
	{
		health_ -= other.getAttack();
		invunerable_ = true;
		invunerableTime = HAPI_Sprites.GetTime() + 2000;
		HAPI_Sprites.PlaySound("Data\\Grunt.wav");
	}
	else if(other.getSide() == pickup)
	{
		maxHealth_ += other.getHealth();
		health_ += other.getHealth();
		speed_ += other.getSpeed();
		rof_ += other.getROF();
		reloadTime = 500 / (rof_+1);
		attack_ += other.getAttack();
		HAPI_Sprites.PlaySound("Data\\Power_Up.wav");
	}/*
	else if(other.isFinish())
		hasFinished == true then check for this in the world loop
	*/

}

void CEntityPlayer::render(Point playerPos)
{
	
	
	if (alive_ == true)
	{
		if (outOfBounds == true)
		{
			pos_ = oldPos;
			outOfBounds = false;
		}
		if (invunerable_ == true)
		{
			sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), [&](const Point p, HAPI_TColour& dest, const HAPI_TColour& source) {
				if (source.alpha == 255)
				{
					dest.red = dest.red + source.red/3 - 20;//making the player more 'white' if invunerable, easier than alpha i believe
					dest.green = dest.green + source.green / 3 - 20;
					dest.blue = dest.blue + source.blue / 3 - 20;
				}
			}, _frameNum);

		}
		else
			sprite_->Render(SCREEN_SURFACE, pos_ - (playerPos - Point(960, 540)), _frameNum);

		if (HAPI_Sprites.GetTime() > frameTime)
		{
			if (_frameNum >= frameOffset + numberOfFramesForAnimation)
				_frameNum = frameOffset;
			else if (_frameNum < frameOffset)
				_frameNum = frameOffset;
			else
				_frameNum++;

			frameTime += 200 - (10 * speed_);
		}
		
	}
}

