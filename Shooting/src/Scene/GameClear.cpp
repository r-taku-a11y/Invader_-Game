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
}

// 更新
void GameClear::Update(void)
{
	InputManager& input = InputManager::GetInstance();

	// 左キー
	if (input.IsTrgDown(KEY_INPUT_LEFT))
	{
		selectMenu_ = PLAY_AGAIN;
	}

	// 右キー
	if (input.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectMenu_ = RETURN_TITLE;
	}

	// 決定
	if (input.IsTrgDown(KEY_INPUT_RETURN))
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
	DrawString(300,200,"GAME CLEAR !!",GetColor(255, 255, 0));

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

}

// 解放
void GameClear::Release(void)
{
}
