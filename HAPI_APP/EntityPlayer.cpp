#include "EntityPlayer.h"



CEntityPlayer::CEntityPlayer(std::string textureLocation) :CEntity(textureLocation)
{



}


CEntityPlayer::~CEntityPlayer()
{
	delete _sprite;
}

void CEntityPlayer::update()
{

}

