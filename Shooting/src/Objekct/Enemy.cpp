#include <DxLib.h>
#include "Enemy.h"
#include "../Manager/SoundManager.h"

// 共通画像ハンドル

int Enemy::redHandle_ = -1;
int Enemy::orangeHandle_ = -1;
int Enemy::whiteHandle_ = -1;
int Enemy::yellowHandle_ = -1;

// コンストラクタ
Enemy::Enemy(void)
{
	// 画像ハンドル初期化
	modelHandle_ = -1;

	//座標初期化
	PosX_ = 0.0f;
	PosY_ = 0.0f;

	// サイズの初期化
	width = 0;
	height = 0;

	// 生存状態で開始させる
	isAlive_ = true;
}

// デストラクタ
Enemy::~Enemy(void)
{
}

// 初期化
void Enemy::Init(TYPE type, float x, float y)
{
	// 敵の種類を保存
	type_ = type;

	// 初期座標を設定
	PosX_ = x;
	PosY_ = y;

	// 生存状態
	isAlive_ = true;

	// 敵の種類ごとに画像を読み込む

	switch (type_)
	{
	case TYPE::RED:
		modelHandle_ = redHandle_;
		break;
	case TYPE::ORANGE:
		modelHandle_ = orangeHandle_;
		break;
	case TYPE::WHITE:
		modelHandle_ = whiteHandle_;
		break;
	case TYPE::YELLOW:
		modelHandle_ = yellowHandle_;
		break;
	default:
		break;
	}

	// 画像サイズの取得
	GetGraphSize(modelHandle_, &width, &height);
}

// 更新(まとめて管理してるので今のとこなし)
void Enemy::Update(void)
{
}

// 描画
void Enemy::Draw(void)
{
	if (!isAlive_)
	{
		return;
	}
	DrawGraph(static_cast<int>(PosX_), static_cast<int>(PosY_), modelHandle_, TRUE);
}

// 解放
void Enemy::Release(void)
{
}

// 生存しているのか取得
bool Enemy::IsAlive(void) const
{
	return isAlive_;
}

// 敵を撃破の状態にする
void Enemy::kill(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	// 撃破音の音量
	seMana_->SetVolumeSE("Kill", KILL_SE_VOLUME);
	// 再生中でも最初から流す
	seMana_->PlaySEForce("Kill");
	// 撃破状態に変更
	isAlive_ = false;
}

// X座標の取得
float Enemy::GetPosX(void) const
{
	return PosX_;
}

// Y座標の取得
float Enemy::GetPosY(void) const
{
	return PosY_;
}

// X座標の設定
void Enemy::SetPosX(float x)
{
	PosX_ = x;
}

// Y座標の設定
void Enemy::SetPosY(float y)
{
	PosY_ = y;
}

// 横幅の取得
int Enemy::GetWidth(void) const
{
	return width;
}

int Enemy::GetHeight(void) const
{
	return height;
}

Enemy::TYPE Enemy::GetType(void) const
{
	return type_;
}

// 
void Enemy::LoadResource(void)
{
	// 一度だけロードする
	if (redHandle_ == -1)
	{
		redHandle_ = LoadGraph("Data/Images/enemy_red.png");
	}

	if (orangeHandle_ == -1)
	{
		orangeHandle_ = LoadGraph("Data/Images/enemy_orange.png");
	}

	if (whiteHandle_ == -1)
	{
		whiteHandle_ = LoadGraph("Data/Images/enemy_white.png");
	}

	if (yellowHandle_ == -1)
	{
		yellowHandle_ = LoadGraph("Data/Images/enemy_yellow.png");
	}
}

// 共通画像の開放
void Enemy::ReleaseResource(void)
{
	if (redHandle_ != -1)
	{
		DeleteGraph(redHandle_);
		redHandle_ = -1;
	}

	if (orangeHandle_ != -1)
	{
		DeleteGraph(orangeHandle_);
		orangeHandle_ = -1;
	}

	if (whiteHandle_ != -1)
	{
		DeleteGraph(whiteHandle_);
		whiteHandle_ = -1;
	}

	if (yellowHandle_ != -1)
	{
		DeleteGraph(yellowHandle_);
		yellowHandle_ = -1;
	}
}

// 列番号の設定
void Enemy::SetColumn(int column)
{
	// 敵が所属する列番号
	column_ = column;
}

// 列番号の取得
int Enemy::GetColumn() const
{
	return column_;
}