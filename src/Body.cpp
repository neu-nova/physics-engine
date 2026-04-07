#include "Body.h"
#include "Integrator.h"

void Body::AddForce(const Vector2& force)
{
	acceleration.x += (force.x / mass) * gravityScale;
	acceleration.y += (force.y / mass) * gravityScale;
}

void Body::Draw() const
{
	DrawCircleV(position, size, WHITE);
}

void Body::Step(float dt)
{
	SemiImplicitEuler(*this, dt);
	acceleration = { 0,0 };
}