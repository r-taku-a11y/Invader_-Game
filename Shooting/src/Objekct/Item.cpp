#include <DxLib.h>
#include "../Application.h"
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

	// アイテム画像ハンドル初期化
	for (int i = 0; i <= ITEM_SCORE; i++)
	{
		// 未読み込み状態
		itemImage_[i] = -1;
	}
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

	// 左右の揺れ初期化
	wave_ = 0.0f;

	// 初期X座標
	startX_ = 0.0f;

	// アイテム画像の読み込み
	if (itemImage_[ITEM_SHIELD] == -1)
	{
		itemImage_[ITEM_SHIELD] = LoadGraph("Data/Images/Shield.png");
	}

	if (itemImage_[ITEM_LIFE] == -1)
	{
		itemImage_[ITEM_LIFE] = LoadGraph("Data/Images/Life.png");
	}

	if (itemImage_[ITEM_POWER] == -1)
	{
		itemImage_[ITEM_POWER] = LoadGraph("Data/Images/Bullet.png");
	}

	if (itemImage_[ITEM_SCORE] == -1)
	{
		itemImage_[ITEM_SCORE] = LoadGraph("Data/Images/Score.png");
	}
}

// 更新
void Item::Update(void)
{
	// 出現していない場合は更新しない
	if (!active_)return;

	// 下方向へ移動
	y_ += speed;

	// 揺れを進める
	wave_ += WAVE_SPEED;

	// 左右に揺らす
	x_ = startX_ + sinf(wave_) * WAVE_WIDTH;

	// 画面外に出たら消滅させる
	if(y_>Application::SCREEN_SIZE_Y)
	{
		// 非アクティブ状態にする
		active_ = false;
	}
}

// 描画
void Item::Draw(void)
{
	// 出現していない場合は更新しない
	if (!active_)return;

	// アイテム画像が読み込まれている場合のみ描画
	if (itemImage_[type_] != -1)
	{
		DrawExtendGraph(static_cast<int>(x_),static_cast<int>(y_),static_cast<int>(x_) + ITEM_SIZE,static_cast<int>(y_) + ITEM_SIZE,itemImage_[type_],TRUE);
	}
}

// 解放
void Item::Release(void)
{
	// 読み込んだ画像を解放
	for (int i = 0; i <= ITEM_SCORE; i++)
	{
		if (itemImage_[i] != -1)
		{
			DeleteGraph(itemImage_[i]);
			itemImage_[i] = -1;
		}
	}
}

// アイテム出現
void Item::Spawn(float x, float y, ITEM_TYPE type)
{
	// 出現位置設定
	x_ = x;
	y_ = y;

	// 初期位置を保存
	startX_ = x;

	// 揺れを初期化
	wave_ = 0.0f;

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
