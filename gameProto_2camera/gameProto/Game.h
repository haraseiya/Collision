#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_types.h>
#include "math.h"


class Game
{
private:
	Game();   // シングルトン

	void Input();                                  // 入力処理
	void Draw();                                   // 描画
	int  Update();                                 // フレーム更新処理
	void ActorUpdate();                            // アクターアップデート
	void ShowActor();                              // アクターリスト表示（デバッグ用）
								                  
	class Renderer*     mRenderer;                 // レンダリングエンジン
	class SceneBase*    mNowScene;                 // ベースシーンクラス
	class CameraActor*  mActiveCamera;             // 現時点のカメラアクター

	bool  mIsRunning;                              // ゲームループ回すか？
	bool  mIsPauseMode;                            // ポーズモード


	std::vector<class Actor*> mActors;             // アクター配列
	std::vector<class Actor*> mPendingActors;      // アクター追加準備用配列

	float    mDeltaTime;                             // 1フレームの経過時間（秒単位）
	Uint32   mTicksCount;                            // 経過時間（ミリ秒単位）
	Matrix4  mViewMatrix;

public:
	~Game();                                     
	static Game& GetInstance()                     // インスタンス
	{
		static Game instance;
		return instance;
	}

	void Run();                                                                    // ゲーム本体実行
	void Shutdown();                                                               // 終了時
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);   // 画面初期化設定

	void setFirstScene(SceneBase* pScene) { mNowScene = pScene; }                  // 開始シーンのセット
	void setShutDown() { mIsRunning = false; }                                     // シャットダウン

	class Renderer* GetRenderer() { return mRenderer; }                            // レンダラーシステムの取得
	SDL_Renderer* GetSDLRenderer();                                                // 2DのSDLレンダラーの取得

	void AddActor(class Actor* actor);                                             // アクターの追加
	void RemoveActor(class Actor* actor);                                          // アクターの削除

	void SetCameraActor(class CameraActor* camera);
	void InActiveCamera(class CameraActor* inActiveCam);
	const Matrix4& GetViewMatrix(); //カメラ行列取得
	const Vector3& GetViewVector(); //視線ベクトル取得

};

#define GAMEINSTANCE Game::GetInstance()
#define RENDERER Game::GetInstance().GetRenderer()

