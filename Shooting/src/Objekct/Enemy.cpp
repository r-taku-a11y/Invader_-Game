#include <DxLib.h>
#include "Enemy.h"

// コンストラクタ
Enemy::Enemy(void)
{
	// 画像ハンドル初期化
	modelHandle_ = -1;

	//座標初期化
	PosX_ = 0.0f;
	PosY_ = 0.0f;

	// 移動速度初期化
	moveSpeed_ = 0.0f;

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
		modelHandle_ = LoadGraph("Data/images/enemy_red.png");
		break;
	case TYPE::ORANGE:
		modelHandle_ = LoadGraph("Data/images/enemy_orange.png");
		break;
	case TYPE::WHITE:
		modelHandle_ = LoadGraph("Data/images/enemy_white.png");
		break;
	case TYPE::YELLOW:
		modelHandle_ = LoadGraph("Data/images/enemy_yellow.png");
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
	// 画像の開放処理
	if (modelHandle_ != -1)
	{
		DeleteGraph(modelHandle_);

		modelHandle_ = -1;
	}
}

// 生存しているのか取得
bool Enemy::IsAlive(void) const
{
	return isAlive_;
}

// 敵を撃破の状態にする
void Enemy::kill(void)
{
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
