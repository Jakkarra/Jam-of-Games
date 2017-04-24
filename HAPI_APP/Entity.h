#pragma once

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>
// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class World;

class CEntity
{
public:
	CEntity(); 
	virtual ~CEntity();
	virtual void render();
	void setPosition(Point pos) { pos_ = pos; }
	void setAngle(float angle) { angle_ = angle; }
	Sprite* getSprite() { return sprite_; }
	void movement() { pos_ += Point{ speed_,speed_ };}
	virtual void initialiseValues() = 0;
	bool isAlive() { return alive_; }
	virtual void update(World& world) = 0;
	
protected:

	enum sides { player, enemy, neutral,};
	
	
	Sprite* sprite_{ nullptr };
	Point pos_ = { 5,5 };
	unsigned int _frameNum = 0;
	float angle_ = 0.0f;
	sides side;
	int health_ = 0;
	int speed_ = 0;
	int attack_ = 0;
	bool alive_ = false;
private:

	


};

