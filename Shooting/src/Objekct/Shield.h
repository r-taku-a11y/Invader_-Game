#pragma once
class Shield
{
public:
	Shield(void);					// コンストラクタ
	~Shield(void);					// デストラクタ

	void Init(float x, float y);	// 初期化(x:シールドのX座標 y:シールドのY座標)
	void Draw(void);				// 描画

	// 敵の弾とシールド当たり判定
	bool checkHit(int bulletX, int bulletY);

	// 生存判定
	bool IsAlive(void)const;

private:

	// シールドのX座標
	float x_;

	// シールドのY座標
	float y_;

	// シールドの横幅
	int width_;

	// シールドの高さ
	int height_;

	// 耐久値(今は3発でなくなるようにしとく)
	int hp_;

	// 生存フラグ
	bool alive_;

	// カラー
	int color;

	// ブロック数
	const int BLOCK_NUM = 1;

	// ブロック幅
	const int BLOCK_WIDTH = 40;

	// ブロック間隔
	const int SPACE = 50;

	// シールドの高さ
	static constexpr int SHIELD_HEIGHT = 30;

	// 初期耐久値
	static constexpr int MAX_HP = 3;

	// 耐久値にいる色変更
	static constexpr int HP_FULL = 3;
	static constexpr int HP_MIDDLE = 2;
};

