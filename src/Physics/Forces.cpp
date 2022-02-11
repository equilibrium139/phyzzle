#include "Forces.h"

namespace phyz
{
Vec2 GenerateDragForce(const Particle& particle, float strength)
{
    Vec2 drag_force{ 0.0f, 0.0f };

    if (float drag_force_magnitude = particle.velocity.MagnitudeSquared() * strength; drag_force_magnitude > 0.0f)
    {
        Vec2 opposite_velocity_dir = -particle.velocity.UnitVector();
        drag_force = opposite_velocity_dir * drag_force_magnitude;
    }

    return drag_force;
}

void AddDragForce(Particle& particle, float strength)
{
    Vec2 drag_force = GenerateDragForce(particle, strength);
    particle.AddForce(drag_force);
}

Vec2 GenerateWeightForce(const Particle& particle, float gravity)
{
    return Vec2{ 0.0f, gravity * particle.mass };
}

void AddWeightForce(Particle& particle, float gravity)
{
    particle.AddForce(GenerateWeightForce(particle, gravity));
}

Vec2 GenerateFrictionForce(const Particle& particle, float strength)
{
    Vec2 force{ 0.0f, 0.0f };

    if (particle.velocity.MagnitudeSquared() > 0.0f)
    {
        Vec2 opposite_velocity_dir = -particle.velocity.UnitVector();
        force = opposite_velocity_dir * strength;
    }

    return force;
}

void AddFrictionForce(Particle& particle, float strength)
{
    particle.AddForce(GenerateFrictionForce(particle, strength));
}

}
