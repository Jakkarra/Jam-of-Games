#pragma once
#include <HAPISprites_lib.h>
#include <vector>
#include <string>
using namespace HAPISPACE;

class Room
{
public:

	Room();

	// Constructor for the room. Must be used in order to use render function.

	Room(std::string sprite_floor_name, Point Position_To_Spawn, std::string file_name, int texture_size);

	// Function to save a sprite ( Width ---must be--- equals to the Height of each frame of the sprite )

	void Create_Invidividual_Room();

	void Create_Complex_Room(std::shared_ptr<Surface> other_surface);

	void Save_Sprite_Sheet_XML();

	// Pushing walls into the vector.

	void Create_Walls();

	void Create_Joined_Room(std::shared_ptr<Surface> other_surface);

	// Render walls & floor

	void Render_Floor(Point PlayerPos);

	Point getPos() const { return  Floor_Position; }
	int getsize() const { return  Texture_Size; }

	~Room();

private:

	Point Floor_Position = { 0,0 };

	int Texture_Size{ 0 };

	std::shared_ptr<Surface> Floor_Sprite;

	Rectangle FloorRect;

	std::string Sheet_Name;

	std::vector<Frame> Room_Sheet_Frames;

	// Information about each wall and corner.

	struct Wall_Or_Corner
	{
		int Frame_Number;

		Point Wall_Position;
	};

	// Vector that stores walls and corners.

	std::vector<Wall_Or_Corner> Walls_And_Corners;



};

