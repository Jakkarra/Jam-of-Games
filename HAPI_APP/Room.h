#pragma once
#include <HAPISprites_lib.h>
#include <vector>
#include <string>
#include <cmath>
using namespace HAPISPACE;

enum ESide
{
	eRight,
	eLeft,
	eTop,
	eBottom,
	eInside,
	eOutside
};

class Room
{
private:
	struct Entrance
	{
		Point Entrance_Position;
		ESide Entrance_Side;
		bool In_Use;
	};

	struct Wall_Or_Corner
	{
		int Frame_Number;

		Point Wall_Position;

		bool active;
	};

public:

	Room();

	// Constructor for the room. Must be used in order to use render function.

	Room(std::string sprite_floor_name, Point Position_To_Spawn, std::string file_name, int texture_size);

	// Function to save a sprite ( Width ---must be--- equals to the Height of each frame of the sprite )

	void Create_Invidividual_Room();

	void Create_Complex_Room(std::shared_ptr<Surface> other_surface);

	void Save_Sprite_Sheet_XML();

	bool Location_Safe(ESide direction, int nr_elements_per_row_, int nr_elements_per_column_, int current_index_position);

	ESide Contains(Rectangle rct, Point other, int offset_value);

	void Pathfind_Corridor(Room & Other_Room);

	void Create_Corridor(int start_index, int end_index, Entrance First_Location, Entrance Second_Location);

	void Special_Link(Entrance Position_1, Point Position_2);

	void Link_Positions(Entrance First_Location, Entrance Second_Location);

	void Link_Rooms(Room & Other_Room);

	void Spawn_Points();

	// Pushing walls into the vector.

	void Create_Walls();

	void Create_Joined_Room(std::shared_ptr<Surface> other_surface);

	void Render_Path(std::string file_name);

	// Render walls & floor

	void Render_Floor(Point PlayerPos);

	~Room();



private:

	Point Floor_Position = { 0,0 };

	int Texture_Size{ 0 };

	std::shared_ptr<Surface> Floor_Sprite;

	Rectangle FloorRect;

	std::string Sheet_Name;

	std::vector<Frame> Room_Sheet_Frames;

	std::vector<Entrance> Room_Access_Points;

	std::vector<Wall_Or_Corner> Walls_And_Corners;

	struct Intermediary_Point
	{
		int index;
		Point Position;
		bool flag;
	};

	std::vector<Intermediary_Point> Pathfinding_Points;
	
	struct Corridor_Information
	{
		Rectangle *Pathfinding_Rectangle;
		int nr_elements_per_row;
		int nr_elements_per_column;
		int offset_for_corridor;
	};

	Corridor_Information Corridor_Data;

	std::vector<HAPISPACE::Line> Actual_Path;

};

