#include "World.h"
#include "Integrator.h"
#include "raymath.h"
#include "Effector.h"

void World::Step(float dt)
{

    for (Body& body : bodies) body.AddForce(gravity * 300.0f);

    for (auto& effector : Effectors) effector->Apply(bodies);

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_X))
    {
        Vector2 position = GetMousePosition();
        for (Body& body : bodies)
        {
            Vector2 dir = position - body.position;
            if (Vector2Length(dir) < 100.0f)
            {
                Vector2 force = Vector2Normalize(dir) * 10000.0f;
                body.AddForce(force);
            }
        }
        DrawCircleLinesV(position, 100, WHITE);
    }

    for (Body& body : bodies) body.Step(dt);


    UpdateCollision();
}

void World::Draw() const
{
	for (const Body& body : bodies) body.Draw();
}

void World::UpdateCollision()
{
	for (Body& body : bodies)
	{
		if (body.position.x + body.size > GetScreenWidth())
		{
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < 0)
		{
			body.position.x = body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > GetScreenHeight())
		{
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < 0)
		{
			body.position.y = body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}

void World::AddBody(Body& body)
{
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	Effectors.push_back(effector);
}