
#include <SFML/Graphics.hpp>

using namespace sf;


int main()
{
	RenderWindow app(VideoMode(400, 400), "Minesweeper!");

	srand(time(0)); // seed

	const int TILES_COUNT = 12; // Texture image with 12 tiles
	const int TILE_WIDTH = 32;  // Texture size of 1 tile
	const int FIELD_SIZE = 10;  // Field size 10x10
	
	int gridView[TILES_COUNT][TILES_COUNT];  // visible to player
	int gridLogic[TILES_COUNT][TILES_COUNT]; // invisible to player
	
	// pressed tile  1  2  3  4  5  6  7  8  bomb  unpressed tile  flag
	//            0  1  2  3  4  5  6  7  8  9     10              11

	Texture texture;
	texture.loadFromFile("Images\\tiles.png");

	Sprite sprite(texture);

	// Create a field and place mines
	for (int i = 1; i <= FIELD_SIZE; i++)
	{
		for (int j = 1; j <= FIELD_SIZE; j++)
		{
			// Initial state of all dots = unpressed tile
			gridView[i][j] = 10;

			// Place mines randomly
			if (rand() % 5 == 0)
			{
				gridLogic[i][j] = 9;
			}
			else
			{
				gridLogic[i][j] = 0;
			}
		}
	}

	// Count mines around each dot
	for (int i = 1; i <= FIELD_SIZE; i++)
	{
		for (int j = 1; j <= FIELD_SIZE; j++)
		{
			int n = 0;
			if (gridLogic[i][j] == 9) continue;
			if (gridLogic[i + 1][j] == 9) n++;
			if (gridLogic[i][j + 1] == 9) n++;
			if (gridLogic[i - 1][j] == 9) n++;
			if (gridLogic[i][j - 1] == 9) n++;
			if (gridLogic[i + 1][j + 1] == 9) n++;
			if (gridLogic[i - 1][j - 1] == 9) n++;
			if (gridLogic[i - 1][j + 1] == 9) n++;
			if (gridLogic[i + 1][j - 1] == 9) n++;
			gridLogic[i][j] = n;
		}
	}
		
	// Main Loop
	while (app.isOpen())
	{
		// Get the coordinates of the mouse cursor relative to app window
		Vector2i pos = Mouse::getPosition(app);
		int x = pos.x / TILE_WIDTH;
		int y = pos.y / TILE_WIDTH;

		Event event;

		// Events processing
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				app.close();
			}
				
			if (event.type == Event::MouseButtonPressed)
			{
				// Open the dot
				if (event.key.code == Mouse::Left)
				{
					gridView[x][y] = gridLogic[x][y];

					// Open the field if hit by a bomb
					if (gridView[x][y] == 9)
					{
						for (int i = 1; i <= FIELD_SIZE; i++)
						{
							for (int j = 1; j <= FIELD_SIZE; j++)
							{
								gridView[i][j] = gridLogic[i][j];

							}
						}
					}
				}

				// Mark danger dot by a flag
				else if (event.key.code == Mouse::Right)
				{
					gridView[x][y] = 11;
				}
			}	
		}

		app.clear(Color::White);

		for (int i = 1; i <= FIELD_SIZE; i++)
		{
			for (int j = 1; j <= FIELD_SIZE; j++)
			{
				// Cut out the texture tiles
				sprite.setTextureRect(IntRect(gridView[i][j] * TILE_WIDTH, 0, TILE_WIDTH, TILE_WIDTH));
				// Set it to the given position
				sprite.setPosition(i * TILE_WIDTH, j * TILE_WIDTH);
				// Paint
				app.draw(sprite);
			}
		}

		app.display();
	}
	return 0;
}

