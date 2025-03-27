#include "HashGrid.h"

HashGrid::HashGrid(int cellSize)
  : cellSize(cellSize)
{}

void HashGrid::Init(std::vector<Particle*> particles)
{
  particles = particles;
}

void HashGrid::Clear()
{
  grid.clear();
}

int HashGrid::GridHashFromPos(vec2 pos)
{
  int x = static_cast<int>(std::floor(pos.x / cellSize));
  int y = static_cast<int>(std::floor(pos.y / cellSize));

  return IndexToHash(x, y);
}

int HashGrid::IndexToHash(int x, int y)
{
  int hash = (x * PRIME1) ^ (y * PRIME2);
  return hash; 
}
