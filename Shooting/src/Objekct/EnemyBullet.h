#pragma once

// 前方宣言
class Player;

class EnemyBullet
{
public:

	EnemyBullet(void);				// コンストラクタ
	~EnemyBullet(void);				// デストラクタ

	void Init(void);				// 初期化
	void Update(void);				// 更新
	void Draw(void);				// 描画

	// 弾の発射
	void Shoot(int startX, int startY);

	// 弾が存在するか取得
	bool IsActive(void)const;

	// 弾を消す
	void Disable(void);

	// X座標取得
	int GetX(void)const;

	// Y座標取得
	int GetY(void)const;

	// 弾の当たり判定
	bool CheckHitPlayer(const Player&player);

private:

	// 弾のX座標
	int x_;

	// 弾のY座標
	float y_;

	// 弾の移動速度
	int speed_;

	// 弾の生存フラグ
	bool active_;

	// 弾の移動速度
	static constexpr int BULLET_SPEED = 3;

	// 弾の横サイズ（半分）
	static constexpr int BULLET_HALF_WIDTH = 2;

	// 弾の縦サイズ（半分）
	static constexpr int BULLET_HALF_HEIGHT = 8;

	// 弾の色
	static constexpr int BULLET_COLOR_R = 255;
	static constexpr int BULLET_COLOR_G = 255;
	static constexpr int BULLET_COLOR_B = 255;
};