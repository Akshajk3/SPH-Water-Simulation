#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Particle.h"
#include "vec2.h"

class HashGrid
{
public:
  HashGrid(int cellSize);

  void Init(std::vector<Particle> particles);
  void Clear();
  void GridHashFromPos(vec2 pos);
  void IndexToHash(int x, int y);
  void GetNeighborIndex(int index);
  void ParticleToCell();
  
};
