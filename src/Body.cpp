#include "Body.h"
#include "Integrator.h"

void Body::AddForce(const Vector2& force, ForceMode forceMode)
{
	if (body != Dynamic) return;

	switch (forceMode)
	{
	case Force:
		acceleration += (force / mass) * gravityScale;
		break;
	case Impulse:
		velocity += (force * inverseMass);
		break;
	case Acceleration:
		acceleration += force;
		break;
	case VelocityChange:
		velocity += force;
		break;
	default:
		break;
	}
}

void Body::Draw() const
{
	DrawCircleV(position, size, WHITE);
	DrawCircleLinesV(position, size, RED);
}

void Body::Step(float dt)
{
	SemiImplicitEuler(*this, dt);
	acceleration = { 0,0 };
}