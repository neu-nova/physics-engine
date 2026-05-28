#include "Effector.h"
#include "raymath.h"

void Effector::Draw()
{
	DrawCircleLinesV(position, size, WHITE);
}

void Effector::CollectBodiesInside(const std::vector<Body>& ibodies, std::vector<Body*>& obodies)
{
	for (const Body& body : ibodies)
	{
		if (Vector2Distance(position, body.position) <= size)
		{
			obodies.push_back(const_cast<Body*>(&body));
		}
	}
}