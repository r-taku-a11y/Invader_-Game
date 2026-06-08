#pragma once
#include<DxLib.h>


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

	// ゲーム開始フラグ
	bool StartGame;

	// 点滅用のタイマー
	int BlinkTimer;

	// PRESS ENTER表示フラグ
	bool ShowPressKey;

	// フォントハンドル
	int titleFont_;
	int menuFont_;
};

