#pragma once
#include <vector>
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Bullet.h"
#include "Player.h"

class EnemyManager
{
public:
	EnemyManager(void);				// デストラクタ
	~EnemyManager(void);			// コンストラクタ

	void Init(void);				// 初期化
	void Update(void);				// 更新
	void Draw(void);				// 描画
	void Release(void);				// 解放


	// 弾との当たり判定
	void CheckHit(Bullet& bullet);

	// 敵弾を発射する
	void ShotEnemyBullet(void);

	// 敵が全滅したかどうか
	bool IsAllDead(void)const;

	// 敵弾とプレイヤーの当たり判定
	bool CheckHitPlayer(const Player& player);

private:

	// 発射間隔管理
	int shotTimer_;

	std::vector<EnemyBullet> enemyBulletList_;

	// 敵の一覧
	std::vector<Enemy>enemyList_;

	// 移動方向
	bool moveRight_;

	// 横移動速度
	float moveSpeed_;

	// 下へ降りる量
	float moveDown_;

};

