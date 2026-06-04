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

	World worldtest;
	World title;
	World lvlone;
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);

	worldtest.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	title.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	lvlone.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;


	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;


	/*
	world_camera.Begin()
	world.Draw()
	*/

	World* currentworld = &worldtest;
	bool test = false;
	int lives = 3;
	float timer = 10.0f;
	bool titleb = true;
	
	DrawCircleLinesV({ GetMousePosition().x, GetMousePosition().y }, 10, RED);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsKeyPressed(KEY_SPACE) && test)
		{
			state.SimulateActive = !state.SimulateActive;
		}
		if (IsKeyPressed(KEY_TAB) && test)
		{
			state.PhysicsPanelActive = !state.PhysicsPanelActive;
		}

		World::SetGravity({ 0, state.GravityValue });

		if (IsKeyDown(KEY_C)) currentworld->GetBodies().clear();

		bool mouseOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(GetMousePosition(), Rectangle{ state.anchor02.x, state.anchor02.y, 304, 664 });

		if (!mouseOverGui && test)
		{
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_GRAVE)) && (IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					AddEffector(*currentworld, world_camera);
				}
				else
				{
					AddBody(*currentworld, world_camera);
				}

			}
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				selectedBody = currentworld->GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
			}


			// spring
			if (selectedBody)
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
				{
					Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
					if (IsKeyDown(KEY_LEFT_CONTROL))
					{
						Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 3.0f);
						selectedBody->AddForce(force);
					}
					else
					{
						connectedBody = currentworld->GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
					}
					DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), WHITE);
				}
				else
				{
					if (selectedBody && connectedBody)
					{
						float distance = Vector2Distance(selectedBody->position, connectedBody->position);
						if (state.SpringAutoLengthChecked)
						{
							currentworld->AddSpring(*selectedBody, *connectedBody, distance, state.SpringStiffnessValue, state.SpringDampingValue);
						}
						else
						{
							currentworld->AddSpring(*selectedBody, *connectedBody, state.SpringLengthValue, state.SpringStiffnessValue, state.SpringDampingValue);
						}
					}

					selectedBody = nullptr;
					connectedBody = nullptr;
				}
			}


		}


		if (state.SimulateActive) {
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				currentworld->Step(fixedTimeStep);
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

		std::string livestext = "LIVES: ";
		livestext += std::to_string(lives);

		DrawText(fpstext.c_str(), 10, 10, 20, WHITE);
		DrawText(livestext.c_str(), 10, 40, 20, WHITE);

		world_camera.Begin();
		if (titleb)
		{
			DrawText("Marble Mayhem", world_camera.WorldToScreen({ -1.0f, 0.0f }).x, world_camera.WorldToScreen({ -1.0f, 0.0f }).y, 50, WHITE);
			DrawText("PRESS SPACE TO START", world_camera.WorldToScreen({ -1.0f, -1.0f }).x, world_camera.WorldToScreen({ -1.0f, -1.0f }).y, 20, WHITE);
		}
		else
		{
			currentworld->Draw();
		}
		world_camera.End();

		if (selectedBody && test)
		{
			DrawCircleLinesV(world_camera.WorldToScreen(selectedBody->position), selectedBody->size * 1.05 * world_camera.GetPixelsPerUnit(), RED);
		}

		if (connectedBody && test)
		{
			DrawCircleLinesV(world_camera.WorldToScreen(connectedBody->position), connectedBody->size * 1.05 * world_camera.GetPixelsPerUnit(), RED);
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
	body.color = ColorFromHSV(GetRandomFloat(360), 1, 1);


	world.AddBody(body);
}

void AddEffector(World& world, WorldCamera& camera)
{
	Vector2 position = camera.ScreenToWorld(GetMousePosition());
	float size = state.BodySizeValue *0.5;
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