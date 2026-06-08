#include <DxLib.h>
#include "GameClear.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
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
	// UDP通信開始
	network.Init();

	// 最初は再プレイのほうを選択させておく
	selectMenu_ = PLAY_AGAIN;

	// フォント作成
	ClearFont_ = CreateFontToHandle("Arial", CLEAR_FONT_SIZE, CLEAR_FONT_THICK);

	// 現在の状態を取得
	network.Update();
	oldButton_ = network.GetButton();

	// シーン遷移待ち
	changeSceneWait_ = false;

	// SE再生用の初期化
	gameClearSEPlayerd_ = false;

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の読み込み
	seMana_->LoadSE("GameClear", "Data/Sound/SE/Gameclear.mp3", false);
	seMana_->LoadSE("click", "Data/Sound/SE/Selection.mp3", false);
}

// 更新
void GameClear::Update(void)
{

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	seMana_->SetVolumeSE("GameClear", CLEAR_SE_VOLUME);

	// Arduinoから送信された最新データを受信
	network.Update();

	InputManager& input = InputManager::GetInstance();

	// ジョイスティック値取得
	int stickX = network.GetStickX();

	// 左選択
	if (stickX < STICK_LEFT_BORDER)
	{
		selectMenu_ = PLAY_AGAIN;
	}

	// 右選択
	if (stickX > STICK_RIGHT_BORDER)
	{
		selectMenu_ = RETURN_TITLE;
	}

	// キーボードでも操作可能
	if (input.IsTrgDown(KEY_INPUT_LEFT) || input.IsTrgDown(KEY_INPUT_A))
	{
		selectMenu_ = PLAY_AGAIN;
	}

	if (input.IsTrgDown(KEY_INPUT_RIGHT)|| input.IsTrgDown(KEY_INPUT_D))
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
		// 決定音再生
		seMana_->PlaySE("click");

		// シーン遷移待ち開始
		changeSceneWait_ = true;
	}

	if (changeSceneWait_)
	{
		if (!seMana_->IsPlayingSE("click"))
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

	// 一回しかゲームオーバーのSEを流さないようにする
	if (!gameClearSEPlayerd_)
	{
		// BGMの再生
		seMana_->PlaySE("GameClear");
		gameClearSEPlayerd_ = true;
	}
}

// 描画
void GameClear::Draw(void)
{
	// GAME CLEAR
	DrawStringToHandle(CLEAR_TEXT_X, CLEAR_TEXT_Y,"GAME CLEAR !!",GetColor(255, 255, 0), ClearFont_);

	// PLAY AGAIN選択中
	if (selectMenu_ == PLAY_AGAIN)
	{
		DrawString(PLAY_AGAIN_X, MENU_Y,"> PLAY AGAIN",GetColor(255, 255, 255));

		DrawString(RETURN_TITLE_X, MENU_Y,"RETURN TITLE",GetColor(128, 128, 128));
	}
	// RETURN TITLE選択中
	else
	{
		DrawString(PLAY_AGAIN_X, MENU_Y,"PLAY AGAIN",GetColor(128, 128, 128));

		DrawString(RETURN_TITLE_X, MENU_Y,"> RETURN TITLE",GetColor(255, 255, 255));
	}

	// 操作説明
	DrawString(GUIDE_X, GUIDE_Y,"LEFT/RIGHT A/D : SELECT   MOUSELEFT/BUTTON : DECIDE",GetColor(255, 255, 255));

	// デバック
	//DrawFormatString(10,10,GetColor(255, 255, 255),"StickX = %d",network.GetStickX());

}

// 解放
void GameClear::Release(void)
{
	// 作成されたフォント
	DeleteFontToHandle(ClearFont_);

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	//サウンドの停止
	seMana_->StopSE("GameClear");
	seMana_->StopSE("click");

	// 解放
	seMana_->ReleaseSound("GameClear");
	seMana_->ReleaseSound("click");
}
