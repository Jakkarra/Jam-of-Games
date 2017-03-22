#include "Room.h"

Room::Room()
{
}

void Room::Initialise(HAPISPACE::Surface& Floor_Sprite_, Point Position_To_Spawn, std::string file_name, int texture_size)
{
	Floor_Position = Position_To_Spawn;
	Texture_Size = texture_size;

	Floor_Sprite = Floor_Sprite_.MakeCopy();
	FloorRect = Rectangle(Floor_Sprite_.Width(), Floor_Sprite_.Height());

	Sheet_Name = file_name;
	
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

	for (int y = 0; y < FloorRect.Height() / 32; y++)
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
	
	
	
	/*
	
	//Top_left
	spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{-Texture_Size, -Texture_Size }, 0);

	//Top_right
	spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{ FloorRect.Width() , -Texture_Size }, 1);

	//Bottom right
	spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{ FloorRect.Width(), FloorRect.Height()}, 3);

	//Bottom left
	spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{ -Texture_Size, FloorRect.Height()}, 2);

	for (int x = 0; x < FloorRect.Width()/32 ; x++)
	{
		spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{x * Texture_Size , -Texture_Size }, 5);
		spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{x * Texture_Size ,  FloorRect.Height() }, 5);

	}

	for (int y = 0; y < FloorRect.Height() / 32 ; y++)
	{
		spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{ -Texture_Size , y * Texture_Size }, 4);
		spriteSheet.Render(SCREEN_SURFACE, Floor_Position + Point{ FloorRect.Width() , y * Texture_Size }, 4);

	}
	*/
}

void Room::Render_Floor()
{
	Sprite Floor_To_Render(Floor_Sprite);

	Floor_To_Render.Render(SCREEN_SURFACE, Floor_Position);

	Sprite spriteSheet("Data\\" + Sheet_Name + ".xml", "Data\\");

	for (auto &p : Walls_And_Corners)
	{
		spriteSheet.Render(SCREEN_SURFACE, p.Wall_Position, p.Frame_Number);

	}
}


Room::~Room()
{
}



