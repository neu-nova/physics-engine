#pragma once
#include "Body.h"
#include <vector>

class World
{
public:
	World() { bodies.reserve(1000); }

	void Step(float dt);
	void Draw() const;

	void AddBody(Body& body);
	void AddEffector(class Effector* effector);

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

private:
	void UpdateCollision();

private:
	Vector2 gravity = { 0, 9.81f };

	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
};
