#include <DxLib.h>
#include "GameOver.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Application.h"

// コンストラクタ
GameOver::GameOver(void)
{
}

// デストラクタ
GameOver::~GameOver(void)
{
}

// 初期化
void GameOver::Init(void)
{
	// UDP通信開始
	network.Init();

	// 最初は再プレイのほうを選択させておく
	selectMenu_ = PLAY_AGAIN;

	// フォントの作成
	OverFont_ = CreateFontToHandle("Arial", GAMEOVER_FONT_SIZE, GAMEOVER_FONT_THICK);

	// 現在の状態を取得
	network.Update();
	oldButton_ = network.GetButton();

	// シーン遷移待ち
	changeSceneWait_ = false;

	// SE再生用の初期化
	gameOverSEPlayerd_ = false;

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の読み込み
	seMana_->LoadSE("GameOver", "Data/Sound/SE/Gameover.mp3", false);
	seMana_->LoadSE("click", "Data/Sound/SE/Selection.mp3", false);
}

// 更新
void GameOver::Update(void)
{

	// Arduinoからの送信された最新データを受信
	network.Update();

	InputManager& input = InputManager::GetInstance();

	// ジョイスティック値の取得
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

	if (input.IsTrgDown(KEY_INPUT_RIGHT) || input.IsTrgDown(KEY_INPUT_D))
	{
		selectMenu_ = RETURN_TITLE;
	}

	// ボタンの現在状態の取得
	bool nowButton = network.GetButton();

	// ボタンが押された瞬間
	bool buttonTrigger = nowButton && !oldButton_;

	oldButton_ = nowButton;

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音量の設定
	seMana_->SetVolumeSE("click", CLICK_VOLUME);

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
	if (!gameOverSEPlayerd_)
	{
		// BGMの再生
		seMana_->PlaySE("GameOver");
		gameOverSEPlayerd_ = true;
	}
}

// 描画
void GameOver::Draw(void)
{
	// GANE OVER
	DrawStringToHandle(GAMEOVER_X, GAMEOVER_Y, "GAMEOVER !!", GetColor(255, 0, 0), OverFont_);

	// PLAY AGAIN選択中
	if (selectMenu_ == PLAY_AGAIN)
	{
		DrawString(PLAY_AGAIN_X, PLAY_AGAIN_Y, "> PLAY AGAIN", GetColor(255, 255, 255));
		DrawString(RETURN_TITLE_X, RETURN_TITLE_Y, "RETURN TITLE", GetColor(128, 128, 128));
	}
	// RETURN TITLE選択中
	else
	{
		DrawString(PLAY_AGAIN_X, PLAY_AGAIN_Y, "PLAY AGAIN", GetColor(128, 128, 128));
		DrawString(RETURN_TITLE_X, RETURN_TITLE_Y, "> RETURN TITLE", GetColor(255, 255, 255));
	}

	// 操作説明
	DrawString(GUIDE_X, GUIDE_Y, "LEFT/RIGHT A/D : SELECT   MOUSELEFT/BUTTON: DECIDE", GetColor(255, 255, 255));

	// デバック
	//DrawFormatString(10, 10, GetColor(255, 255, 255), "StickX = %d", network.GetStickX());
	//DrawFormatString(10,30,GetColor(255, 255, 255),"Menu = %d",selectMenu_);
}

// 解放
void GameOver::Release(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	
	//サウンドの停止
	seMana_->StopSE("GameOver");
	seMana_->StopSE("click");

	// 解放
	seMana_->ReleaseSound("GameOver");
	seMana_->ReleaseSound("click");

	// 作成されたフォント
	DeleteFontToHandle(OverFont_);
}
