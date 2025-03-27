#include "HashGrid.h"

HashGrid::HashGrid(int cellSize)
  : cellSize(cellSize)
{}

void HashGrid::Init(std::vector<Particle*> particles)
{
  this->particles = particles;
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

std::vector<Particle*> HashGrid::GetNeighbors(Particle* particle)
{
  std::vector<Particle*> neighbors;
  vec2 pos = particle->pos;

  int particleGridX = static_cast<int>(pos.x / cellSize);
  int particleGridY = static_cast<int>(pos.y / cellSize);

  for (int x = -1; x <= 1; x++)
  {
    for (int y = -1; y <= 1; y++)
    {
      int gridX = particleGridX + x;
      int gridY = particleGridY + y;

      int hashID = IndexToHash(gridX, gridY);

      std::vector<Particle*> content = GetContentOfCell(hashID);

      neighbors.insert(neighbors.end(), content.begin(), content.end());
    }
  }

  return neighbors;
}

std::vector<Particle*> HashGrid::GetContentOfCell(int hashID)
{
  auto it = grid.find(hashID);

  if (it != grid.end())
  {
    return it->second;
  }

  return {};
}

void HashGrid::ParticleToCell()
{
  for (Particle* particle : particles)
  {
    int hash = GridHashFromPos(particle->pos);

    grid[hash].push_back(particle);
  }
}
