#pragma once

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h> 

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntity
{
public:
	CEntity(std::string textureLocation);
	virtual ~CEntity();
	virtual void render();
	void CEntity::initialise();
	void setPosition(Point pos) { pos_ = pos; }
	void setAngle(float angle) { angle_ = angle; }
	Sprite* getSprite() { return sprite_; }

protected:
	virtual void update() = 0;
	Sprite* sprite_;
	Point pos_ = { 5,5 };
	unsigned int _frameNum = 0;
	float angle_ = 0.0f;

	int health_;
	int speed_;
	int attack_;
private:

	


};

