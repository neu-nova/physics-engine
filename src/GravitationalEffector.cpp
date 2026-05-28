#include "GravitationalEffector.h"
#include "raymath.h"

void GravitationalEffector::Apply(std::vector<Body>& ibodies)
{
    std::vector<Body*> bodies;
    CollectBodiesInside(ibodies, bodies);


    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& bodyA = *bodies[i];
            Body& bodyB = *bodies[j];
            // STEP 1: Direction vector
			Vector2 direction = bodyB.position - bodyA.position;

                // STEP 2: Distance
			float distance = Vector2Length(direction);

                // STEP 3: Clamp distance
			distance = Clamp(distance, 5.0f, 1000.0f);

                // STEP 4: Force magnitude
			float forceMagnitude = strength * (bodyA.mass * bodyB.mass) / (distance * distance);

            // STEP 5: Force vector
			Vector2 force = Vector2Normalize(direction) * forceMagnitude;

                // STEP 6: Apply equal and opposite forces
			bodyA.AddForce(force);
			bodyB.AddForce(force * -1);
        }
    }
}

void GravitationalEffector::Draw()
{
    Effector::Draw();
    DrawCircleV(position, size, Fade(BLUE, 0.2f));
}