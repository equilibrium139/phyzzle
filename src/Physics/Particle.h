#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    Particle(Vec2 position, float mass, int radius);
    void AddForce(Vec2 force) { sum_forces += force; }
    void Integrate(float dt);

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sum_forces;

    float inverse_mass;
    int radius;
};

#endif
