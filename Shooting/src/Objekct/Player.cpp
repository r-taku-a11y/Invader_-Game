#include <DxLib.h>
#include "Player.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/NetworkManager.h"
#include "../Manager/SoundManager.h"

// コンストラクタ
Player::Player(void)
{
	// 画像の初期化
	PlayerModel_ = -1;
	// 座標初期化
	PosX_ = 0.0f;
	PosY_ = 0.0f;

	// プレイヤースピード
	moveSpeed_ = MOVE_SPEED;

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
	
	// 再度読み込まれないようにする
	if (PlayerModel_ == -1)
	{
		// プレイヤー画像の読み込み
		PlayerModel_ = LoadGraph("Data/Images/player_ship.png");
	}

	// 残機の初期化
	life_ = START_LIFE;

	// 無敵状態
	invincible_ = false;

	// 無敵時間
	incincbleTimer_ = 0;

	// タイマー初期化
	blinkTimer_ = 0.0f;

	// 画像サイズの取得(ポインター渡し)
	GetGraphSize(PlayerModel_, &width, &height);

	//  画面下中央に配置させる
	PosX_ = Application::HALF_SCREEN_SIZE_X - width / 2.0f;
	PosY_ = (Application::HALF_SCREEN_SIZE_Y - height) + PLAYER_Y_OFFSET;

	// インスタンスの取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の読み込み
	seMana_->LoadSE("Shoot","Data/Sound/SE/Player.mp3",false);
	seMana_->LoadSE("Damage", "Data/Sound/SE/Damage.mp3", false);
}

// 更新
void Player::Update(NetworkManager& network)
{
	// InputManagerの取得
	InputManager& input = InputManager::GetInstance();

	// インスタンスの取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// フレーム時間取得
	float deltaTime = Application::GetInstance().GetDeltaTime();

	// 左キー
	if (input.IsNew(KEY_INPUT_LEFT) || input.IsNew(KEY_INPUT_A))
	{
		PosX_ -= moveSpeed_ * deltaTime;
	}

	// 右キー
	if (input.IsNew(KEY_INPUT_RIGHT) || input.IsNew(KEY_INPUT_D))
	{
		PosX_ += moveSpeed_ * deltaTime;
	}

	// ジョイスティック取得
	int stickX = network.GetStickX();

	// デッドゾーン
	bool move = true;

	if (stickX > STICK_DEADZONE_MIN && stickX < STICK_DEADZONE_MAX)
	{
		move = false;
	}

	// 左右移動だけ制御
	if (move)
	{
		// 左
		if (stickX < STICK_LEFT_BORDER)
		{
			PosX_ -= moveSpeed_ * deltaTime;
		}

		// 右
		if (stickX > STICK_RIGHT_BORDER)
		{
			PosX_ += moveSpeed_ * deltaTime;
		}
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

	// 立ち上がり
	bool pressd = (nowButton && !prevButton_);

	// 音の音量調整
	seMana_->SetVolumeSE("Shoot", SHOOT_VOLUME);

	// 押した瞬間のみ発射
	if (pressd)
	{
		seMana_->PlaySE("Shoot");
		shoot();
	}

	// マウスの左でも一応できるようにしている
	if (input.IsTrgMouseLeft())
	{
		seMana_->PlaySE("Shoot");
		shoot();
	}

	// 次フレーム用に保存
	prevButton_ = nowButton;

	// 弾の更新
	bullet.Update();

	// 無敵時間の更新
	if (invincible_)
	{
		// フレーム時間取得
		float deltaTime = Application::GetInstance().GetDeltaTime();

		blinkTimer_ += deltaTime;

		// 無敵時間減少
		incincbleTimer_ -= deltaTime;

		// 無敵終了
		if (incincbleTimer_ <= 0.0f)
		{
			invincible_ = false;
			blinkTimer_ = 0.0f;
		}
	}

}

// 描画
void Player::Draw(void)
{
	// 無敵中は点滅表示
	if (invincible_)
	{
		// 4フレームごとに表示・非表示切替
		if (fmod(blinkTimer_, 0.2f) < 0.1f)
		{
			DrawGraph(static_cast<int>(PosX_), static_cast<int>(PosY_), PlayerModel_, TRUE);
		}
	}
	else
	{
		// 通常描画
		DrawGraph(static_cast<int>(PosX_), static_cast<int>(PosY_), PlayerModel_, TRUE);
	}

	
	// 弾の描画
	bullet.Draw();
}

// 解放
void Player::Release(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の停止
	seMana_->StopSE("Shoot");
	seMana_->StopSE("Damage");

	// 音の開放
	seMana_->ReleaseSound("Shoot");
	seMana_->ReleaseSound("Damage");

	// 読みこんだ画像を解放させる
	if (PlayerModel_ != -1)
	{
		DeleteGraph(PlayerModel_);

		PlayerModel_ = -1;
	}
	
}

// 残機を減らす
void Player::Damage(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	seMana_->PlaySE("Damage");

	// 無敵中はダメージを受けない
	if (invincible_)return;

	// 残機の減少
	if (life_ > 0)
	{
		life_--;
	}

	// 無敵時間開始
	invincible_ = true;

	// 約1秒
	incincbleTimer_ = INVINCIBLE_TIME;
}

// 無敵中判定
bool Player::IsInvincible(void) const
{
	return invincible_;
}

// ボタン状態
void Player::ResetButtonState(bool currentState)
{
	prevButton_ = currentState;
}

// プレイヤーの弾の取得
Bullet& Player::GetBullet(void)
{
	return bullet;
}

// X座標の取得
float Player::GetX(void)const
{
	return PosX_;
}

// Y座標の取得
float Player::GetY(void)const
{
	return PosY_;
}

// 横の取得
int Player::GetWidth(void) const
{
	return width;
}

// 縦
int Player::GetHeight(void) const
{
	return height;
}

// 残機の取得
int Player::GetLife(void) const
{
	return life_;
}

// 画像ハンドルの取得
int Player::GetModelHandle(void) const
{
	return PlayerModel_;
}

// 弾の発射処理
void Player::shoot()
{
	bullet.Shoot(static_cast<int>(PosX_ + width / 2), static_cast<int>(PosY_ - 5));
}
