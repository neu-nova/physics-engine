/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include "World.h"
#include "Random.h"
#include "GravitationalEffector.h"
#include "PointEffector.h"
#include "AreaEffector.h"
#include "DragEffector.h"
#include "Spring.h"
#include "world_camera.h"

#include <string>

#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4576)
#include "gui_physics.h"
#pragma warning(pop)

GuiPhysicsState state;

void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);

int main ()
{

	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	state = InitGuiPhysics();

	GuiLoadStyle("raygui/styles/dark/style.rgs");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	SetTargetFPS(60);

	World world;
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);

	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;


	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;


	/*
	world_camera.Begin()
	world.Draw()
	*/
	
	DrawCircleLinesV({ GetMousePosition().x, GetMousePosition().y }, 10, RED);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsKeyPressed(KEY_SPACE))
		{
			state.SimulateActive = !state.SimulateActive;
		}
		if (IsKeyPressed(KEY_TAB))
		{
			state.PhysicsPanelActive = !state.PhysicsPanelActive;
		}

		World::SetGravity({ 0, state.GravityValue });

		if (IsKeyDown(KEY_C)) world.GetBodies().clear();

		bool mouseOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(GetMousePosition(), Rectangle{ state.anchor02.x, state.anchor02.y });

		if (!mouseOverGui)
		{
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_GRAVE)) && (IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					AddEffector(world, world_camera);
				}
				else
				{
					AddBody(world, world_camera);
				}

			}

			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
			}

			if (selectedBody && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL))
			{
				Vector2 position = world_camera.ScreenToWorld(GetMousePosition()); //screentoworld
				Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 3.0f);
				selectedBody->AddForce(force);

				DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), GREEN); //worldtoscreen
			}

		}


		if (state.SimulateActive) {
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}

	

		DrawCircleV(GetMousePosition(), 10, RED);

		//DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		std::string fpstext = "FPS: ";


		fpstext += std::to_string(GetFPS());

		DrawText(fpstext.c_str(), 10, 10, 20, WHITE);

		world_camera.Begin();
		world.Draw();
		world_camera.End();

		if (selectedBody)
		{
			DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05, RED	);
		}


		GuiPhysics(&state);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;
	body.position = camera.ScreenToWorld(GetMousePosition());
	body.body = (BodyType)state.BodyTypeActive;
	float angle = GetRandomFloat() * 2 * PI;
	Vector2 dir;
	dir.x = cosf(angle);
	dir.y = sinf(angle);
	body.velocity = Vector2Scale(dir, state.BodyVelocityValue);

	
	body.acceleration = { 0, 0 };

	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;
	body.mass = state.BodyMassValue;
	body.inverseMass = (body.body == Static) ? 0 : 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;


	world.AddBody(body);
}

void AddEffector(World& world, WorldCamera& camera)
{
	Vector2 position = camera.ScreenToWorld(GetMousePosition());
	float size = state.EffectorSizeValue;
	switch (state.EffectorTypeActive)
	{
	case 0:
		world.AddEffector(new GravitationalEffector(position, size, state.EffectorForceValue));
		break;
	case 1:
		world.AddEffector(new PointEffector(position, size, state.EffectorForceValue));
		break;
	case 2:
		world.AddEffector(new AreaEffector(position, size, state.EffectorAngleValue, state.EffectorForceValue));
		break;
	case 3:
		world.AddEffector(new DragEffector(position, size, state.EffectorForceValue));
		break;
	default:
		break;
	}
}