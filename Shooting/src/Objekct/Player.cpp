#include <DxLib.h>
#include "Player.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/NetworkManager.h"

// コンストラクタ
Player::Player(void)
{
	// 画像の初期化
	PlayerModel_ = -1;
	// 座標初期化
	PosX_ = 0.0f;
	PosY_ = 0.0f;

	// プレイヤースピード
	moveSpeed_ = 5.0f;

	// 弾の初期化
	prevButton_ = false;
}

// デストラクタ
Player::~Player(void)
{
}

// 初期化
void Player::Init(void)
{
	// 弾の初期化
	prevButton_ = false;

	bullet.Init();
	
	// プレイヤー画像の読み込み
	PlayerModel_ = LoadGraph("Data/images/player_ship.png");

	// プレイヤーの座標
	PosX_ = 400.0f;
	PosY_ = 500.0f;

	// 画像サイズの取得(ポインター渡し)
	GetGraphSize(PlayerModel_, &width, &height);

	//  画面下中央に配置させる
	PosX_ = Application::HALF_SCREEN_SIZE_X - width / 2.0f;
	PosY_ = (Application::HALF_SCREEN_SIZE_Y - height) + 300.0f;
}

// 更新
void Player::Update(NetworkManager& network)
{
	// InputManagerの取得
	InputManager& input = InputManager::GetInstance();

	// 左キー
	if (input.IsNew(KEY_INPUT_LEFT) || input.IsNew(KEY_INPUT_A))
	{
		PosX_ -= moveSpeed_;
	}

	// 右キー
	if (input.IsNew(KEY_INPUT_RIGHT) || input.IsNew(KEY_INPUT_D))
	{
		PosX_ += moveSpeed_;
	}

	// ジョイスティック取得
	int stickX = network.GetStickX();

	// 中央付近なら無視（＝入力なし扱い）
	if (stickX < 480)
	{
		PosX_ -= moveSpeed_;
	}
	else if (stickX > 600)
	{
		PosX_ += moveSpeed_;
	}

	// 左
	if (stickX < 523)
	{
		PosX_ -= moveSpeed_;
	}

	// 右
	if (stickX > 700)
	{
		PosX_ += moveSpeed_;
	}

	// 画面上に出ないようにする
	if (PosX_ < 0)
	{
		PosX_ = 0;
	}

	GetGraphSize(PlayerModel_, &width, &height);

	if (PosX_ > Application::SCREEN_SIZE_X - width)
	{
		PosX_ = Application::SCREEN_SIZE_X - width;
	}

	// ボタンの現在状態取得
	bool nowButton = network.GetButton();

	// 押した瞬間のみ発射
	if (nowButton && !prevButton_)
	{
		shoot();
	}

	// マウスの左でも一応できるようにしている
	if (input.IsTrgMouseLeft())
	{
		shoot();
	}

	// 次フレーム用に保存
	prevButton_ = nowButton;

	// 弾の更新
	bullet.Update();

}

// 描画
void Player::Draw(void)
{
	DrawGraph(static_cast<int>(PosX_), static_cast<int>(PosY_), PlayerModel_, TRUE);
	// 弾の描画
	bullet.Draw();
}

// 解放
void Player::Release(void)
{
	// 読みこんだ画像を解放させる
	if (PlayerModel_ != -1)
	{
		DeleteGraph(PlayerModel_);

		PlayerModel_ = -1;
	}
	
}

// プレイヤーの弾の取得
Bullet& Player::GetBullet(void)
{
	return bullet;
}

// 弾の発射処理
void Player::shoot()
{
	bullet.Shoot(static_cast<int>(PosX_ + width / 2), static_cast<int>(PosY_ - 5));
}
