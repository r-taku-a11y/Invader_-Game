#include <DxLib.h>
#include "Bullet.h"
#include "../Application.h"

// コンストラクタ
Bullet::Bullet(void)
{
	x_ = 0;
	y_ = 0;
	active_ = false;

	// 上方向の速度
	speed_ = MOVE_SPEED;
}

// デストラクタ
Bullet::~Bullet(void)
{
}

// 初期化
void Bullet::Init(void)
{
	active_ = false;
	x_ = 0;
	y_ = 0;
}

// 更新
void Bullet::Update(void)
{
	// 弾が生存しているうちは弾を打ち出さないようにする
	if (!active_)return;


	// 前フレームからの経過時間取得
	float deltaTime = Application::GetInstance().GetDeltaTime();

	// 上に移動
	y_ -= speed_ * deltaTime * 60.0f;

	// 画面外に出たら消す
	if (y_ < SCREEN_TOP)
	{
		active_ = false;
	}
}

// 描画
void Bullet::Draw(void)
{
	// 弾が生存しているうちは弾を打ち出さないようにする
	if (!active_)return;
	
	DrawLine(x_,y_ - BULLET_DRAW_TOP,x_,y_ + BULLET_DRAW_BOTTOM,GetColor(255, 255, 255));
}

// 弾の発射
void Bullet::Shoot(int x, int y, int level)
{
	// すでに飛んでいる場合は再発射しない
	if (active_)return;

	// 発射位置を設定
	x_ = x;
	y_ = y;

	// フラグを立てる
	active_ = true;
}

// 弾の生存しているか
bool Bullet::IsActive(void) const
{
	return active_;
}

// 弾の無効化
void Bullet::Disable(void)
{
	// フラグを折る
	active_ = false;
}

// X座標の取得
int Bullet::GetX(void) const
{
	return x_;
}

// Y座標の取得
int Bullet::GetY(void) const
{
	return y_;
}
