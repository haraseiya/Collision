#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	typedef enum AnimationType
	{
		PLAYERANIM_IDLE = 0,    // �ҋ@
		PLAYERANIM_RUN,         // ����

		PLAYERANIM_ITEMNUM,     // ���A�j���[�V������
	}AnimationType;

	PlayerActor();
	~PlayerActor();

	void UpdateActor(float deltaTime) override;

	void StateChange();

private:
	class SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	bool mbWait;  // �ҋ@��Ԃ��H

	AnimationType mAnimState;
};