#include "Spring.h"
#include "Body.h"


void Spring::Apply(float multiplier)
{


    Vector2 force = GetSpringForce(bodyA->position, bodyB->position, restLength, stiffness * multiplier);

    Vector2 direction = bodyB->position - bodyA->position;
    float length = Vector2Length(direction);
    if (length <= 1e-6f) return; // avoid divide-by-zero and huge forces
    Vector2 ndirection = Vector2Normalize(direction);

    Vector2 rv = bodyB->velocity - bodyA->velocity;
    float dampingFactor = Vector2DotProduct(ndirection, rv) * damping;

    Vector2 dampingForce = ndirection * dampingFactor;
    force -= dampingForce;

    bodyA->AddForce(force * -1.0f);
	bodyB->AddForce(force);
}

void Spring::Draw() 
{
    DrawLineV(bodyA->position, bodyB->position, WHITE);
}

Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness)
{
    Vector2 direction = positionB - positionA;
    float length = Vector2Length(direction);
	if (length <= 1e-6f) return Vector2Zero(); // guard against zero-length

    float displacement = length - restLength;
    float magnitude = -(displacement * stiffness);

    return Vector2Normalize(direction) * magnitude;
}