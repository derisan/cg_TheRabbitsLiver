#include "aabb.h"

#include <array>

#include <glm/gtx/rotate_vector.hpp>

#include "plane.h"
#include "box_component.h"

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	: mMin{ min },
	mMax{ max }
{

}

void AABB::UpdateMinMax(const glm::vec3& point)
{
	mMin.x = glm::min(mMin.x, point.x);
	mMin.y = glm::min(mMin.y, point.y);
	mMin.z = glm::min(mMin.z, point.z);

	mMax.x = glm::max(mMax.x, point.x);
	mMax.y = glm::max(mMax.y, point.y);
	mMax.z = glm::max(mMax.z, point.z);
}

void AABB::Rotate(float angle)
{
	std::array < glm::vec3, 8> points;

	points[0] = mMin;
	points[1] = glm::vec3{ mMax.x, mMin.y, mMin.z };
	points[2] = glm::vec3{ mMin.x, mMax.y, mMin.z };
	points[3] = glm::vec3{ mMin.x, mMin.y, mMax.z };

	points[4] = glm::vec3{ mMin.x, mMax.y, mMax.z };
	points[5] = glm::vec3{ mMax.x, mMin.y, mMax.z };
	points[6] = glm::vec3{ mMax.x, mMax.y, mMin.z };
	points[7] = mMax;

	auto p = glm::rotate(points[0], glm::radians(angle), glm::vec3{ 0.0f, 1.0f, 0.0f });
	mMin = p;
	mMax = p;

	for (size_t i = 1; i < points.size(); ++i)
	{
		p = glm::rotate(points[i], glm::radians(angle), glm::vec3{ 0.0f, 1.0f, 0.0f });
		UpdateMinMax(p);
	}
}

bool AABB::Contains(const glm::vec3& point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;

	// If none of these are true, the point is inside the box
	return !outside;
}

float AABB::MinDist2(const glm::vec3& point) const
{
	float dx = glm::max(mMin.x - point.x, 0.0f);
	dx = glm::max(dx, point.x - mMax.x);
	float dy = glm::max(mMin.y - point.y, 0.0f);
	dy = glm::max(dy, point.y - mMax.y);
	float dz = glm::max(mMin.z - point.z, 0.0f);
	dz = glm::max(dy, point.z - mMax.z);

	return dx * dx + dy * dy + dz * dz;
}

bool Intersects(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;

	// If none of these are true, they must intersect
	return !no;
}

bool Intersects(const AABB& a, Plane* plane)
{
	const auto& b = plane->GetBox()->GetWorldBox();

	bool res = (b.mMax.z > a.mMin.z) && (b.mMin.z < a.mMin.z) && (a.mMin.y <= b.mMax.y);

	return res;
}