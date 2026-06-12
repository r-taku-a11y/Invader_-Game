#include "Title.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Application.h"

// コンストラクタ
Title::Title(void)
{
}

// デストラクタ
Title::~Title(void)
{
}

// 初期化
void Title::Init(void)
{
	network.Init();

	// ゲーム開始フラグの初期化
	StartGame = false;

	// 点滅用のタイマーの初期化
	BlinkTimer = 0;

	// PRESS ENTER表示フラグ
	ShowPressKey = true;

	// フォント作成

	titleFont_ = CreateFontToHandle("Arial",64,4);

	menuFont_ = CreateFontToHandle("Arial",28,2);

	// ボタン状態初期化
	oldButton_ = false;
}

// 更新
void Title::Update(void)
{
	// 受信
	network.Update();

	BlinkTimer++;

	// 30フレームごとにPRESS ENTERの表示の切り替えさせる
	if (BlinkTimer >= 30)
	{
		ShowPressKey = !ShowPressKey;

		BlinkTimer = 0;
	}

	// 現在のボタン状態
	bool nowButton = network.GetButton();

	// 押した瞬間判定
	bool buttonTriggr = nowButton && !oldButton_;

	oldButton_ = nowButton;

	// マウスの左
	if (InputManager::GetInstance().IsTrgMouseLeft())
	{
		StartGame = true;
	}

	// ボタン
	if (buttonTriggr)
	{
		StartGame = true;

		// ボタンを離すまで待つ
		while (network.GetButton())
		{
			network.Update();
			ProcessMessage();
		}
	}
}

// 描画
void Title::Draw(void)
{
	// 画面中央座標

	int centerX = Application::SCREEN_SIZE_X / 2;

	// タイトル

	const char* title = "SPACE INVADER";

	int titleWidth =GetDrawStringWidthToHandle(title,strlen(title),titleFont_);

	DrawStringToHandle(centerX - titleWidth / 2,150,title,GetColor(0, 255, 0),titleFont_);

	// サブタイトル

	const char* subTitle = "DEFEND THE EARTH";

	int subWidth =GetDrawStringWidthToHandle(subTitle,strlen(subTitle),menuFont_);

	DrawStringToHandle(centerX - subWidth / 2,230,subTitle,GetColor(255, 255, 0),menuFont_);

	// PRESS ENTER

	if (ShowPressKey)
	{
		const char* pressText = "PRESS BUTTON";

		int pressWidth =GetDrawStringWidthToHandle(pressText,strlen(pressText),menuFont_);

		DrawStringToHandle(centerX - pressWidth / 2,330,pressText,GetColor(255, 255, 255),menuFont_);
	}

	// ゲーム開始

	const char* startText = "BUTTON / CLICK : GAME START";

	int startWidth =GetDrawStringWidthToHandle(startText,strlen(startText),menuFont_);

	DrawStringToHandle(centerX - startWidth / 2,430,startText,GetColor(255, 255, 255),menuFont_);

	// 終了

	const char* exitText = "ESC : EXIT";

	int exitWidth =GetDrawStringWidthToHandle(exitText,strlen(exitText),menuFont_);

	DrawStringToHandle(centerX - exitWidth / 2,470,exitText,GetColor(255, 255, 255),menuFont_);

	// デバック
	// DrawFormatString(10,10,GetColor(255, 255, 255),"BTN=%d",network.GetButton());
}

// 解放
void Title::Release(void)
{
	DeleteFontToHandle(titleFont_);
	DeleteFontToHandle(menuFont_);
}

// ゲーム開始判定の取得
bool Title::IsStartGame() const
{
	return StartGame;
}
