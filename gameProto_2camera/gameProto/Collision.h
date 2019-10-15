#pragma once
#include "game.h"

// 球体
struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3 mCenter;
	float mRadius;
};

// 軸並行ボックス(Axis-Aligned Bounding Box)
struct AABB
{
	AABB();
	AABB(const Vector3& min, const Vector3& max);

	// モデル読み込みの時の最大の点を求めるのに使用
	void InitMinMax(const Vector3& point) { mMin = mMax = point; }
	void UpdateMinMax(const Vector3& point);
	bool Contains(const Vector3& point) const;
	float MinDistSq(const Vector3& point) const;
	void Rotate(const Quaternion& q);
	void SetArrowRotate(bool value) { mIsRotatable = value; }

	Vector3 mMin;
	Vector3 mMax;
	bool	mIsRotatable;
};

bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Sphere& s, const AABB& box);

void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec);