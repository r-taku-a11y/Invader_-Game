#include <DxLib.h>
#include "EnemyBullet.h"

// コンストラクタ
EnemyBullet::EnemyBullet(void)
{
	x_ = 0;
	y_ = 0;

	// 下方向への移動速度
	speed_ = 5;

	// 非アクティブ状態
	active_ = false;
}

// デストラクタ
EnemyBullet::~EnemyBullet(void)
{
}

// 初期化
void EnemyBullet::Init(void)
{
	x_ = 0;
	y_ = 0;

	active_ = false;
}

// 更新処理
void EnemyBullet::Update(void)
{
	// 弾が存在しない
	if (!active_)return;
	
	// 下方向へ移動
	y_ += speed_;

	// 画面外へ出たら消す
	if (y_ > 600)
	{
		active_ = false;
	}
}

// 描画
void EnemyBullet::Draw(void)
{
	// 弾が存在しない
	if (!active_)
	{
		return;
	}

	// 敵弾の描画
	DrawBox(x_-2, y_ - 8,x_ + 2,y_ + 8,GetColor(255, 255, 255),TRUE);
}

// 発射
void EnemyBullet::Shoot(int startX, int startY)
{
	// すでに弾がするのか
	if (active_)return;

	// 発射位置設定
	x_ = startX;
	y_ = startY;

	// 生存状態か
	active_ = true;
}

// 生存状態取得
bool EnemyBullet::IsActive(void) const
{
	return active_;
}

// 弾を消す
void EnemyBullet::Disable(void)
{ 
	active_ = false;
}

// X座標取得
int EnemyBullet::GetX(void) const
{
	return x_;
}

// Y座標取得
int EnemyBullet::GetY(void) const
{
	return y_;
}
