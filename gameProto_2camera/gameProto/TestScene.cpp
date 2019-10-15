#include "game.h"
#include "Math.h"
#include "Renderer.h"
#include "TestScene.h"
#include "PlayerActor.h"
#include "DebugGrid.h"
#include "LookdownCamera.h"

TestScene::TestScene()
{ 

	PlayerActor *player = new PlayerActor;

	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection    = Vector3(0.0f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor    = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	LookDownCamera* c = new LookDownCamera(player);
	c->Init(Vector3(-300, 0, 300), Vector3::Zero, Vector3::UnitZ);

	Matrix4 proj;
}

TestScene::~TestScene()
{
}

SceneBase *TestScene::update()
{
	float radius = 160.0f;
	static float anim = 0.0f;
	anim += 0.01f;
	Matrix4 view;
	view = GAMEINSTANCE.GetViewMatrix();
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);

	return this;
}

void TestScene::draw()
{
	glClearColor(0.5f, 0.4f, 0.4f, 1.0f);
	//画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//ゲームシステム関連描画
	GAMEINSTANCE.GetRenderer()->Draw();
	mGrid->Draw();

	//画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}