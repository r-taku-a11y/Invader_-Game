#pragma once
#include <DxLib.h>
#include <string>
#include "Scene/Title.h"
#include "Scene/GameSenen.h"
#include "Scene/GameClear.h"
#include "Scene/GameOver.h"
#include "Manager/SceneManager.h"

class Application
{
public:
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;

	// スクリーンサイズの半分
	static constexpr float HALF_SCREEN_SIZE_X = static_cast<float>(SCREEN_SIZE_X / 2.0f);
	static constexpr float HALF_SCREEN_SIZE_Y = static_cast<float>(SCREEN_SIZE_Y / 2.0f);

	// フレームレート
	static constexpr float FRAME_RATE = (1000 / 30);

	// データパス関連
	static const std::string PATH_MODEL;
	static const std::string PATH_IMAGE;
	static const std::string PATH_SOUND; 
	static const std::string PATH_EFFECT;

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD_TO_DEG = (180.0f / DX_PI_F);
	static constexpr float DEG_TO_RAD = (DX_PI_F / 180.0f);

	// 文字の色
	static constexpr int GLAY = 128;	// グレー
	static constexpr int WHITE = 255;	// 白

	// 明示的にインスタンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// 解放
	void Release(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void)const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void)const;

	// ゲッター
	float GetDeltaTime() const;

private:

	SceneManager::SCENE_ID oldScene_;

	// クラス
	Title title_;
	GameSenen gamesenen;
	GameClear gameclear;
	GameOver gameover;

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application&);

	// デストラクタも同様
	~Application(void);

	// フレームレート関連
	int currentTime;
	int lastFrameTime;
	int frameCnt;
	int updateFrameRateTime;
	float frameRate;

	// 前フレームからの経過時間(秒)
	float deltaTime_;

};