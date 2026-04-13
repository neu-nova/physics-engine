/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include "World.h"
#include "Random.h"
#include "GravitationalEffector.h"

#include <string>

int main ()
{

	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	SetTargetFPS(60);

	World world;
	world.AddEffector(new GravitationalEffector(500.0f));


	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_GRAVE)) && (IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;
			body.position = GetMousePosition();
			float angle = GetRandomFloat() * 2 * PI;
			Vector2 dir;
			dir.x = cosf(angle);
			dir.y = sinf(angle);
			//body.velocity = dir * (GetRandomFloat() * 500 + 50);
			//body.AddForce(dir * (GetRandomFloat() * 500 + 50), VelocityChange);


			body.acceleration = { 0, 0 };
			body.size = GetRandomValue(20, 25);

			body.restitution = GetRandomFloat() * 0.05f + .95f;
			body.mass = body.size;
			body.inverseMass = (body.body == Static) ? 0 : 1.0f / body.mass;
			body.gravityScale = GetRandomFloat() * 0.5f * body.mass + 0.5f;
			body.damping = GetRandomFloat() * 0.01f + 0.99f;


			world.AddBody(body);

		}

		timeAccum += dt;

		while (timeAccum > fixedTimeStep)
		{
			world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}

	

		DrawCircleV(GetMousePosition(), 10, RED);

		//DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		std::string fpstext = "FPS: ";

		fpstext += std::to_string(GetFPS());

		world.Draw();
		DrawText(fpstext.c_str(), 10, 10, 20, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
