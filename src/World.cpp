#include "World.h"
#include "Integrator.h"
#include "raymath.h"
#include "Effector.h"
#include "Spring.h"

Vector2 World::gravity = { 0, 9.81f };
float World::springmultiplier = 1.0f;

void World::Step(float dt)
{

	//for (Body& body : bodies) body.acceleration = { 0,0 };
	for (Body& body : bodies) body.AddForce(gravity * body.mass * body.gravityScale * -1, Acceleration);


    for (auto& effector : effectors) effector->Apply(bodies);

	for (auto& spring : springs) spring->Apply(springmultiplier);

    for (Body& body : bodies) body.Step(dt);

	for (int i = 0; i < 4; i++) UpdateCollision();

	for (auto& body : bodies) body.acceleration = { 0,0 };
}

void World::Draw() const
{
	
	for(float x = boundsMin.x; x < (boundsMax.x); x += 1)
	{
		DrawLineV(Vector2{ x, boundsMin.y }, Vector2{ x, boundsMax.y }, DARKGRAY);
	}
	for (float y = boundsMin.y; y < (boundsMax.y); y += 1)
	{
		DrawLineV(Vector2{ boundsMin.x, y }, Vector2{ boundsMax.x, y }, DARKGRAY);
	}
	

	for (auto& effector : effectors) effector->Draw();
	for (const Body& body : bodies) body.Draw();
	for (auto& spring : springs) spring->Draw();
}


Body* World::GetBodyIntersect(const Vector2& point)
{
	for (Body& body : bodies)
	{
		if (CheckCollisionPointCircle(point, body.position, body.size))
		{
			return &body;
		}
	}
	return nullptr;
}

void World::UpdateCollision()
{
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);
	ResolveContacts(contacts);

	for (Body& body : bodies)
	{
		if (body.position.x + body.size > boundsMax.x)
		{
			body.position.x = boundsMax.x - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < boundsMin.x)
		{
			body.position.x = boundsMin.x + body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > boundsMax.y)
		{
			body.position.y = boundsMax.y - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < boundsMin.y)
		{
			body.position.y = boundsMin.y + body.size;
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
	effectors.push_back(effector);
}

void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness, float damping)
{
	springs.push_back(new Spring(&bodyA, &bodyB, restLength, stiffness, damping));
}