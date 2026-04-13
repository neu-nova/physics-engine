#include "Collision.h"
#include "Random.h"
#include "raymath.h"

bool Intersects(const Body& bodyA, const Body& bodyB)
{
	float distanceSqr = Vector2DistanceSqr(bodyA.position, bodyB.position);
	float radius = bodyA.size + bodyB.size;

	return (distanceSqr <= (radius * radius));
}

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.body != Dynamic && bodyB.body != Dynamic) continue;

			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distanceSqr = Vector2LengthSqr(direction);
				if (distanceSqr <= EPSILON)
				{
					direction = Vector2{ GetRandomFloat(-0.05f, 0.05f), GetRandomFloat(-0.05f, 0.05f) };
					distanceSqr = Vector2LengthSqr(direction);
				}

				float distance = sqrtf(distanceSqr);
				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);
				contact.restitution = (bodyA.restitution + bodyB.restitution) / 2.0f;

				contacts.push_back(contact);
			}
		}
	}
}

void SeparateContacts(std::vector<Contact>&contacts)
{ 
	for (auto& contact : contacts)
	{
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		Vector2 separation = contact.normal * (contact.depth / totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);
	}
}

void ResolveContacts(std::vector<Contact>&contacts)
{
	for (auto& contact : contacts)
	{
		// compute relative velocity
		Vector2 rv = contact.bodyA->velocity - contact.bodyB->velocity;
		// project relative velocity onto the contact normal
		float nv = Vector2DotProduct(rv, contact.normal);

		// skip if bodies are separating
		if (nv > 0) continue;

		// total inverse mass = (1/mA + 1/mB)
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		// impulse scalar = -(1 + restitution) * vn / (1/mA + 1/mB)
		float impulseMagnitude = -(1 + contact.restitution) * nv / totalInverseMass;

		// impulse vector along contact normal
		Vector2 impulse = contact.normal * impulseMagnitude;

		// apply equal and opposite impulses
		contact.bodyA->AddForce(impulse, Impulse);

		contact.bodyB->AddForce((impulse * -1), Impulse);
	}
}