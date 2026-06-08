#include <DxLib.h>
#include "EnemyBullet.h"
#include "Player.h"
#include "../Application.h"

// コンストラクタ
EnemyBullet::EnemyBullet(void)
{
	x_ = 0;
	y_ = 0;

	// 下方向への移動速度
	speed_ = BULLET_SPEED;

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
	
	// 前フレームからの経過時間取得
	float deltaTime = Application::GetInstance().GetDeltaTime();

	// 下方向へ移動
	y_ += speed_ * deltaTime * 60.0f;

	// 画面外へ出たら消す
	if (y_ > Application::SCREEN_SIZE_Y)
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
	DrawBox(x_-BULLET_HALF_WIDTH, y_ - BULLET_HALF_HEIGHT,x_ + BULLET_HALF_WIDTH,y_ + BULLET_HALF_HEIGHT,GetColor(BULLET_COLOR_R, BULLET_COLOR_G, BULLET_COLOR_B),TRUE);

	// デバック
	// DrawFormatString(10, 550, GetColor(255, 255, 255), "Bullet X=%d Y=%d", x_, y_);
}

// 発射
void EnemyBullet::Shoot(int startX, int startY)
{
	// すでに弾があるのか
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

bool EnemyBullet::CheckHitPlayer(const Player& player)
{
	// 弾が存在しない
	if (!active_)
	{
		return false;
	}

	// プレイヤーの当たり判定範囲

	// 左側
	int playerLeft = static_cast<int>(player.GetX());

	// 右側
	int playerRight = static_cast<int>(player.GetX()) + player.GetWidth();

	// 上端
	int playerTop = static_cast<int>(player.GetY());

	// 下側
	int playerBottom = static_cast<int>(player.GetY()) + player.GetHeight();

	// 敵の弾の当たり判定

	// 左端
	int bulletLeft = x_ - BULLET_HALF_WIDTH;

	// 右端
	int bulletRight = x_ + BULLET_HALF_WIDTH;

	// 上端
	int bulletTop = y_ - BULLET_HALF_HEIGHT;

	// 下端
	int bulletBottom = y_ + BULLET_HALF_HEIGHT;

	// 短形同士の当たり判定
	// 1つでも当てはまれば当たっていないようにする

	// 弾がプレイヤーより左になる
	if (bulletRight < playerLeft)
	{
		return false;
	}

	// 弾がプレイヤーよりも右にある
	if (bulletLeft > playerRight)
	{
		return false;
	}

	// 弾がプレイヤーよりも上にある
	if (bulletBottom < playerTop)
	{
		return false;
	}

	// 弾がプレイヤーより下にある
	if (bulletTop > playerBottom)
	{
		return false;
	}

	// 当たった弾を消す
	active_ = false;

	// 上記のどれにも当てはまらない場合のみ(当たった判定を返す)
	return true;
}
