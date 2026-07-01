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

	// 弾リストを空にする　
	bulletList_.clear();
	
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

	// 弾の初期レベル
	bulletLevel_ = DEFAULT_BULLET_LEVEL;

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
	bool pressed = (nowButton && !prevButton_);

	// Arduinoボタンまたはマウスを押した瞬間
	if (pressed || input.IsTrgMouseLeft())
	{
		// 発射
		shoot();
	}

	// 次フレーム用に保存
	prevButton_ = nowButton;

	// 全ての弾の更新
	for (auto& bullet : bulletList_)
	{
		bullet.Update();
	}

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

	// 非アクティブになった弾をリストから削除
	bulletList_.erase(std::remove_if(bulletList_.begin(),bulletList_.end(),[](const Bullet& bullet)
	{
		// アクティブではない弾を削除対象にする
		return !bullet.IsActive();
	}),
		// remove_ifで後ろへ移動された不要な要素を削除
		bulletList_.end());

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

	
	// 全ての弾を描画
	for (auto& bullet : bulletList_)
	{
		bullet.Draw();
	}
	
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

// 残機を増やす
void Player::AddLife(void)
{
	// 最大残機未満なら回復
	if (life_ < START_LIFE)
	{
		life_++;
	}
}

// 弾レベルアップ
void Player::PowerUp(void)
{
	// 最大レベル未満なら強化させる
	if (bulletLevel_ < MAX_BULLET_LEVEL)
	{
		bulletLevel_++;
	}
}

// 弾レベルの取得
int Player::GetBulletLevel(void) const
{
	return bulletLevel_;
}

// 最大残機の取得
int Player::GetMaxLife(void) const
{
	return START_LIFE;
}

// 最大弾レベル取得
int Player::GetMaxBulletLevel(void) const
{
	return MAX_BULLET_LEVEL;
}

// プレイヤーの弾の取得
std::vector<Bullet>& Player::GetBulletList(void)
{
	return bulletList_;
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

	// インスタンスの取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の音量調整
	seMana_->SetVolumeSE("Shoot", SHOOT_VOLUME);

	// 現在画面に存在する弾数
	int activeBulletCount = 0;

	// 発射中の弾を数える
	for (auto& bullet : bulletList_)
	{
		if (bullet.IsActive())
		{
			activeBulletCount++;
		}
	}

	// 最大弾数に達しているなら発射しない
	if (activeBulletCount >= bulletLevel_)
	{
		return;
	}

	// 発射位置の計算
	int x = static_cast<int>(PosX_ + width / 2);
	int y = static_cast<int>(PosY_ - BULLET_Y_OFFSET);

	// 新しい弾の生成
	Bullet bullet;

	// 弾の初期化
	bullet.Init();

	// 現在の弾レベルで発射
	bullet.Shoot(x, y, bulletLevel_);

	// 弾リストへ追加
	bulletList_.push_back(bullet);

	// 効果音が再生中ではない場合だけ再生
	if (!seMana_->IsPlayingSE("Shoot"))
	{
		// 発射できた時だけSEを再生
		seMana_->PlaySE("Shoot");
	}
}
