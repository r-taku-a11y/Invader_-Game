#include <DxLib.h>
#include "Item.h"

// コンストラクタ
Item::Item(void)
{
	// 初期化
	x_ = 0.0f;
	y_ = 0.0f;

	// 落下速度
	speed = ITEM_SPEED;

	// アイテム種類
	type_ = ITEM_NONE;

	// 非アクティブ状態
	active_ = false;
}

// デストラクタ
Item::~Item(void)
{
}

// 初期化
void Item::Init(void)
{
	// 初期化
	x_ = 0.0f;
	y_ = 0.0f;

	// 落下速度
	speed = ITEM_SPEED;

	// アイテム種類
	type_ = ITEM_NONE;

	// 非アクティブ状態
	active_ = false;
}

// 更新
void Item::Update(void)
{
	// 出現していない場合は更新しない
	if (!active_)return;

	// 下方向へ移動
	y_ += speed;
}

// 描画
void Item::Draw(void)
{
	// 出現していない場合は更新しない
	if (!active_)return;

	// 現在は仮の図形で描画
	DrawCircle(static_cast<int>(x_), static_cast<int>(y_), 8, GetColor(255, 255, 0), TRUE);
}

// 解放
void Item::Release(void)
{
}

// アイテム出現
void Item::Spawn(float x, float y, ITEM_TYPE type)
{
	// 出現位置設定
	x_ = x;
	y_ = y;

	// アイテム種類設定
	type_ = type;

	// アクティブ状態にする
	active_ = true;

}

// アイテム取得
void Item::Destroy(void)
{
	// 非アクティブ状態
	active_ = false;
}

// アクティブ状態取得
bool Item::IsActive(void) const
{
	return 	active_;
	;
}

// アイテム種類取得
Item::ITEM_TYPE Item::GetType(void) const
{
	return type_;
}

// X座標取得
float Item::GetX(void) const
{
	return x_;
}

// Y座標取得
float Item::GetY(void) const
{
	return y_;
}

// プレイヤーとの当たり判定
bool Item::CheckHit(const Player& player)
{
	// 出現していない
	if (!active_)return false;

	// プレイヤーとの矩形判定
	if (x_ >= player.GetX() && x_ <= player.GetX() + player.GetWidth() && y_ >= player.GetY() && y_ <= player.GetY() + player.GetHeight())
	{
		return true;
	}

	return false;
}
