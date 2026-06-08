#pragma once
class Bullet
{
public:
	Bullet(void);				// コンストラクタ
	~Bullet(void);				// デストラクタ

	void Init(void);			// 初期化
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void Shoot(int startX, int startY);			// 弾の発射
	bool IsActive(void)const;	// 弾の生存しているか
	void Disable(void);			// 弾の無効化

	// X座標
	int GetX(void)const;
	// Y座標
	int GetY(void)const;

private:

	// 弾の座標
	int x_;
	float y_;

	// 生存フラグ
	bool active_;

	// 弾の速度
	int speed_;

	// 移動速度
	static constexpr int MOVE_SPEED = 5;

	// 画面上端
	static constexpr int SCREEN_TOP = 0;

	// 弾の描画サイズ
	static constexpr int BULLET_DRAW_TOP = 10;
	static constexpr int BULLET_DRAW_BOTTOM = 8;

};