#include "Room.h"

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

	Sheet_Name = sheet_name;

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


void Room::Create_Walls()
{
	
	//Top_Left_Corner

	Walls_And_Corners.push_back(
								Wall_Or_Corner{ 0 , Floor_Position +  Point{ -Texture_Size, -Texture_Size } }
							   );

	//Top_Right_Corner
	
	Walls_And_Corners.push_back(
						        Wall_Or_Corner{ 1 , Floor_Position +  Point{ FloorRect.Width() , -Texture_Size } }
							   );

	//Bottom_Left_Corner

	Walls_And_Corners.push_back(
								Wall_Or_Corner{ 2 , Floor_Position +  Point{ -Texture_Size, FloorRect.Height() } }
							   );
	
	//Bottom_Right_Corner

	Walls_And_Corners.push_back(
								Wall_Or_Corner{ 3 , Floor_Position +  Point{ FloorRect.Width(), FloorRect.Height() } }
							   );



	for (int x = 0; x < FloorRect.Width() / Texture_Size; x++)
	{
		// Top Wall
		Walls_And_Corners.push_back(
									Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size , -Texture_Size } } }
								   );
		
		// Bottom Wall
		Walls_And_Corners.push_back(
									Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size ,  FloorRect.Height() } } }
								   );

	}

	for (int y = 0; y < FloorRect.Height() / Texture_Size; y++)
	{
		// Left Wall
		Walls_And_Corners.push_back(
									Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ -Texture_Size , y * Texture_Size } } } 
								   );

		// Right Wall
		Walls_And_Corners.push_back(
									Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ FloorRect.Width() , y * Texture_Size } } }
								    );
	}

}

void Room::Create_Joined_Room(std::shared_ptr<Surface> other_surface)
{
	int Other_Surface_Width = other_surface->Width();
	int Other_Surface_Height = other_surface->Height();

	Floor_Sprite = Floor_Sprite->Combine(other_surface, false);	
	
	
	//Top_Left_Corner

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 0 , Floor_Position + Point{ -Texture_Size, -Texture_Size } }
	);

	//Top_Right_Corner

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 1 , Floor_Position + Point{ FloorRect.Width() , -Texture_Size } }
	);

	//Bottom_Right_Corner_Initial_Rectangle

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 3 , Floor_Position + Point{ FloorRect.Width(), FloorRect.Height() } }
	);

	//Middle_"Corner"

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 0 , Floor_Position + Point{ Other_Surface_Width, FloorRect.Height() } }
	);

	//Bottom_Left_Corner_Other_Rectangle

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 2 , Floor_Position + Point{ -Texture_Size, FloorRect.Height() + Other_Surface_Height } }
	);

	//Bottom_Right_Corner_Other_Rectangle

	Walls_And_Corners.push_back(
		Wall_Or_Corner{ 3 , Floor_Position + Point{ Other_Surface_Width, FloorRect.Height() + Other_Surface_Height } }
	);


	for (int x = 0; x < FloorRect.Width() / Texture_Size; x++)
	{
		// Top Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size , -Texture_Size } } }
		);

	}

	for (int x = (Other_Surface_Width + Texture_Size) / Texture_Size; x < FloorRect.Width() / Texture_Size; x++)
	{

		// Middle Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size ,  FloorRect.Height() } } }
		);
	}

	for (int x = 0; x < Other_Surface_Width / Texture_Size; x++)
	{
		// Bottom Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 5 , Point{ Floor_Position + Point{ x * Texture_Size ,  FloorRect.Height() + Other_Surface_Height } } }
		);
	}

	for (int y = 0; y < FloorRect.Height() / Texture_Size; y++)
	{
		// Right Wall Initial Rectangle
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ FloorRect.Width() , y * Texture_Size } } }
		);
	}

	for (int y = (FloorRect.Height() + Texture_Size) / Texture_Size; y < (FloorRect.Height() + Other_Surface_Height) / Texture_Size; y++)
	{
		// Right Wall Other Rectangle
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ Other_Surface_Width , y * Texture_Size } } }
		);
	}

	for (int y = 0; y < (FloorRect.Height() + Other_Surface_Height) / Texture_Size; y++)
	{
		// Left Wall
		Walls_And_Corners.push_back(
			Wall_Or_Corner{ 4 , Point{ Floor_Position + Point{ -Texture_Size , y * Texture_Size } } }
		);
	}
}

void Room::Render_Floor(Point PlayerPos)
{
	Sprite Floor_To_Render(Floor_Sprite);

	Floor_To_Render.Render(SCREEN_SURFACE, Floor_Position - (PlayerPos - Point(960, 540)));

	Sprite spriteSheet("Data\\" + Sheet_Name + ".xml", "Data\\");

	for (auto &p : Walls_And_Corners)
	{
		spriteSheet.Render(SCREEN_SURFACE, p.Wall_Position - (PlayerPos - Point(960, 540)), p.Frame_Number);

	}
}


Room::~Room()
{
}



