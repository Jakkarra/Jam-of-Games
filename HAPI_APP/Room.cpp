#include "Room.h"
#include <ctime>

Room::Room()
{
}

Room::Room(std::string sprite_floor_name, Point Position_To_Spawn, std::string sheet_name, int texture_size)
{
	Floor_Position = Position_To_Spawn;
	Texture_Size = texture_size;

	Floor_Sprite = HAPI_Sprites.MakeSurface("Data\\" + sprite_floor_name);

	if (!Floor_Sprite->HasData())
	{
		return;
	}

	FloorRect = Rectangle(Floor_Sprite->Width(), Floor_Sprite->Height());

	CollisionRect = Rectangle(Floor_Position.x, Floor_Position.x + Floor_Sprite->Width(), Floor_Position.y, Floor_Position.y + Floor_Sprite->Height());

	Sheet_Name = sheet_name;

	Has_Path = false;

	Save_Sprite_Sheet_XML();

}

void Room::Create_Invidividual_Room()
{
	Create_Walls();
}

void Room::Create_Complex_Room(std::shared_ptr<Surface> other_surface)
{
	Create_Joined_Room(other_surface);
}

void Room::Save_Sprite_Sheet_XML()
{
	auto Corners_Walls_Texture = HAPI_Sprites.MakeSurface("Data\\" + Sheet_Name);

	Sprite *newSprite = new Sprite(Corners_Walls_Texture);	

	for (unsigned int y = 0; y < Corners_Walls_Texture->Height() / Texture_Size; y++)
	{
		for (unsigned int x = 0; x < Corners_Walls_Texture->Width() / Texture_Size; x++)
		{

			if (y == 0)
			{
				switch (x)
				{
				case 0: 			
					
					Room_Sheet_Frames.push_back(
					Frame("Top_left_corner", Rectangle(x * Texture_Size, (x + 1) * Texture_Size, y * Texture_Size, (y + 1) * Texture_Size)));
					break;

				case 1: 		

					Room_Sheet_Frames.push_back(
					Frame("Top_right_corner", Rectangle(x * Texture_Size, (x + 1) * Texture_Size, y * Texture_Size, (y + 1) * Texture_Size)));
					break;

				case 2:

					Room_Sheet_Frames.push_back(
					Frame("Bottom_left_corner", Rectangle(x * Texture_Size, (x + 1) * Texture_Size, y * Texture_Size, (y + 1) * Texture_Size)));
					break;


				case 3: 	

					Room_Sheet_Frames.push_back(
					Frame("Bottom_right_corner", Rectangle(x * Texture_Size, (x + 1) * Texture_Size, y * Texture_Size, (y + 1) * Texture_Size)));
					break;
				}
			}

			else
			{
				switch (x)
				{
				case 0: 					
					
					Room_Sheet_Frames.push_back(
					Frame("Wall_top_bottom", Rectangle(x * Texture_Size, (x + 1) * Texture_Size, y * Texture_Size, (y + 1) * Texture_Size)));
					break;
					

				case 2: 					
					
					Room_Sheet_Frames.push_back(
					Frame("Wall_left_right", Rectangle(x * Texture_Size, (x + 1) * Texture_Size, y * Texture_Size, (y + 1) * Texture_Size)));
					break;
				}
			}
		}
	}
	newSprite->ModifyFrames(Room_Sheet_Frames);

	newSprite->SaveAsXML("Data\\"+ Sheet_Name + ".xml",true);

}

bool Room::Location_Safe(ESide direction, int nr_elements_per_row_, int nr_elements_per_column_, int current_index_position)
{
	//UP
	if (current_index_position > nr_elements_per_row_ && direction == eTop)
	{
		if (Pathfinding_Points[current_index_position - nr_elements_per_row_].flag == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//DOWN
	if (current_index_position < (nr_elements_per_column_ * nr_elements_per_row_) - nr_elements_per_row_ && direction == eBottom)
	{
		if (Pathfinding_Points[current_index_position + nr_elements_per_row_].flag == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//RIGHT

	if (Pathfinding_Points[current_index_position].Position.y == Pathfinding_Points[current_index_position + 1].Position.y && direction == eRight)
	{
		if (Pathfinding_Points[current_index_position + 1].flag == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//LEFT
	if (Pathfinding_Points[current_index_position].Position.y == Pathfinding_Points[current_index_position - 1].Position.y && direction == eLeft)
	{
		if (Pathfinding_Points[current_index_position - 1].flag == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

ESide Room::Contains(Rectangle rct, Point other, int offset_value)
{
	if (other.x < rct.left && other.x > rct.left - offset_value)
		return eLeft;

	if (other.x > rct.right && other.x < rct.right + offset_value)
		return eRight;

	if (other.y < rct.top && other.y > rct.top - offset_value)
		return eTop;

	if (other.y > rct.bottom && other.y < rct.bottom + offset_value)
		return eBottom;


	return eOutside;
}


void Room::Pathfind_Corridor(Room & Other_Room)
{
	Rectangle *Pathfinding_Rectangle = new Rectangle(0, 0, 0, 0);

	int offset_for_corridor = Texture_Size * 4;

	if (Other_Room.Floor_Position.x < Floor_Position.x)
	{
		Pathfinding_Rectangle->left = Other_Room.Floor_Position.x - offset_for_corridor;
		Pathfinding_Rectangle->right = Floor_Position.x + FloorRect.Width() + offset_for_corridor;
	}
	else
	{
		Pathfinding_Rectangle->left = Floor_Position.x - offset_for_corridor;
		Pathfinding_Rectangle->right = Other_Room.Floor_Position.x + Other_Room.FloorRect.Width() + offset_for_corridor;
	}

	if (Other_Room.Floor_Position.y < Floor_Position.y)
	{
		Pathfinding_Rectangle->top = Other_Room.Floor_Position.y - offset_for_corridor;
		Pathfinding_Rectangle->bottom = Floor_Position.y + FloorRect.Height() + offset_for_corridor;
	}
	else
	{
		Pathfinding_Rectangle->top = Floor_Position.y - offset_for_corridor;
		Pathfinding_Rectangle->bottom = Other_Room.Floor_Position.y + Other_Room.FloorRect.Height() + offset_for_corridor;
	}

	Rectangle This_Room_Rectangle(FloorRect);
	Rectangle Other_Room_Rectangle(Other_Room.FloorRect);

	int index_counter = 0;

	float sensitive_value = 3.2f;

	for (int y = Pathfinding_Rectangle->top; y < Pathfinding_Rectangle->bottom; y += (int)offset_for_corridor / sensitive_value)
	{
		for (int x = Pathfinding_Rectangle->left; x < Pathfinding_Rectangle->right; x += (int)offset_for_corridor / sensitive_value)
		{
			Point migh_be_position{ x,y };

			int magnify_value = (int)Texture_Size * 1.4;

			This_Room_Rectangle.left = Floor_Position.x - magnify_value;
			This_Room_Rectangle.right = Floor_Position.x + FloorRect.Width() + magnify_value;
			This_Room_Rectangle.top = Floor_Position.y - magnify_value;
			This_Room_Rectangle.bottom = Floor_Position.y + FloorRect.Height() + magnify_value;

			Other_Room_Rectangle.left = Other_Room.Floor_Position.x - magnify_value;
			Other_Room_Rectangle.right = Other_Room.Floor_Position.x + Other_Room.FloorRect.Width() + magnify_value;
			Other_Room_Rectangle.top = Other_Room.Floor_Position.y - magnify_value;
			Other_Room_Rectangle.bottom = Other_Room.Floor_Position.y + Other_Room.FloorRect.Height() + magnify_value;

			if (This_Room_Rectangle.Contains(migh_be_position) || Other_Room_Rectangle.Contains(migh_be_position))
			{
				Pathfinding_Points.push_back(Intermediary_Point{ index_counter++, migh_be_position, false});
			}
			else
			{
				
				if ((Contains(This_Room_Rectangle, migh_be_position, Texture_Size) == eTop && Contains(Other_Room_Rectangle, migh_be_position, Texture_Size) == eBottom)
					|| (Contains(This_Room_Rectangle, migh_be_position, Texture_Size) == eBottom && Contains(Other_Room_Rectangle, migh_be_position, Texture_Size) == eTop)
					|| (Contains(This_Room_Rectangle, migh_be_position, Texture_Size) == eLeft && Contains(Other_Room_Rectangle, migh_be_position, Texture_Size) == eRight)
					|| (Contains(This_Room_Rectangle, migh_be_position, Texture_Size) == eRight && Contains(Other_Room_Rectangle, migh_be_position, Texture_Size) == eLeft)
					)
				{
					Pathfinding_Points.push_back(Intermediary_Point{ index_counter++, migh_be_position, false });
				}
				else
				{
					Pathfinding_Points.push_back(Intermediary_Point{ index_counter++, migh_be_position, true });
				}
			}
		}
	}

	int nr_elements_per_row = (Pathfinding_Rectangle->right - Pathfinding_Rectangle->left) / ((int)offset_for_corridor / sensitive_value) + 1;
	int nr_elements_per_column = (Pathfinding_Rectangle->bottom - Pathfinding_Rectangle->top) / ((int)offset_for_corridor  / sensitive_value) + 1;

	Corridor_Data.Pathfinding_Rectangle = Pathfinding_Rectangle;
	Corridor_Data.nr_elements_per_column = nr_elements_per_column;
	Corridor_Data.nr_elements_per_row = nr_elements_per_row;
	Corridor_Data.offset_for_corridor = offset_for_corridor;
}

void Room::Create_Corridor(int start_index, int end_index, Entrance First_Location, Entrance Second_Location)
{	
	int index = start_index;
	int nr_elements_per_row = (Corridor_Data.Pathfinding_Rectangle->right - Corridor_Data.Pathfinding_Rectangle->left) / ((int)Corridor_Data.offset_for_corridor / 3.2f) + 1;
	int nr_elements_per_column = (Corridor_Data.Pathfinding_Rectangle->bottom - Corridor_Data.Pathfinding_Rectangle->top) / ((int)Corridor_Data.offset_for_corridor / 3.2f) + 1;

	Pathfinding_Points[index];

	Special_Link(First_Location, Pathfinding_Points[index].Position);

	Special_Link(Second_Location, Pathfinding_Points[end_index].Position);

	bool target_reached = false;

	while (target_reached == false)
	{
		int dx = 0;
		int dy = 0;

		Point Current_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };

		Point Next_Position = { 0,0 };

		HAPISPACE::Line might_be_line(Current_Position, Next_Position);

		dx = Pathfinding_Points[end_index].Position.x - Pathfinding_Points[index].Position.x;

		dy = Pathfinding_Points[end_index].Position.y - Pathfinding_Points[index].Position.y;

		bool chosen = false;

		if (abs(dx) > abs(dy))
		{
			if (dx >= 0)
			{
				if (Location_Safe(eRight, nr_elements_per_row, nr_elements_per_column, index) == true)
				{
					index = index + 1;
					Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
					might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
					chosen = true;
				}
			}
			if (dx < 0)
			{
				if (Location_Safe(eLeft, nr_elements_per_row, nr_elements_per_column, index) == true)
				{
					index = index - 1;
					Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
					might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
					chosen = true;
				}
			}

			if (chosen == false)
			{
				if (dy >= 0)
				{
					if (Location_Safe(eBottom, nr_elements_per_row, nr_elements_per_column, index) == true)
					{
						index = index + nr_elements_per_row;
						Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
						might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
						chosen = true;
					}
				}

				if (dy < 0)
				{
					if (Location_Safe(eTop, nr_elements_per_row, nr_elements_per_column, index) == true)
					{
						index = index - nr_elements_per_row;
						Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
						might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
						chosen = true;
					}
				}

			}
		}
		else
		{
			if (dy >= 0)
			{
				if (Location_Safe(eBottom, nr_elements_per_row, nr_elements_per_column, index) == true)
				{
					index = index + nr_elements_per_row;
					Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
					might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
					chosen = true;
				}
			}

			if (dy < 0)
			{
				if (Location_Safe(eTop, nr_elements_per_row, nr_elements_per_column, index) == true)
				{
					index = index - nr_elements_per_row;
					Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
					might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
					chosen = true;
				}
			}

			if (chosen == false)
			{
				if (dx >= 0)
				{
					if (Location_Safe(eRight, nr_elements_per_row, nr_elements_per_column, index) == true)
					{
						index = index + 1;
						Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
						might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
						chosen = true;
					}
				}
				if (dx < 0)
				{
					if (Location_Safe(eLeft, nr_elements_per_row, nr_elements_per_column, index) == true)
					{
						index = index - 1;
						Next_Position = { Pathfinding_Points[index].Position.x , Pathfinding_Points[index].Position.y };
						might_be_line.p1 -= Point{ (Texture_Size / 2), (Texture_Size / 2) };
						chosen = true;
					}
				}
			}
		}

		if (Next_Position != HAPISPACE::Point{ 0 , 0 })
		{
			might_be_line.p2 = Next_Position;

			Point Middle_Point = might_be_line.p1 + Point{ ((might_be_line.p2 - might_be_line.p1).x / 2), ((might_be_line.p2 - might_be_line.p1).y / 2) };
			
			HAPISPACE::Line line_1(might_be_line.p1, Middle_Point);
			HAPISPACE::Line line_2(Middle_Point, might_be_line.p2); // Point{ (Texture_Size / 2), (Texture_Size / 2) }, might_be_line.p2);

			Actual_Path.push_back(line_1);
			Actual_Path.push_back(line_2);
		}

		if (index == end_index)
		{
			target_reached = true;
			All_Paths.push_back(Actual_Path);
			Actual_Path.clear();
			Pathfinding_Points.clear();
		}

	}
}

void Room::Special_Link(Entrance Position_1, Point Position_2)
{
	Point Current_Position = Position_1.Entrance_Position;

	int jump_value = 32;
	int offset_value = 12;

	if (Position_1.Entrance_Side == eRight)
	{
		while (Current_Position.x < Position_2.x + offset_value)
		{
			HAPISPACE::Line line_1(Current_Position, Current_Position + Point{ jump_value , 0 });
			Current_Position.x += jump_value;

			Actual_Path.push_back(line_1);

		}

	}

	if (Position_1.Entrance_Side == eLeft)
	{
		while (Current_Position.x > Position_2.x - offset_value)
		{
			HAPISPACE::Line line_1(Current_Position, Current_Position - Point{ jump_value , 0 });
			Current_Position.x -= jump_value;

			Actual_Path.push_back(line_1);

		}
	}
		

	if (Position_1.Entrance_Side == eBottom)
	{
		while (Current_Position.y < Position_2.y + offset_value)
		{
			HAPISPACE::Line line_1(Current_Position, Current_Position + Point{ 0 , jump_value });
			Current_Position.y += jump_value;

			Actual_Path.push_back(line_1);

		}
	}

	if (Position_1.Entrance_Side == eTop)
	{
		while (Current_Position.y > Position_2.y - offset_value)
		{
			HAPISPACE::Line line_1(Current_Position, Current_Position - Point{ 0 , jump_value });
			Current_Position.y -= jump_value;

			Actual_Path.push_back(line_1);

		}
	}
		
}

void Room::Link_Positions(Entrance First_Location, Entrance Second_Location)
{
	float Closest_Entrance = 100000.0f;
	float Closest_Exit = 10000.0f;
	int entrance_index = 0;

	
	int exit_index = 0;

	for (auto & p : Pathfinding_Points)
	{
		int dx_1 = (First_Location.Entrance_Position.x + Texture_Size/2) - p.Position.x;
		int dy_1 = (First_Location.Entrance_Position.y + Texture_Size / 2) - p.Position.y;
		float distEnt = std::sqrt((dx_1*dx_1) + (dy_1*dy_1));

		int dx_2 = (Second_Location.Entrance_Position.x + Texture_Size / 2) - p.Position.x;
		int dy_2 = (Second_Location.Entrance_Position.y + Texture_Size / 2) - p.Position.y;
		float distExit = std::sqrt((dx_2*dx_2) + (dy_2*dy_2));

		if (p.flag == true && distEnt < Closest_Entrance)
		{
			Closest_Entrance = distEnt;
			entrance_index = p.index;
		}

		if (p.flag == true && distExit < Closest_Exit)
		{
			Closest_Exit = distExit;	
			exit_index = p.index;
		}
	}

	Create_Corridor(entrance_index, exit_index, First_Location, Second_Location);

}

void Room::Link_Rooms(Room & Other_Room)
{
	float minimum_distance = 100000.f;
	Entrance Access_Point_1;
	Entrance Access_Point_2;

	for (auto & entrance : Room_Access_Points)
	{
		for (auto & exit : Other_Room.Room_Access_Points)
		{
			int dx_1 = (exit.Entrance_Position.x + Texture_Size / 2) - entrance.Entrance_Position.x;
			int dy_1 = (exit.Entrance_Position.y + Texture_Size / 2) - entrance.Entrance_Position.y;
			float distEnt = std::sqrt((dx_1*dx_1) + (dy_1*dy_1));

			if (distEnt < minimum_distance && entrance.In_Use == false && exit.In_Use==false)
			{
				minimum_distance = distEnt;
				Access_Point_1 = entrance;
				Access_Point_2 = exit;
			}

		}

	}

	for (auto & entrance : Room_Access_Points)
	{
		if (entrance.Entrance_Position == Access_Point_1.Entrance_Position)
		{
			entrance.In_Use = true;
		}
	}

	for (auto & exit : Other_Room.Room_Access_Points)
	{
		if (exit.Entrance_Position == Access_Point_2.Entrance_Position)
		{
			exit.In_Use = true;
		}
	}

	for (auto & wall : Walls_And_Corners)
	{
		if (wall.Wall_Position == Access_Point_1.Entrance_Position 
			|| wall.Wall_Position == Access_Point_1.Entrance_Position + Point{0,Texture_Size}
			|| wall.Wall_Position == Access_Point_1.Entrance_Position + Point{ Texture_Size, 0 })
		{
			wall.active = false;
		}
	}

	for (auto & wall : Other_Room.Walls_And_Corners)
	{
		if (wall.Wall_Position == Access_Point_2.Entrance_Position 
			|| wall.Wall_Position == Access_Point_2.Entrance_Position + Point{ 0,Texture_Size }
			|| wall.Wall_Position == Access_Point_2.Entrance_Position + Point{ Texture_Size, 0 })
		{
			wall.active = false;
		}
	}

	Has_Path = true;

	Link_Positions(Access_Point_1, Access_Point_2);
	
}

void Room::Spawn_Points(Point Player_Pos)
{
	/*
	for (auto p : Pathfinding_Points)
	{
		if (p.flag == true)
		{
			SCREEN_SURFACE.DrawFilledCircle(p.Position, 7, HAPI_TColour::Random());
		}
	}
	*/
	for (auto l : Actual_Path)
	{
		l.p1 -= Player_Pos - Point{960,540};
		l.p2 -= Player_Pos - Point{ 960,540 };
		SCREEN_SURFACE.DrawLine(l, HAPI_TColour::Random());
	}

}

void Room::Create_Walls()
{
	
	//Top_Left_Corner

	Walls_And_Corners.push_back(
								Wall_Or_Corner{ 0 , Floor_Position +  Point{ -Texture_Size, -Texture_Size }, true }
							   );

	//Top_Right_Corner
	
	Walls_And_Corners.push_back(
						        Wall_Or_Corner{ 1 , Floor_Position +  Point{ FloorRect.Width() , -Texture_Size }, true }
							   );

	//Bottom_Left_Corner

	Walls_And_Corners.push_back(
								Wall_Or_Corner{ 2 , Floor_Position +  Point{ -Texture_Size, FloorRect.Height() }, true }
							   );
	
	//Bottom_Right_Corner

	Walls_And_Corners.push_back(
								Wall_Or_Corner{ 3 , Floor_Position +  Point{ FloorRect.Width(), FloorRect.Height() }, true }
							   );

	for (int y = 0; y < FloorRect.Height() / Texture_Size; y++)
	{
		// Left Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ -Texture_Size , y * Texture_Size } }, true }
		);



		// Right Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ FloorRect.Width() , y * Texture_Size } }, true }
		);


	}

	int how_many = (int)FloorRect.Height() / Texture_Size;

	Room_Access_Points.push_back(Entrance{ { Point{ Floor_Position + Point{ -Texture_Size , (int)(how_many / 2) * Texture_Size } } },
											 eLeft,
											 false }
								);


	Room_Access_Points.push_back(Entrance{ { Point{ Floor_Position + Point{ FloorRect.Width() , (int)(how_many / 2) * Texture_Size } } },
											 eRight,
											 false }
								);




	for (int x = 0; x < FloorRect.Width() / Texture_Size; x++)
	{
		// Top Wall
		Walls_And_Corners.push_back(
									Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size , -Texture_Size } }, true }
								   );

		


		
		// Bottom Wall
		Walls_And_Corners.push_back(
									Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size ,  FloorRect.Height() } }, true }
								   );
	}

	how_many = (int)FloorRect.Width() / Texture_Size;

	Room_Access_Points.push_back(Entrance{ { Point{ Floor_Position + Point{ (int)(how_many / 2) * Texture_Size  , -Texture_Size } } },
											 eTop,
											 false }
								);

	Room_Access_Points.push_back(Entrance{ { Point{ Floor_Position + Point{ (int)(how_many / 2) * Texture_Size  , FloorRect.Height() } } },
											 eBottom,
											 false }
								);


}

void Room::Create_Joined_Room(std::shared_ptr<Surface> other_surface)
{
	int Other_Surface_Width = other_surface->Width();
	int Other_Surface_Height = other_surface->Height();

	Floor_Sprite = Floor_Sprite->Combine(other_surface, false);	
	
	
	//Top_Left_Corner

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 0 , Floor_Position + Point{ -Texture_Size, -Texture_Size }, true }
	);

	//Top_Right_Corner

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 1 , Floor_Position + Point{ FloorRect.Width() , -Texture_Size }, true }
	);

	//Bottom_Right_Corner_Initial_Rectangle

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 3 , Floor_Position + Point{ FloorRect.Width(), FloorRect.Height() }, true }
	);

	//Middle_"Corner"

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 0 , Floor_Position + Point{ Other_Surface_Width, FloorRect.Height() }, true }
	);

	//Bottom_Left_Corner_Other_Rectangle

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 2 , Floor_Position + Point{ -Texture_Size, FloorRect.Height() + Other_Surface_Height }, true }
	);

	//Bottom_Right_Corner_Other_Rectangle

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 3 , Floor_Position + Point{ Other_Surface_Width, FloorRect.Height() + Other_Surface_Height }, true }
	);


	for (int x = 0; x < FloorRect.Width() / Texture_Size; x++)
	{
		// Top Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size , -Texture_Size } }, true }
		);

	}

	for (int x = (Other_Surface_Width + Texture_Size) / Texture_Size; x < FloorRect.Width() / Texture_Size; x++)
	{

		// Middle Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size ,  FloorRect.Height() } }, true }
		);
	}

	for (int x = 0; x < Other_Surface_Width / Texture_Size; x++)
	{
		// Bottom Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size ,  FloorRect.Height() + Other_Surface_Height } }, true }
		);
	}

	for (int y = 0; y < FloorRect.Height() / Texture_Size; y++)
	{
		// Right Wall Initial Rectangle
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ FloorRect.Width() , y * Texture_Size } }, true }
		);
	}

	for (int y = (FloorRect.Height() + Texture_Size) / Texture_Size; y < (FloorRect.Height() + Other_Surface_Height) / Texture_Size; y++)
	{
		// Right Wall Other Rectangle
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ Other_Surface_Width , y * Texture_Size } }, true }
		);
	}

	for (int y = 0; y < (FloorRect.Height() + Other_Surface_Height) / Texture_Size; y++)
	{
		// Left Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ -Texture_Size , y * Texture_Size } }, true }
		);
	}
}

void Room::Render_Path(std::string file_name, Point PlayerPos)
{
	std::shared_ptr<Surface> Corridor_Sprite = HAPI_Sprites.MakeSurface("Data\\" + file_name);

	if (!Floor_Sprite->HasData())
	{
		return;
	}

	Sprite Corridor_Tile(Corridor_Sprite);

	for (auto & path : All_Paths)
	{

		for (auto & corridor : path)
		{
			Corridor_Tile.Render(SCREEN_SURFACE, corridor.p1 - (PlayerPos - Point(960, 540)));
		}
	}

	
}

void Room::Render_Floor(Point playerPos)
{
	Sprite Floor_To_Render(Floor_Sprite);

	Floor_To_Render.Render(SCREEN_SURFACE, Floor_Position - (playerPos - Point(960, 540)));

	Sprite spriteSheet("Data\\" + Sheet_Name + ".xml", "Data\\");

	for (auto &p : Walls_And_Corners)
	{
		if (p.active == true)
		{
			spriteSheet.Render(SCREEN_SURFACE, p.Wall_Position - (playerPos - Point(960, 540)), p.Frame_Number);
		}

	}
}

const Rectangle Room::Get_Collision_Rectangle() const
{
	return CollisionRect;
}

bool Room::Check_Path_Exists()
{
	return Has_Path;
}

Point Room::Get_Room_Position()
{
	return Floor_Position;
}


Room::~Room()
{
}



