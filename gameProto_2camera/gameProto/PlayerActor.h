#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	typedef enum AnimationType
	{
		PLAYERANIM_IDLE = 0,    // 待機
		PLAYERANIM_RUN,         // 走る

		PLAYERANIM_ITEMNUM,     // 総アニメーション数
	}AnimationType;

	PlayerActor();
	~PlayerActor();

	void UpdateActor(float deltaTime) override;

	void StateChange();

private:
	class SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	bool mbWait;  // 待機状態か？

	AnimationType mAnimState;
};