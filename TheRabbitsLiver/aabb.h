#pragma once

#include <glm/glm.hpp>

struct AABB
{
	AABB(const glm::vec3& min, const glm::vec3& max);

	void UpdateMinMax(const glm::vec3& point);
	void Rotate(float angle);
	bool Contains(const glm::vec3& point) const;
	float MinDist2(const glm::vec3& point) const;

	glm::vec3 mMin;
	glm::vec3 mMax;
};

bool Intersects(const AABB& a, const AABB& b);
bool Intersects(const AABB& a, class Plane* plane);