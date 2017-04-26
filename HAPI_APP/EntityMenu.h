#pragma once
#include "Entity.h"
class CEntityMenu :
	public CEntity
{
public:
	CEntityMenu(std::string textureLocation);
	~CEntityMenu();
	void update(World &world);

	void initialiseValues();
	
private:

};

