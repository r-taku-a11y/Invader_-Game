#pragma once
#include <DxLib.h>
#include "../Manager/NetworkManager.h"


class Title
{
public:
	Title(void);			// コンストラクタ
	~Title(void);			// デストラクタ

	// 初期処理
	void Init(void);		// 初期化
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放

	// ゲーム開始判定の取得
	bool IsStartGame()const;

private:

	// ネットワーク管理
	NetworkManager& network = NetworkManager::GetInstance();

	// 前フレームのボタン状態
	bool oldButton_;

	// ゲーム開始フラグ
	bool StartGame;

	// 点滅用のタイマー
	int BlinkTimer;

	// PRESS ENTER表示フラグ
	bool ShowPressKey;

	// シーン遷移待ちフラグ
	bool changeSceneWait_;

	// フォントハンドル
	int titleFont_;
	int menuFont_;

	// フォント
	static constexpr int TITLE_FONT_SIZE = 64;
	static constexpr int TITLE_FONT_THICK = 4;

	static constexpr int MENU_FONT_SIZE = 28;
	static constexpr int MENU_FONT_THICK = 2;

	// 点滅間隔
	static constexpr int BLINK_INTERVAL = 30;

	// 音量
	static constexpr int BGM_VOLUME = 120;
	static constexpr int CLICK_VOLUME = 200;

	// タイトル表示位置
	static constexpr int TITLE_Y = 150;
	static constexpr int SUBTITLE_Y = 230;
	static constexpr int PRESS_BUTTON_Y = 330;
	static constexpr int START_TEXT_Y = 430;
	static constexpr int EXIT_TEXT_Y = 470;
};

