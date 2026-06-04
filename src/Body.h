#pragma once
#include <vector>
#include "raylib.h"

enum BodyType
{
	Dynamic,
	Kinematic,
	Static
};

enum ForceMode
{
	Force,
	Impulse,
	Acceleration,
	VelocityChange
};

enum BallType
{
	NONE,
	IN,
	OUT,
};


struct Body
{
	BodyType body = Dynamic;
	Vector2 position = { 0, 0 };
	Vector2 velocity = { 0, 0 };
	Vector2 acceleration = { 0, 0 };
	float mass = 1.0f;	
	float inverseMass = 1.0f;
	float size = 10.0f;
	float restitution = 0.9f;
	float gravityScale = 1.0f;
	float damping = 0.0f;
	Color color = RED;
	BallType balltype;

	void AddForce(const Vector2& force, ForceMode forceMode = Force);
	void Draw() const;
	void Step(float dt);
};