#pragma once
#include "Player.h"

class Item
{ 
public:

	Item(void);			// コンストラクタ
	~Item(void);		// デストラクタ

	// アイテムの種類あ
	enum ITEM_TYPE
	{
		ITEM_NONE,		// アイテム無し
		ITEM_SHIELD,	// シールド回復
		ITEM_LIFE,		// 残機の回復
		ITEM_POWER,		// 弾数アップ
		ITEM_SCORE		// スコア加算
	};

	// 初期化
	void Init(void);

	// 更新
	void Update(void);	

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// アイテム出現
	void Spawn(float x, float y, ITEM_TYPE type);

	// アクティブ状態取得
	void Destroy(void);

	// アイテム種類取得
	bool IsActive(void) const;

	// アイテム種類取得
	ITEM_TYPE GetType(void) const;

	// X座標取得
	float GetX(void) const;

	// Y座標取得
	float GetY(void) const;

	// プレイヤーとの当たり判定
	bool CheckHit(const Player& player);

private:

	// アイテムの落下速度
	static constexpr float ITEM_SPEED = 3.0f;

	// 座標
	float x_;
	float y_;

	// 落下速度
	float speed;

	// アイテム種類
	ITEM_TYPE type_;

	// 出現
	bool active_;

};

