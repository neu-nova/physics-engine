#pragma once
#include "Body.h"
#include "Collision.h"
#include <vector>

class World
{
public:
	World() { bodies.reserve(1000); }

	void Step(float dt);
	void Draw() const;

	void AddBody(Body& body);
	void AddEffector(class Effector* effector);
	void AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness);

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

	Body* GetBodyIntersect(const Vector2& point);

	static void SetGravity(const Vector2& g) { gravity = g; }
	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

private:
	void UpdateCollision();

private:
	static Vector2 gravity;

	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<Contact> contacts;
	std::vector<class Spring*> springs;
	Vector2 boundsMin = { -10.0f, -5.0f };
	Vector2 boundsMax = { 10.0f, 5.0f };
};
