#include "Title.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
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
	// UDP通信開始
	network.Init();

	// ゲーム開始フラグの初期化
	StartGame = false;

	// 点滅用のタイマーの初期化
	BlinkTimer = 0;

	// PRESS ENTER表示フラグ
	ShowPressKey = true;

	// フォント作成

	titleFont_ = CreateFontToHandle("Arial", TITLE_FONT_SIZE, TITLE_FONT_THICK);

	menuFont_ = CreateFontToHandle("Arial", MENU_FONT_SIZE, MENU_FONT_THICK);

	// ボタン状態初期化
	oldButton_ = false;

	// 現在のボタン状態を取得
	network.Update();
	oldButton_ = network.GetButton();

	// シーン遷移待ち
	changeSceneWait_ = false;

	// インスタンス取得
	SoundManager * seMana_ = SoundManager::GetInstance();

	// 流す音の読み込み
	seMana_->LoadBGM("Title","Data/Sound/BGM/Title.mp3", false);
	seMana_->LoadSE("click", "Data/Sound/SE/Selection.mp3", false);
}

// 更新
void Title::Update(void)
{
	// 受信
	network.Update();

	BlinkTimer++;

	// 30フレームごとにPRESS ENTERの表示の切り替えさせる
	if (BlinkTimer >= BLINK_INTERVAL)
	{
		ShowPressKey = !ShowPressKey;

		BlinkTimer = 0;
	}

	// 現在のボタン状態
	bool nowButton = network.GetButton();

	// 押した瞬間判定
	bool buttonTriggr = nowButton && !oldButton_;

	oldButton_ = nowButton;

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	// 音の音量調整
	seMana_->SetVolumeBGM("Title", BGM_VOLUME);
	seMana_->SetVolumeSE("click", CLICK_VOLUME);
	// マウスの左
	if (InputManager::GetInstance().IsTrgMouseLeft() || buttonTriggr)
	{
		// 決定音再生
		seMana_->PlaySE("click");
		
		// シーン遷移待ち開始
		changeSceneWait_ = true;
	}

	if (changeSceneWait_)
	{
		// clickが鳴り終わった
		if (!seMana_->IsPlayingSE("click"))
		{
			StartGame = true;
		}
	}

	// タイトルのBGMの再生
	seMana_->PlayBGM("Title");
}

// 描画
void Title::Draw(void)
{
	// 画面中央座標

	int centerX = Application::SCREEN_SIZE_X / 2;

	// タイトル

	const char* title = "SPACE INVADER";

	int titleWidth = GetDrawStringWidthToHandle(title, strlen(title), titleFont_);

	DrawStringToHandle(centerX - titleWidth / 2, TITLE_Y, title, GetColor(0, 255, 0), titleFont_);

	// サブタイトル

	const char* subTitle = "DEFEND THE EARTH";

	int subWidth = GetDrawStringWidthToHandle(subTitle, strlen(subTitle), menuFont_);

	DrawStringToHandle(centerX - subWidth / 2, SUBTITLE_Y, subTitle, GetColor(255, 255, 0), menuFont_);

	// PRESS ENTER

	if (ShowPressKey)
	{
		const char* pressText = "PRESS BUTTON";

		int pressWidth = GetDrawStringWidthToHandle(pressText, strlen(pressText), menuFont_);

		DrawStringToHandle(centerX - pressWidth / 2, PRESS_BUTTON_Y, pressText, GetColor(255, 255, 255), menuFont_);
	}

	// ゲーム開始

	const char* startText = "BUTTON / CLICK : GAME START";

	int startWidth = GetDrawStringWidthToHandle(startText, strlen(startText), menuFont_);

	DrawStringToHandle(centerX - startWidth / 2, START_TEXT_Y, startText, GetColor(255, 255, 255), menuFont_);

	// 終了

	const char* exitText = "ESC : EXIT";

	int exitWidth = GetDrawStringWidthToHandle(exitText, strlen(exitText), menuFont_);

	DrawStringToHandle(centerX - exitWidth / 2, EXIT_TEXT_Y, exitText, GetColor(255, 255, 255), menuFont_);

	// デバック
	// DrawFormatString(10,10,GetColor(255, 255, 255),"BTN=%d",network.GetButton());
}

// 解放
void Title::Release(void)
{
	DeleteFontToHandle(titleFont_);
	DeleteFontToHandle(menuFont_);

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// サウンドの停止
	seMana_->StopBGM("Title");
	seMana_->StopSE("click");

	// 音の開放
	seMana_->ReleaseSound("Title");
	seMana_->ReleaseSound("click");

}

// ゲーム開始判定の取得
bool Title::IsStartGame() const
{
	return StartGame;
}
