#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "PlayerActor.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"

const float cAnimationSpeed = 0.5f;

PlayerActor::PlayerActor()
	:mbWait(true)
	,mAnimState(AnimationType::PLAYERANIM_IDLE)
{
	//メッシュのロード
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(RENDERER->GetMesh("Assets/SK_Mannequin.gpmesh"));
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/SK_Mannequin.gpskel"));
	
	//アニメーションの取得&アニメーション配列にセット
	mAnimTypes.resize(AnimationType::PLAYERANIM_ITEMNUM);
	mAnimTypes[PLAYERANIM_IDLE] = RENDERER->GetAnimation("Assets/ThirdPersonIdle.gpanim");
	mAnimTypes[PLAYERANIM_RUN]  = RENDERER->GetAnimation("Assets/ThirdPersonRun.gpanim");

	// アイドル状態アニメーションをセット
	mMeshComp->PlayAnimation( mAnimTypes[PLAYERANIM_IDLE], cAnimationSpeed);

	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
}

PlayerActor::~PlayerActor()
{
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerActor::UpdateActor(float deltaTime)
{

	//キャラ状態遷移
	StateChange();

	//キャラ入力
	const float speed = 200.0f;
	Vector3 forwardVec = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 rightVec   = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = GetForward();

	// キャラクター移動
	Vector3 DirVec(0.0f, 0.0f, 0.0f);
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
	{
		DirVec += forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
	{
		DirVec -= forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
	{
		DirVec += rightVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
	{
		DirVec -= rightVec;
	}

	// 進行方向進むベクトルが小さいならキーは押されていない。以下の移動処理をキャンセル
	if (DirVec.LengthSq() < 0.1f)
	{
		return;
	}
	
	// 方向キー入力
	charaForwardVec = DirVec ;
	
	// 進行方向に向けて回転
	charaForwardVec.Normalize();
	RotateToNewForward(charaForwardVec);

	// 進行方向に移動
	mPosition += speed * deltaTime * DirVec;
}

// キャラ状態遷移
void PlayerActor::StateChange()
{
	// キー入力からアニメーション状態へ移行
	bool IsRunStart  = INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_UP) | 
		               INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_RIGHT) |
		               INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_DOWN) |
		               INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_LEFT);
	bool IsIdleStart = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
		               INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
		               INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) & 
		               INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);

	// 待機アニメ開始
	if (IsIdleStart && (mAnimState != PLAYERANIM_IDLE))
	{
		mMeshComp->PlayAnimation(mAnimTypes[PLAYERANIM_IDLE], cAnimationSpeed);
		mAnimState = PLAYERANIM_IDLE;
		return;
	}

	// RUNアニメ開始
	if (IsRunStart && (mAnimState != PLAYERANIM_RUN))
	{
		mMeshComp->PlayAnimation(mAnimTypes[PLAYERANIM_RUN], cAnimationSpeed);
		mAnimState = PLAYERANIM_RUN;
		return;
	}

}
