#include <DxLib.h>
#include "Shield.h"

// コンストラクタ
Shield::Shield(void)
{
}

// デストラクタ
Shield::~Shield(void)
{
}

// 初期化(x:シールドのX座標 y:シールドのY座標)
void Shield::Init(float x, float y)
{
	// 座標の設定
	x_ = x;
	y_ = y;

	// シールドのサイズ
	width_ = BLOCK_NUM * BLOCK_WIDTH +(BLOCK_NUM - 1) * SPACE;
	height_ = SHIELD_HEIGHT;

	// 耐久値
	// 敵弾を３発(仮)防ぐ
	hp_ = MAX_HP;

	// 生存状態
	alive_ = true;

}

// 描画
void Shield::Draw(void)
{

	// 破壊済みなら描画しない
	if (!alive_)
	{
		return;
	}

	// 耐久値によって色を変更させる(目に見てわかるようにする)
	// 耐久値3
	// 無傷状態
	if (hp_ == HP_FULL)
	{
		// 白
		color = GetColor(255, 255, 255);
	}
	// 耐久値2
	// 少し損傷
	else if (hp_ == HP_MIDDLE)
	{
		// 黄色
		color = GetColor(255, 255, 0);
	}
	// 耐久値1
	// めちゃ損傷
	else
	{
		// 赤
		color = GetColor(255, 0, 0);
	}

	// シールドの本体の描画
	// 横一列のシールド描画
	for (int i = 0; i < BLOCK_NUM; i++)
	{
		DrawBox((int)x_ + i * (BLOCK_WIDTH + SPACE),(int)y_,(int)x_ + i * (BLOCK_WIDTH + SPACE) + BLOCK_WIDTH,(int)y_ + height_,color,TRUE);
	}

	// デバック
	// 現在の耐久値の表示
	//DrawFormatString((int)x_ + 25, (int)y_ + 12, GetColor(255, 255, 255), "%d", hp_);
}

// 敵の弾とシールド当たり判定
bool Shield::checkHit(int bulletX, int bulletY)
{
	// すでに破壊済みなら判定しないようにする
	if (!alive_)
	{
		return false;
	}

	// シールド範囲内に弾があるか判定する
	if (bulletX >= x_ && bulletX <= x_ + width_ && bulletY >= y_ && bulletY <= y_ + height_)
	{
		// 耐久値を減らす
		hp_--;

		// デバック
		// printfDx("Shield Hit! HP=%d\n", hp_);

		// 耐久が０になれば破壊する
		if (hp_ <= 0)
		{
			alive_ = false;
		}
		return true;
	}

	return false;
}

// 生存判定
bool Shield::IsAlive(void) const
{
	return alive_;
}
