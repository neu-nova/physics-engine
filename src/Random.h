#pragma once
#include "raylib.h"

float GetRandomFloat()
{
	return GetRandomValue(0, 10000) / (float)10000;
}

float GetRandomFloat(float max)
{
	return GetRandomFloat() * max;
}

float GetRandomFloat(float min, float max)
{
	if (min > max) return GetRandomFloat(max, min);
	return min + GetRandomFloat() * (max - min);
}