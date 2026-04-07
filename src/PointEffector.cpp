#include "PointEffector.h"
#include "raymath.h"

void PointEffector::Apply(std::vector<Body>& bodies)
{
	for (Body& body : bodies)
	{
		Vector2 dir = position - body.position;
		if (Vector2Length(dir) <= size)
		{
			Vector2 force = Vector2Normalize(dir) * forceMagnitude;
			body.AddForce(force);
		}
	}
}

void PointEffector::Draw()
{
	DrawCircleLinesV(position, size, WHITE);
}