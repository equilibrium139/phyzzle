#pragma once

#include "Particle.h"

namespace phyz
{

Vec2 GenerateDragForce(const Particle& particle, float strength);
void AddDragForce(Particle& particle, float strength);

Vec2 GenerateWeightForce(const Particle& particle, float gravity = 9.8f);
void AddWeightForce(Particle& particle, float gravity = 9.8f);

Vec2 GenerateFrictionForce(const Particle& particle, float strength);
void AddFrictionForce(Particle& particle, float strength);

}