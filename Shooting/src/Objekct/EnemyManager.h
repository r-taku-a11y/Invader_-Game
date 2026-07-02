#pragma once
#include <vector>
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Bullet.h"
#include "Player.h"
#include "BonusEnemy.h"

// ラウンド設定
struct RoundData
{
	// 敵移動速度
	float moveSpeed;

	// 発射間隔(秒)
	float shotInterval;

	// 敵数
	int enemyCount;

	// 同時発射数
	int shotCount;

	// ボーナス機出現率
	int bonusChance;
};

class EnemyManager
{
public:
	EnemyManager(void);				// コンストラクタ
	~EnemyManager(void);			// デストラクタ

	void Init(int round);			// 初期化
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

	// 敵がゲームオーバーラインまで到達したか
	bool IsReachedGameOverLine(void)const;

	// 敵弾リスト取得
	std::vector<EnemyBullet>& GetEnemyBulletList();

	// アイテム出現要求
	bool IsItemSpawn(void)const;

	// 出現フラグをリセット
	void ResetItemSpawn(void);

	// スコア加算
	void AddScore(int score);

	// スコア取得
	int GetScore(void)const;

	// ハイスコアの取得
	int GetHiScore(void)const;

	// X座標取得
	float GetItemSpawnX(void)const;

	// Y座標取得
	float GetItemSpawnY(void) const;

private:

	// 発射間隔管理
	float shotTimer_;

	std::vector<EnemyBullet> enemyBulletList_;

	// 敵の一覧
	std::vector<Enemy>enemyList_;
	// ボーナス機
	BonusEnemy bonusEnemy_;

	// 移動方向
	bool moveRight_;

	// 横移動速度
	float moveSpeed_;

	// 下へ降りる量
	float moveDown_;

	// ゲームオーバーライン
	const float GAME_OVER_LINE = 500.0f;

	// スコア
	int score_;

	// ハイスコア
	int hiScore_;

	// 発射間隔
	float shotInterval_;

	// 同時発射数
	int shotCount_;

	// 敵の列数
	int enemyCount_;

	// 敵が何回下へ移動したか
	int moveDownCount_;

	// ボーナス機出現確率
	int bonusChance_;

	// 前回発射した列
	int lastShotColumn_;

	// ボーナス機出現済みフラグ
	bool bonusSpawned_;

	// アイテム出現座標取得
	float itemSpawnX_;
	float itemSpawnY_;

	// アイテム出現フラグ
	bool itemSpawn_;

	// 最終ラウンド
	static constexpr int MAX_ROUND = 5;

	// 敵の数
	static constexpr int ENEMY_COUNT = 8;

	// 敵配置
	static constexpr float ENEMY_START_X = 20.0f;
	static constexpr float ENEMY_START_Y_RED = 20.0f;
	static constexpr float ENEMY_START_Y_ORANGE = 70.0f;
	static constexpr float ENEMY_START_Y_WHITE = 120.0f;
	static constexpr float ENEMY_START_Y_YELLOW = 170.0f;
	static constexpr float ENEMY_INTERVAL_X = 60.0f;

	// 移動
	static constexpr float DEFAULT_MOVE_SPEED = 0.5f;
	static constexpr float MOVE_DOWN_DISTANCE = 20.0f;

	// 乱数
	static constexpr int BONUS_RANDOM_MAX = 100;
	static constexpr int BONUS_DIRECTION_RANDOM = 2;

	// 壁判定用
	static constexpr float INIT_LEFT_MOST = 99999.0f;
	static constexpr float INIT_RIGHT_MOST = -99999.0f;

	// 敵弾
	static constexpr int BULLET_OFFSET_Y = 48;

	// 敵の撃破点数
	static constexpr int BONUS_SCORE = 100;
	static constexpr int RED_SCORE = 40;
	static constexpr int ORANGE_SCORE = 30;
	static constexpr int WHITE_SCORE = 20;
	static constexpr int YELLOW_SCORE = 10;

};