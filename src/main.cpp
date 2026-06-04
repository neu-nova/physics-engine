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
#include <array>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4576)
#include "gui_physics.h"
#pragma warning(pop)
#include "miniaudio.h"

static ma_sound g_loopingSound;
static bool g_loopingSoundInited = false;

GuiPhysicsState state;

void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);
Body CreatePlayer(World& world, WorldCamera& camera);
bool CheckWinState(Vector2 goalposition, float goalradius, World& world, WorldCamera& camera);
void MakeShot(World& world, Body& playbody, Vector2 mouseposition, WorldCamera& camera);
void AddMarble(World& world, WorldCamera& camera, int type, Vector2 position);
bool PlayLoopingSound(ma_engine* engine, const char* fileName);
void StopAndUninitLoopingSound();

int main ()
{
	ma_engine engine;

	if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
		std::cerr << "Failed to initialize audio engine.\n";
		return -1;
	}
	PlayLoopingSound(&engine, "play.mp3");

	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	int width = 1280;
	int height = 800;
	// Create the window and OpenGL context
	InitWindow(width, height, "Hello Raylib");

	state = InitGuiPhysics();

	GuiLoadStyle("raygui/styles/dark/style.rgs");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	SetTargetFPS(60);

	World worldtest;
	World title;
	World lvlone;
	World lvltwo;
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);



	worldtest.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	title.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	lvlone.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	lvltwo.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;


	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;

	Body* playbody = nullptr;
	bool makeplayer = false;

	/*
	world_camera.Begin()
	world.Draw()
	*/

	World* currentworld = &title;
	bool test = false;
	int shots = 3;
	float timer = 10.0f;
	bool titleb = true;
	int currentlvl = 0;
	bool newworld = false;
	bool playvalid = false;
	bool win = false;
	bool lose = false;
	bool credits = true;

	Vector2 goalposition = { 4.0f, 0.0f };
	float goalradius = 3.0f;
	
	DrawCircleLinesV({ GetMousePosition().x, GetMousePosition().y }, 10, RED);

	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		//play game music, change credits
		float dt = GetFrameTime();

		if (IsKeyPressed(KEY_SPACE) && test)
		{
			state.SimulateActive = !state.SimulateActive;
		}
		if (IsKeyPressed(KEY_TAB) && test)
		{
			state.PhysicsPanelActive = !state.PhysicsPanelActive;
		}
		if (IsKeyPressed(KEY_SPACE) && (titleb || win)) // next level
		{
			titleb = false;
			win = false;
			currentlvl++;
			newworld = true;
			if (currentlvl == 0)
			{
				currentlvl = 0;
				titleb = true;
			}
		}
		if (IsKeyPressed(KEY_R) && !titleb) // restart level
		{
			newworld = true;
			win = false;
			lose = false;
		}
		
		if (IsKeyPressed(KEY_ZERO))
		{
			currentlvl = 0;
			titleb = true;
			win = false;
		}
		if (IsKeyPressed(KEY_ONE))
		{
			currentlvl = 1;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_TWO))
		{
			currentlvl = 2;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_THREE))
		{
			currentlvl = 3;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_FOUR))
		{
			currentlvl = 4;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_FIVE))
		{
			currentlvl = 5;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_SIX))
		{
			currentlvl = 6;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_SEVEN))
		{
			currentlvl = 7;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_EIGHT))
		{
			currentlvl = 8;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if (IsKeyPressed(KEY_NINE))
		{
			currentlvl = 9;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}
		if(IsKeyPressed(KEY_ZERO))
		{
			currentlvl = 10;
			newworld = true;
			titleb = false;
			win = false;
			lose = false;
		}

		if (newworld)
		{
			makeplayer = true;
			currentworld->GetBodies().clear();
			Body newPlayer = CreatePlayer(*currentworld, world_camera);
			currentworld->AddBody(newPlayer);
			playbody = &currentworld->GetBodies().back();

			switch (currentlvl)
			{
			case 0:
				break;
			case 1:
				shots = 1;
				AddMarble(*currentworld, world_camera, 1, { 0,0 });
				goalposition = { 4.0f, 0.0f };
				goalradius = 3.0f;
				break;
			case 2:
				shots = 1;
				AddMarble(*currentworld, world_camera, 2, { 1.0f, 0.0f });
				goalposition = { 2.0f, 0.0f };
				goalradius = 2.0f;
				break;
			case 3:
				shots = 2;
				AddMarble(*currentworld, world_camera, 1, { -1.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 2, { 1.0f, 0.0f });
				goalposition = { 4.0f, 0.0f };
				goalradius = 3.0f;
				break;
			case 4:
				shots = 3;
				AddMarble(*currentworld, world_camera, 1, { -1.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 2, { 0.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 1, { 1.0f, 0.0f });
				goalposition = { 4.0f, 0.0f };
				goalradius = 3.0f;
				break;
			case 5:
				shots = 2;
				AddMarble(*currentworld, world_camera, 1, { -1.0f, 0.0f });
				goalposition = { 6.0f, 5.0f };
				goalradius = 2.0f;
				break;
			case 6:
				shots = 5;
				AddMarble(*currentworld, world_camera, 1, { -1.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 2, { 4.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 1, { 1.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 1, { 3.0f, 0.0f });
				goalposition = { 4.0f, 0.0f };
				goalradius = 2.0f;
				break;
			case 7:
				shots = 3;
				AddMarble(*currentworld, world_camera, 2, { -1.0f, 0.0f });
				goalposition = { -2.0f, -3.0f };
				goalradius = 11.0f;
				break;
			case 8:
				shots = 1;
				AddMarble(*currentworld, world_camera, 1, { -1.0f, 0.0f });
				goalposition = { 4.0f, 0.0f };
				goalradius = 1.0f;
				break;
			case 9:
				shots = 3;
				AddMarble(*currentworld, world_camera, 2, { -1.0f, 1.0f });
				AddMarble(*currentworld, world_camera, 2, { -1.0f, 0.0f });
				AddMarble(*currentworld, world_camera, 2, { -1.0f, -1.0f });
				goalposition = { -4.0f, 0.0f };
				goalradius = 10.0f;
				break;
			case 10:
				shots = 3;
				AddMarble(*currentworld, world_camera, 1, { -1.0f, -1.0f });
				AddMarble(*currentworld, world_camera, 2, { 0.0f, 1.0f });
				AddMarble(*currentworld, world_camera, 2, { 0.0f, -1.0f });
				AddMarble(*currentworld, world_camera, 1, { -1.0f, 1.0f });
				goalposition = { 7.0f, 0.0f };
				goalradius = 5.0f;
				break;
			case 11:
				win = true;
				break;
			}
			playvalid = true;

			newworld = false;

			if ((currentlvl == 11 && !credits) || (currentlvl != 11 && credits))
			{
				StopAndUninitLoopingSound();
				{
					switch (currentlvl)
					{
					case 0:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 1:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 2:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 3:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 4:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 5:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 6:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 7:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 8:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 9:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 10:
						PlayLoopingSound(&engine, "play.mp3");
						credits = false;
						break;
					case 11:
						PlayLoopingSound(&engine, "victory.mp3");
						currentlvl = -1;
						credits = true;
						break;
					default:
						PlayLoopingSound(&engine, "play.mp3");
						break;
					}
				}
			}
			
		}

		if (playvalid && playbody && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			MakeShot(*currentworld, *playbody, world_camera.ScreenToWorld(GetMousePosition()), world_camera);
			shots--;
			//shoot sound
			ma_engine_play_sound(&engine, "hit.mp3", NULL);
			playvalid = false;
			timer = 7.0f;
		}

		if (!playvalid)
		{
			timer -= dt;
			if (timer <= 0 && !(win || lose))
			{
				if (CheckWinState(goalposition, goalradius, *currentworld, world_camera))
				{
					//winsound
					ma_engine_play_sound(&engine, "win.mp3", NULL);
					win = true;
				}
				else if (shots > 0)
				{

					Body newPlayer = CreatePlayer(*currentworld, world_camera);
					currentworld->AddBody(newPlayer);
					playbody = &currentworld->GetBodies().back();
					playvalid = true;
				}
				else
				{
					//losesound
					ma_engine_play_sound(&engine, "lose.mp3", NULL);
					lose = true;
				}

			}
		}

		World::SetGravity({ 0, state.GravityValue });
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
				currentworld->Step(fixedTimeStep, &engine);
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

		std::string leveltext = "LEVEL: ";
		leveltext += std::to_string(currentlvl);

		std::string livestext = "SHOTS REMAINING: ";
		livestext += std::to_string(shots);

		DrawText(fpstext.c_str(), 10, 10, 20, WHITE);
		if (currentlvl > 0)
		{
			DrawText(leveltext.c_str(), 10, 40, 20, WHITE);
			DrawText(livestext.c_str(), 10, 70, 20, WHITE);
		}

		if (win)
		{
			if (currentlvl == -1)
			{
				DrawText("Game Complete!", width / 2 - MeasureText("Game Complete!", 120) / 2, height / 2 - 100, 120, WHITE);
				DrawText("Press Space to Replay!", width / 2 - MeasureText("Press Space to Replay!", 40) / 2, height / 2 + 20, 40, WHITE);
			}
			else {
				DrawText("Level Complete!", width / 2 - MeasureText("Level Complete!", 120) / 2, height / 2 - 100, 120, WHITE);
				DrawText("Press Space to Continue!", width / 2 - MeasureText("Press Space to Continue!", 40) / 2, height / 2 + 20, 40, WHITE);
			}
		}

		if (lose)
		{
			DrawText("Level Failed!", width / 2 - MeasureText("Level Failed!", 120) / 2, height / 2 - 100, 120, WHITE);
			DrawText("Press R to restart!", width / 2 - MeasureText("Press R to restart!", 40) / 2, height / 2 + 20, 40, WHITE);
		}

		switch(currentlvl)
		{
			case 0:
				break;
			case 1:
				DrawText("Shoot the green marble by positioning your mouse behind the marble!", 10, height - 140, 30, WHITE);
				DrawText("Get all blue marbles into the goal!", 10, height  - 100, 30, WHITE);
				DrawText("The position of green marbles don't matter!", 10, height - 60, 30, WHITE);
				break;
			case 2:
				DrawText("Get all red marbles out of the goal!", 10, height  - 100, 30, WHITE);
				break;
			case 3:
				DrawText("Some levels can have both types of marbles!", 10, height - 100, 30, WHITE);
				break;
		}

		
		
		if (titleb)
		{
			DrawText("Marble Mayhem", width/2 - MeasureText("Marble Mayhem", 120) / 2, height/2 - 100, 120, WHITE);
			DrawText("PRESS SPACE TO START", width/2 - MeasureText("PRESS SPACE TO START", 40) / 2, height/2 + 20, 40, WHITE);
		}
		else
		{
			DrawCircleLinesV(world_camera.WorldToScreen(goalposition), goalradius* world_camera.GetPixelsPerUnit(), GREEN);
			world_camera.Begin();
			currentworld->Draw();
			world_camera.End();
		}


		if (playvalid && playbody)
		{
			DrawLineV(world_camera.WorldToScreen(playbody->position), GetMousePosition(), WHITE);
		}

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

	ma_engine_uninit(&engine);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

bool PlayLoopingSound(ma_engine* engine, const char* filePath)
{
	// Use stream for music; use MA_SOUND_FLAG_DECODE for short effects if you prefer fully decoded audio.
	ma_result r = ma_sound_init_from_file(engine, filePath, MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL, &g_loopingSound);
	if (r != MA_SUCCESS) return false;

	ma_sound_set_looping(&g_loopingSound, MA_TRUE);
	ma_sound_start(&g_loopingSound);

	g_loopingSoundInited = true;
	return true;
}

void StopAndUninitLoopingSound()
{
	if (!g_loopingSoundInited) return;
	ma_sound_stop(&g_loopingSound);
	ma_sound_uninit(&g_loopingSound);
	g_loopingSoundInited = false;
}

void AddMarble(World& world, WorldCamera& camera, int type, Vector2 position)
{
	Body body;
	body.position = position;
	body.body = Dynamic;
	body.velocity = { 0, 0 };
	body.acceleration = { 0, 0 };
	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;
	body.mass = state.BodyMassValue;
	body.inverseMass = 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;
	switch (type)
	{
		case 0:
			body.color = ColorFromHSV(120, 1, 1);
			body.balltype = NONE;
			break;
		case 1:
			body.color = ColorFromHSV(240, 1, 1);
			body.balltype = IN;
			break;
		case 2:
			body.color = ColorFromHSV(360, 1, 1);
			body.balltype = OUT;
			break;
	}
	world.AddBody(body);
}

void MakeShot(World& world, Body& playbody, Vector2 mouseposition, WorldCamera& camera)
{
	Vector2 dir = Vector2Subtract(mouseposition, playbody.position);
	float distance = Vector2Length(dir);
	dir = Vector2Normalize(dir);
	playbody.AddForce(Vector2Scale(dir, distance)* (-200));
}

bool CheckWinState(Vector2 goalposition, float goalradius, World& world, WorldCamera& camera)
{
	std::vector<Body>& bodies = world.GetBodies();
	for (int i = 0; i < bodies.size(); i++)
	{
		Body& body = bodies[i];
		switch (body.balltype)
		{
			case NONE:
				break;
			case IN:
			{
				float distanceSqr = Vector2DistanceSqr(body.position, goalposition);
				if (!(distanceSqr <= (goalradius * goalradius)))
				{
					return false;
				}
				break;
			}
			case OUT:
			{
				float distanceSqr1 = Vector2DistanceSqr(body.position, goalposition);
				if (distanceSqr1 <= (goalradius * goalradius))
				{
					return false;
				}
				break;
			}
		}
	}
	
	return true;
}

Body CreatePlayer(World& world, WorldCamera& camera)
{
	Body body;
	//spawn players  in the centerleft of the screen
	body.position = camera.ScreenToWorld({ 300, GetScreenHeight() / 2.0f });
	body.body = Dynamic;
	body.velocity = { 0, 0 };
	body.acceleration = { 0, 0 };
	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;
	body.mass = state.BodyMassValue;
	body.inverseMass = 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;
	body.color = ColorFromHSV(120, 1, 1);
	body.balltype = NONE;
	return body;
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