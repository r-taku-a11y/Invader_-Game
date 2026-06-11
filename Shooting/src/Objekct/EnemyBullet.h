#pragma once
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

private:

	// 弾のX座標
	int x_;

	// 弾のY座標
	int y_;

	// 弾の移動速度
	int speed_;

	// 弾の生存フラグ
	bool active_;
};

