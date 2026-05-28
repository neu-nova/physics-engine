#include "DragEffector.h"
#include "raymath.h"

void DragEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		Vector2 force = body->velocity * -drag;
		body->AddForce(force);
	}
}

void DragEffector::Draw()
{
	Effector::Draw();
	DrawCircleV(position, size, Fade(YELLOW, 0.2f));
}