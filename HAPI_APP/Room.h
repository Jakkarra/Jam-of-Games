#pragma once
#include <HAPISprites_lib.h>
#include <vector>
#include <string>
using namespace HAPISPACE;

class Room
{
public:

	Room();

	void Initialise(HAPISPACE::Surface& Floor_Sprite_, Point Position_To_Spawn, std::string file_name, int texture_size);

	void Save_Sprite_Sheet_XML();

	void Create_Walls();

	void Render_Floor();

	~Room();

private:

	Point Floor_Position = { 0,0 };

	int Texture_Size{ 0 };

	std::shared_ptr<Surface> Floor_Sprite;

	Rectangle FloorRect;

	std::string Sheet_Name;

	std::vector<Frame> Room_Sheet_Frames;

	struct Wall_Or_Corner
	{
		int Frame_Number;

		Point Wall_Position;
	};

	std::vector<Wall_Or_Corner> Walls_And_Corners;



};

