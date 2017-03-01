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
	void setPosition(Point pos) { _pos = pos; }
	void setAngle(float angle) { _angle = angle; }
	Sprite* getSprite() { return _sprite; }

protected:
	virtual void update() = 0;
	Sprite* _sprite;
	Point _pos = { 5,5 };
	unsigned int _frameNum = 0;
	float _angle = 0.0f;

private:

	


};

