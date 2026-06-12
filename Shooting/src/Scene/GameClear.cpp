#include <DxLib.h>
#include "GameClear.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Application.h"

// コンストラクタ
GameClear::GameClear(void)
{
}

// デストラクタ
GameClear::~GameClear(void)
{
}

// 初期化
void GameClear::Init(void)
{
	// 最初は再プレイのほうを選択させておく
	selectMenu_ = PLAY_AGAIN;

	// フォント作成
	ClearFont_ = CreateFontToHandle("Arial", 48, 3);

	oldButton_ = false;
}

// 更新
void GameClear::Update(void)
{
	// Arduinoから送信された最新データを受信
	network.Update();

	InputManager& input = InputManager::GetInstance();

	// ジョイスティック値取得
	int stickX = network.GetStickX();

	// 左選択
	if (stickX < 300)
	{
		selectMenu_ = PLAY_AGAIN;
	}

	// 右選択
	if (stickX > 700)
	{
		selectMenu_ = RETURN_TITLE;
	}

	// キーボードでも操作可能
	if (input.IsTrgDown(KEY_INPUT_LEFT))
	{
		selectMenu_ = PLAY_AGAIN;
	}

	if (input.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectMenu_ = RETURN_TITLE;
	}

	// ボタンの現在状態の取得
	bool nowButton = network.GetButton();

	// ボタンを押した瞬間
	bool buttonTrigger = nowButton && !oldButton_;

	oldButton_ = nowButton;

	// 決定
	if (input.IsTrgMouseLeft() || buttonTrigger)
	{
		switch (selectMenu_)
		{
			// もう一度プレイ
		case PLAY_AGAIN:

			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMESEMEN);

			break;

			// タイトルへ戻る
		case RETURN_TITLE:

			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);

			break;
		}
	}

}

// 描画
void GameClear::Draw(void)
{
	// GAME CLEAR
	DrawStringToHandle(200,200,"GAME CLEAR !!",GetColor(255, 255, 0), ClearFont_);

	// PLAY AGAIN選択中
	if (selectMenu_ == PLAY_AGAIN)
	{
		DrawString(180,350,"> PLAY AGAIN",GetColor(255, 255, 255));

		DrawString(450,350,"RETURN TITLE",GetColor(128, 128, 128));
	}
	// RETURN TITLE選択中
	else
	{
		DrawString(180,350,"PLAY AGAIN",GetColor(128, 128, 128));

		DrawString(450,350,"> RETURN TITLE",GetColor(255, 255, 255));
	}

	// 操作説明
	DrawString(180,500,"LEFT/RIGHT : SELECT   ENTER : DECIDE",GetColor(255, 255, 255));

	DrawFormatString(10,10,GetColor(255, 255, 255),"StickX = %d",network.GetStickX());

}

// 解放
void GameClear::Release(void)
{
	// 作成されたフォント
	DeleteFontToHandle(ClearFont_);
}
