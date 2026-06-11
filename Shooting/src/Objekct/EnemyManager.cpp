#include "EnemyManager.h"
#include "../Application.h"

// デストラクタ
EnemyManager::EnemyManager(void)
{
}

// コンストラクタ
EnemyManager::~EnemyManager(void)
{
}

// 初期化
void EnemyManager::Init(void)
{
	// 右方向へ移動
	moveRight_ = true;

	// 横移動速度
	moveSpeed_ = 0.5f;

	// 発射タイマー初期化
	shotTimer_ = 0;

	// 壁に当たった時に下へ移動する量
	moveDown_ = 20.0f;

	// 敵リストを空にする
	enemyList_.clear();

	// 敵の弾のリストを空にする
	enemyBulletList_.clear();

	// 赤色の敵(1列目)
	for (int i = 0; i < 8; i++)
	{
		Enemy enemy;
		// X座標を横方向に並べる
		// Y座標は50固定
		enemy.Init(Enemy::TYPE::RED, 20.0f + i * 60.0f, 20.0f);
		enemyList_.push_back(enemy);
	}

	// オレンジ色の敵(2列目)
	for (int i = 0; i < 8; i++)
	{
		Enemy enemy;
		enemy.Init(Enemy::TYPE::ORANGE, 20.0f + i * 60.0f, 70.0f);
		enemyList_.push_back(enemy);
	}

	// 白色の敵(3列目)
	for (int i = 0; i < 8; i++)
	{
		Enemy enemy;
		enemy.Init(Enemy::TYPE::WHITE, 20.0f + i * 60.0f, 120.0f);
		enemyList_.push_back(enemy);
	}

	// 黄色の敵(4列目)
	for (int i = 0; i < 8; i++)
	{
		Enemy enemy;
		enemy.Init(Enemy::TYPE::YELLOW, 20.0f + i * 60.0f, 170.0f);
		enemyList_.push_back(enemy);
	}
}

// 更新
void EnemyManager::Update(void)
{
	// 生存している敵の左右を取得
	float leftMost = 99999.0f;
	float rightMost = -99999.0f;

	for (auto& enemy : enemyList_)
	{
		// 撃破済なら無視する
		if (!enemy.IsAlive())
		{
			continue;
		}

		// 左端の敵を取得
		if (enemy.GetPosX() < leftMost)
		{
			leftMost = enemy.GetPosX();
		}

		// 右端の敵を取得
		if (enemy.GetPosX() + enemy.GetWidth() > rightMost)
		{
			rightMost = enemy.GetPosX() + enemy.GetWidth();
		}
	}

	// 壁判定

	bool hitWall = false;

	// 右移動中
	if (moveRight_)
	{
		if (rightMost >= Application::SCREEN_SIZE_X)
		{
			hitWall = true;
		}
	}
	else
	// 左移動中
	{
		if (leftMost <= 0)
		{
			hitWall = true;
		}
	}

	// 壁に当たった

	if (hitWall)
	{
		// 移動方向反転
		moveRight_ = !moveRight_;

		// 敵全体を1段下へ移動
		for (auto& enemy : enemyList_)
		{
			if (!enemy.IsAlive())
			{
				continue;
			}

			enemy.SetPosY(enemy.GetPosY() + moveDown_);
		}
	}
	// 通常移動
	else
	{
		// 現在の移動方向に応じて
		// 移動量を決定する
		float move = moveRight_ ? moveSpeed_ : -moveSpeed_;

		for (auto& enemy : enemyList_)
		{
			if (!enemy.IsAlive())
			{
				continue;
			}

			enemy.SetPosX(enemy.GetPosX() + move);
		}
	}

	// 敵の弾更新
	for (auto& bullet : enemyBulletList_)
	{
		bullet.Update();
	}

	// タイマー加算
	shotTimer_++;

	// 60フレームごとに発射
	if (shotTimer_ >= 60)
	{
		ShotEnemyBullet();
		shotTimer_ = 0;
	}
}

// 描画
void EnemyManager::Draw(void)
{
	// 敵のスピードのデバック用の表示
	//DrawFormatString(10, 10, GetColor(255, 255, 255), "Enemy Speed : %.2f", moveSpeed_);

	for (auto& enemy : enemyList_)
	{
		// 撃破済みは描画しない
		if (!enemy.IsAlive())
		{
			continue;
		}

		enemy.Draw();
	}

	for (auto& bullet : enemyBulletList_)
	{
		bullet.Draw();
	}
}

// 解放
void EnemyManager::Release(void)
{
	for (auto& enemy : enemyList_)
	{
		// 敵画像などを解放
		enemy.Release();
	}

	// 敵リストを空にする
	enemyList_.clear();
	// 敵の弾の削除
	enemyBulletList_.clear();
}

// 弾との当たり判定
void EnemyManager::CheckHit(Bullet& bullet)
{
	// 弾が存在しない
	if (!bullet.IsActive())
	{
		return;
	}

	for (auto& enemy : enemyList_)
	{
		// 撃破済みは無視
		if (!enemy.IsAlive())
		{
			continue;
		}

		// 当たり判定
		if (
			bullet.GetX() >= enemy.GetPosX() &&
			bullet.GetX() <= enemy.GetPosX() + enemy.GetWidth() &&
			bullet.GetY() >= enemy.GetPosY() &&
			bullet.GetY() <= enemy.GetPosY() + enemy.GetHeight()
			)
		{
			// 敵撃破
			enemy.kill();

			// 弾の消滅
			bullet.Disable();
			
			// 1発で1体だけ倒す
			break;
		}
	}
}

// 敵弾発射
void EnemyManager::ShotEnemyBullet(void)
{

	// 各列の最下段を保存
	std::vector<Enemy*> bottomEnemies;


	// 8列分
	for (int column = 0; column < 8; column++)
	{
		Enemy* bottomEnemy = nullptr;

		for (auto& enemy : enemyList_)
		{
			// 撃破済み
			if (!enemy.IsAlive())
			{
				continue;
			}

			// この敵が何列目か
			int enemyColumn =
				static_cast<int>((enemy.GetPosX() - 20.0f) / 60.0f);

			if (enemyColumn != column)
			{
				continue;
			}

			// 最下段をさがす
			if (bottomEnemy == nullptr)
			{
				bottomEnemy = &enemy;
			}
			else if (enemy.GetPosY() > bottomEnemy->GetPosY())
			{
				bottomEnemy = &enemy;
			}
		}

		// 見つかったら保存する
		if (bottomEnemy != nullptr)
		{
			bottomEnemies.push_back(bottomEnemy);
		}
	}

	// 発射可能な敵がいない
	if (bottomEnemies.empty())
	{
		return;
	}

	// ランダムな列を選ぶ
	int index = rand() % bottomEnemies.size();

	Enemy* shooter = bottomEnemies[index];

	// 敵弾生成
	EnemyBullet bullet;

	bullet.Init();

	// 発射
	bullet.Shoot(
		static_cast<int>(shooter->GetPosX()) +
		shooter->GetWidth() / 2,

		static_cast<int>(shooter->GetPosY()) + 48
	);

	// リストへ追加
	enemyBulletList_.push_back(bullet);
}

// 全滅したか
bool EnemyManager::IsAllDead(void) const
{
	for (const auto& enemy : enemyList_)
	{
		// 生存している敵がいる
		if (enemy.IsAlive())
		{
			return false;
		}
	}

	// 全滅
	return true;
}
