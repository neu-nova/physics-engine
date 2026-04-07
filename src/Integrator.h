#pragma once
#include "Body.h"
#include "raymath.h"

inline void ExplicitEuler(Body& body, float dt)
{
	body.position += body.velocity * dt;
	body.velocity += body.acceleration * dt;
	body.velocity *= (1.0f / (1.0f + body.damping * dt));
}

inline void SemiImplicitEuler(Body& body, float dt)
{
	body.velocity += body.acceleration * dt;
	body.velocity *= (1.0f / (1.0f + body.damping * dt));
	body.position += body.velocity * dt;
}

inline void HalfEuler(Body& body, float dt)
{
	body.velocity += body.acceleration * dt / 2;
	body.position += body.velocity * dt;
	body.velocity += body.acceleration * dt / 2;
	body.velocity *= (1.0f / (1.0f + body.damping * dt));
}