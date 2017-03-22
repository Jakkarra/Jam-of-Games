#pragma once

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>
// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntity
{
public:
	CEntity(); //was used to take in textureLocation
	virtual ~CEntity();
	virtual void render();
	void setPosition(Point pos) { pos_ = pos; }
	void setAngle(float angle) { angle_ = angle; }
	Sprite* getSprite() { return sprite_; }
	void movement() { pos_ += Point{ speed_,speed_ };}
	virtual void initialiseValues();
	bool isAlive() { return alive_; }
	
protected:
	virtual void update() = 0;
	
	Sprite* sprite_{ nullptr };
	Point pos_ = { 5,5 };
	unsigned int _frameNum = 0;
	float angle_ = 0.0f;

	int health_ = 0;
	int speed_ = 0;
	int attack_ = 0;
	bool alive_ = false;
private:

	


};

