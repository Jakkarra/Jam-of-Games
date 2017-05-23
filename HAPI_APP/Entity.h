#pragma once

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>
// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class World;

class CEntity
{
public:
	enum EnemyType
	{
		eMelee, eRanged, eBrute, eBoss, eplayer
	};

	CEntity(); 
	virtual ~CEntity();
	virtual void render(Point playerPos);

	enum sides { player, enemy, neutral, pickup };

	void setPosition(Point pos) { pos_ = pos; }
	void setAngle(float angle) { angle_ = angle; }
	
	Sprite* getPntrToSprite() { return sprite_; }
	Sprite getSprite() { return *sprite_; }
	Point getPos() { return pos_; }
	void movement() { pos_ += Point{ speed_,speed_ };}
	bool isAlive() { return alive_; }
	void setAlive(bool alive) { alive_ = alive; }
	bool isInvunerable() { return invunerable_; }
	int getAttack() { return attack_; }
	int getHealth() { return health_; }
	int getSpeed() { return speed_; }
	int getROF() { return rof_; }
	float getAngle() { return angle_; }
	sides getSide() { return side; }
	EnemyType getclass() { return myclass; }
	virtual void update(World& world) = 0;

	virtual void hasCollided(CEntity &other);
	virtual void initialiseValues() = 0;
protected:
	enum weapons { sword = 0, staff, bow };
	
	Sprite* sprite_{ nullptr };
	Point pos_ = { 5,5 };
	unsigned int _frameNum = 0;
	float angle_ = 0.0f;
	sides side;
	int health_ = 0;
	int maxHealth_ = 1;
	int speed_ = 0;
	int attack_ = 0;
	int rof_ = 0;
	bool alive_ = false;
	bool invunerable_ = false;
	Point oldPos = { 0,0 };
	float xVector = 0;
	float yVector = 0;
	float interpValue = 0.f;
	int min_range_{ 0 };
	int max_range_{ 0 };
	EnemyType myclass;
	unsigned int frameOffset = 0;
	unsigned int numberOfFramesForAnimation = 0;
	int frameTime = 100;

private:

	


};

