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
	// InputManagerの取得
	InputManager& input
		= InputManager::GetInstance();

	// 上キー
	if (input.IsTrgDown(KEY_INPUT_UP))
	{
		selectMenu_--;

		//一番上でさらに上を押すと一番下に移動させる
		if (selectMenu_ < 0)
		{
			selectMenu_ = MENU_NUM - 1;
		}
	}

	// 下キー
	if (input.IsTrgDown(KEY_INPUT_DOWN))
	{
		selectMenu_++;

		// 一番下でさらに下を押したら一番上に移動させる
		if (selectMenu_ >= MENU_NUM)
		{
			selectMenu_ = 0;
		}
	}

	// とりあえず今はデバックなのでエンターキーで実行させる
	if (input.IsTrgDown(KEY_INPUT_RETURN))
	{
		switch (selectMenu_)
		{
			// もう一度プレイ
		case PLAY_AGAIN:
			// ゲームシーンに遷移させる
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMESENEN);
			break;

			// タイトル画面へ戻る
		case RETURN_TITLE:
			// タイトルの遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			break;
		}
	}

}

// 描画
void GameClear::Draw(void)
{
	// 背景の描画
	DrawGraph(0, 0, gameClearHandle_, TRUE);

	// PLAY AGAIN
	if (selectMenu_ == PLAY_AGAIN)
	{
		// 選択中
		//DrawString(Application::SCREEN_SIZE_X)
	}
}
