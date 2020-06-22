#include "Map.h"
#include "Cylinder.h"

scene::Map::Map(std::string name, int width, int height) : name(name),
	width(width), height(height)
{
	levels = MAP_COUNT_LEVELS;
	structure.resize(levels);
	for (int level = 0; level < levels; level++)
	{
		this->structure[level].resize(width * height);
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				this->structure[level][width * row + col].setCoords(col, row, level);
			}
		}
	}	
}

void scene::Map::addThing(data::Thing newThing, int line, int col, int level)
{
	this->structure[level][width * line + col].addItem(newThing);
}

void scene::Map::removeThing(std::string name, int x, int y, int z)
{
	this->structure[z][width * y + x].removeItem(name);
}

void scene::Map::drawMap(cairo_t* cr)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			structure[0][width * y + x].draw(cr);
		}
	}
}

int scene::Map::getWidth() const
{
	return width;
}

int scene::Map::getHeight() const
{
	return height;
}