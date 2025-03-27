#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

#include "Particle.h"
#include "vec2.h"

class HashGrid
{
public:
  HashGrid(int cellSize);

  void Init(std::vector<Particle*> particles);
  void Clear();
  int GridHashFromPos(vec2 pos);
  int IndexToHash(int x, int y);
  std::vector<Particle*> GetNeighbors(int index);
  void ParticleToCell();
  void GetContentOfCell();

private:
  float cellSize;
  std::unordered_map<int, std::vector<Particle*>> grid;
  const int PRIME1 = 661401;
  const int PRIME2 = 752887;
  std::vector<Particle*> particles;
};
