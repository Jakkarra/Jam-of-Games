#include "EntityPlayer.h"
#include "World.h"



CEntityPlayer::CEntityPlayer()//:CEntity(textureLocation)
{
	sprite_ = new Sprite(HAPI_Sprites.MakeSurface("Data\\tranImage.png"));// we could make it so you pass in a value for the texture but we should know what textures are going to be used
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

	//best to probably make a vector of "to move" and then add up all movements then before applying check if it is possible, room or wall in way etc.
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzone_left_)
	{
		pos_.y += speed_;
		//directionToMove += speed;
		//more of these then use final speed value to calculate the rendering
		//can use to check if 
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzone_left_)
	{

		pos_.y -= speed_;

	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzone_left_)
	{

		pos_.x -= speed_;

	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzone_left_)
	{
		pos_.x += speed_;
	}

	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
	{
		shoot(world.getBullets().at(bulletNum));
		bulletNum++;

		if (bulletNum > 499)
			bulletNum = 0;
	}


	
	if (HAPI_Sprites.GetTime() > invunerableTime)
		invunerable_ = false;

	

	if (angle_ >= 6.28)//6.28 is 360 degrees in radians
	{
		angle_ -= 6.28;
	}
	
	
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

