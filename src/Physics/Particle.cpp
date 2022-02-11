#include "Particle.h"
#include <iostream>

Particle::Particle(Vec2 position, float mass, int radius)
	:position(position), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), sum_forces(0.0f, 0.0f), mass(mass), inverse_mass(1.0f / mass), radius(radius)
{
}

void Particle::Integrate(float dt)
{
    acceleration = sum_forces * inverse_mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    sum_forces = Vec2(0.0f, 0.0f);
}
