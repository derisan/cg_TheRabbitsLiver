#pragma once

#include <random>
#include <vector>

#include <glm/vec3.hpp>

class Random
{
public:
	static void Init();

	// Returns 0.0f ~ 1.0f
	static float GetFloat();

	static float GetFloatRange(float min, float max);
	static int GetIntRange(int min, int max);
	static int GetChoice(int a, int b);
	static glm::vec3 GetVec3(float min, float max);

	// Return vector of int that excludes parameter 'ex'
	static std::vector<int> GetShuffledArray(int min, int max, int ex = 123456);

private:
	static std::mt19937 mEngine;
};

