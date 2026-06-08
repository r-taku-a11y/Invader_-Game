#include <DxLib.h>
#include "Application.h"
#include "Manager/InputManager.h"
#include "Manager/SoundManager.h"

// インスタンスの生成
Application* Application::instance_ = nullptr;

// 明示的にインスタンスを生成する
void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

// 静的インスタンスの取得
Application& Application::GetInstance(void)
{
	return *instance_;
}

// 初期化
void Application::Init(void)
{
	// サウンドマネージャー生成
	SoundManager::CreateInstance();

	// アプリケーションの初期設定
	SetWindowText("Space Invader");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);

	// ウィンドウモードの設定
	// true : ウィンドウ、false : フルスクリーン
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;

	// フレームレート
	deltaTime_ = 1.0f / 60.0f;

	//SetUsePixelLighting(true);

	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	title_.Init();
	gameclear.Init();
	gameover.Init();

	SetDrawScreen(DX_SCREEN_BACK);

	// キー制御初期化
	SetUseDirectInputFlag(true);

	oldScene_ = SceneManager::SCENE_ID::TITLE;

	// フレームレート関連
	currentTime = 0;
	lastFrameTime = 0;
	frameCnt = 0;
	updateFrameRateTime = 0;
	frameRate = 0.0f;
}

// ゲームループの開始
void Application::Run(void)
{

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// CPU負荷軽減
		Sleep(1);

		// 現在時刻取得
		currentTime = GetNowCount();

		// 前フレームからの経過時間(ms)
		int elapsed = currentTime - lastFrameTime;

		// 経過時間を秒へ変換
		deltaTime_ = elapsed / 1000.0f;

		// 現在時刻保存
		lastFrameTime = currentTime;

			InputManager::GetInstance().Update();

			// 画面を初期化
			ClearDrawScreen();

			SceneManager::SCENE_ID currentScene =SceneManager::GetInstance().GetScene();


			// シーン変更検知
			if (oldScene_ != currentScene)
			{
				// 前のシーンを先に解放する
				switch (oldScene_)
				{
				case SceneManager::SCENE_ID::TITLE:
					title_.Release();
					break;

				case SceneManager::SCENE_ID::GAMESEMEN:
					gamesenen.Release();
					break;

				case SceneManager::SCENE_ID::GAMECLEAR:
					gameclear.Release();
					break;

				case SceneManager::SCENE_ID::GAMEOVER:
					gameover.Release();
					break;
				}


				switch (currentScene)
				{
				case SceneManager::SCENE_ID::TITLE:
					title_.Init();
					break;

				case SceneManager::SCENE_ID::GAMESEMEN:
					gamesenen.Init();
					break;

				case SceneManager::SCENE_ID::GAMECLEAR:
					gameclear.Init();
					break;

				case SceneManager::SCENE_ID::GAMEOVER:
					gameover.Init();
					break;
				}

				oldScene_ = currentScene;
			}

			// シーンの更新・描画
			switch (SceneManager::GetInstance().GetScene())
			{
			// タイトル
			case SceneManager::SCENE_ID::TITLE:

				title_.Update();

				// タイトルからゲーム開始
				if (title_.IsStartGame())
				{

					title_.Release();

					// シーン切り替え
					SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMESEMEN);
				}
				title_.Draw();
				break;
			// ゲームシーン
			case SceneManager::SCENE_ID::GAMESEMEN:

				gamesenen.Update();
				gamesenen.Draw();

				break;

			// ゲームクリア
			case SceneManager::SCENE_ID::GAMECLEAR:

				gameclear.Update();
				gameclear.Draw();

				if (oldScene_ == SceneManager::SCENE_ID::GAMECLEAR &&currentScene != SceneManager::SCENE_ID::GAMECLEAR)
				{
					gameclear.Release();
				}
				break;

				// ゲームオーバー
			case SceneManager::SCENE_ID::GAMEOVER:

				gameover.Update();
				gameover.Draw();
				if (oldScene_ == SceneManager::SCENE_ID::GAMEOVER &&currentScene != SceneManager::SCENE_ID::GAMEOVER)
				{
					gameover.Release();
				}
				break;

			default:
				break;
			}

			ScreenFlip();
	}
}

// 解放
void Application::Release(void)
{
	// タイトル画面の開放
	title_.Release();
	// ゲームシーンの開放
	gamesenen.Release();

	// サウンドマネージャーの開放
	SoundManager::DeleteInstance();

	// 敵の開放
	Enemy::ReleaseResource();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}
}

// 初期化成功／失敗の判定
bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

// 解放成功／失敗の判定
bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

float Application::GetDeltaTime() const
{
	return deltaTime_;
}

// デフォルトコンストラクタをprivateにして、
// 外部から生成できない様にする
Application::Application(void)
{
	// フラグの初期化
	isInitFail_ = false;
	isReleaseFail_ = false;

	// フレームレート関連
	currentTime = 0;
	lastFrameTime = 0;
	frameCnt = 0;
	updateFrameRateTime = 0;
	frameRate = 0.0f;
}

// デストラクタも同様
Application::~Application(void)
{
	// インスタンスの解放
	delete instance_;
}