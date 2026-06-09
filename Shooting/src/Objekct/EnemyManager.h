#pragma once
#include<vector>
#include"Enemy.h"

class EnemyManager
{
public:
	EnemyManager(void);				// デストラクタ
	~EnemyManager(void);			// コンストラクタ

	void Init(void);				// 初期化
	void Update(void);				// 更新
	void Draw(void);				// 描画
	void Release(void);				// 解放

private:

	// 敵の一覧
	std::vector<Enemy>enemyList_;

	// 移動方向
	bool moveRight_;

	// 横移動速度
	float moveSpeed_;

	// 下へ降りる量
	float moveDown_;

};

