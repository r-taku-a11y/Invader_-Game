#pragma once
class BonusEnemy
{
public:
	BonusEnemy(void);		// コンストラクタ
	~BonusEnemy(void);		// デストラクタ

	void Init(void);		// 初期化
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放処理

	// 出現
	void Spawn(bool fromLeft);	

	// 撃破
	void Kill(void);

	// 当たり判定
	bool CheckHit(int bulletX,int bulletY);

	// 生存判定
	bool IsAlive(void)const;

private:

	// 出現位置Y
	static constexpr float SPAWN_Y = 10.0f;

	// UFOの描画サイズ
	static constexpr int DRAW_WIDTH = 100;
	static constexpr int DRAW_HEIGHT = 50;

	// 移動速度
	static constexpr float MOVE_SPEED = 100.0f;

	// 画像
	int modelHandle_;

	// 座標
	float x_;
	float y_;

	// 移動速度
	float speed_;

	// 生存状態
	bool alive_;

	// 左側から出現したか
	bool fromLeft_;

	// 横サイズ
	int width_;

	// 縦サイズ
	int height_;

	// 実際の描画サイズ
	int drawWidth_;
	int drawHeight_;
};

