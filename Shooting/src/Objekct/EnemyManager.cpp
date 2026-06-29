#include "EnemyManager.h"
#include "../Application.h"
#include "../Manager/SoundManager.h"

// ラウンド設定テーブル
static const RoundData ROUND_DATA[] =
{
	// ダミー(0番は未使用)
	{0.0f,0.0f,0,0,0},

	// Round 1
	{0.3f,4.0f,4,2,10},

	// Round 2
	{0.35f,3.5f,6,3,30},

	// Round 3
	{0.4f,3.0f,8,4,40},

	// Round 4
	{0.45f,2.5f,10,5,50},

	// Round 5
	{0.5f,2.0f,12,6,60}
};

// コンストラクタ
EnemyManager::EnemyManager(void)
{
	// ハイスコアの初期化
	hiScore_ = 0;

	// スコアの初期化
	score_ = 0;
}

// デストラクタ
EnemyManager::~EnemyManager(void)
{
}

// 初期化
void EnemyManager::Init(int round)
{
	// 画像のロード
	Enemy::LoadResource();

	// 右方向へ移動
	moveRight_ = true;

	// ボーナス機未出現
	bonusSpawned_ = false;

	// 横移動速度
	moveSpeed_ = DEFAULT_MOVE_SPEED;

	// 発射タイマー初期化
	shotTimer_ = 0;

	// 敵の弾
	shotCount_ = 1;

	// 前回発射なし
	lastShotColumn_ = -1;

	// アイテム未出現
	itemSpawn_ = false;

	// 出現座標初期化
	itemSpawnX_ = 0.0f;
	itemSpawnY_ = 0.0f;

	// ラウンド1ならスコアを初期化
	if (round == 1)
	{
		// スコアの初期化
		score_ = 0;
	}

	// 壁に当たった時に下へ移動する量
	moveDown_ = MOVE_DOWN_DISTANCE;

	// 敵リストを空にする
	enemyList_.clear();

	// 敵の弾のリストを空にする
	enemyBulletList_.clear();

	// 敵のデフォルトの数
	enemyCount_ = ENEMY_COUNT;

	// ボーナス機初期化
	bonusEnemy_.Init();

	// 敵の下降回数初期化
	moveDownCount_ = 0;

	// ラウンド範囲チェック
	if (round < 1)
	{
		round = 1;
	}

	if (round > MAX_ROUND)
	{
		round = MAX_ROUND;
	}

	// ラウンドデータ取得
	const RoundData& data = ROUND_DATA[round];

	// 設定反映
	moveSpeed_ = data.moveSpeed;
	shotInterval_ = data.shotInterval;
	enemyCount_ = data.enemyCount;
	shotCount_ = data.shotCount;
	bonusChance_ = data.bonusChance;

	// 赤色の敵(1列目)
	for (int i = 0; i < enemyCount_; i++)
	{
		Enemy enemy;
		// X座標を横方向に並べる
		// Y座標は50固定
		enemy.Init(Enemy::TYPE::RED, ENEMY_START_X + i * ENEMY_INTERVAL_X, ENEMY_START_Y_RED);
		// この敵が所属する列番号を保存
		enemy.SetColumn(i);
		enemyList_.push_back(enemy);
	}

	// オレンジ色の敵(2列目)
	for (int i = 0; i < enemyCount_; i++)
	{
		Enemy enemy;
		enemy.Init(Enemy::TYPE::ORANGE, ENEMY_START_X + i * ENEMY_INTERVAL_X, ENEMY_START_Y_ORANGE);
		// この敵が所属する列番号を保存
		enemy.SetColumn(i);
		enemyList_.push_back(enemy);
	}

	// 白色の敵(3列目)
	for (int i = 0; i < enemyCount_; i++)
	{
		Enemy enemy;
		enemy.Init(Enemy::TYPE::WHITE, ENEMY_START_X + i * ENEMY_INTERVAL_X, ENEMY_START_Y_WHITE);
		// この敵が所属する列番号を保存
		enemy.SetColumn(i);
		enemyList_.push_back(enemy);
	}

	// 黄色の敵(4列目)
	for (int i = 0; i < enemyCount_; i++)
	{
		Enemy enemy;
		enemy.Init(Enemy::TYPE::YELLOW, ENEMY_START_X + i * ENEMY_INTERVAL_X, ENEMY_START_Y_YELLOW);
		// この敵が所属する列番号を保存
		enemy.SetColumn(i);
		enemyList_.push_back(enemy);
	}

	// インスタンスの取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	
	// 音のロード
	seMana_->LoadSE("Bullet", "Data/Sound/SE/Enemy.mp3", false);
	seMana_->LoadSE("Kill", "Data/Sound/SE/Enemyextinction.mp3", false);
}

// 更新
void EnemyManager::Update(void)
{

	// インスタンスの取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 生存している敵の左右を取得
	float leftMost = INIT_LEFT_MOST;
	float rightMost = INIT_RIGHT_MOST;

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
		// まだボーナス機が出現していない場合
		if (!bonusSpawned_)
		{
			// 0～99の乱数を作成
			int chance = rand() % BONUS_RANDOM_MAX;

			// bonusChance_% の確率で出現
			if (chance < bonusChance_)
			{
				// 左右どちらから出現するか決定
				bool fromLeft = (rand() % BONUS_DIRECTION_RANDOM == 0);

				// ボーナス機出現
				bonusEnemy_.Spawn(fromLeft);
			}

			// 抽選した時点でフラグを立てる
			bonusSpawned_ = true;
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

			enemy.SetPosX(enemy.GetPosX() + move * Application::GetInstance().GetDeltaTime() * 60.0f);
		}
	}

	// 敵の弾更新
	for (auto& bullet : enemyBulletList_)
	{
		bullet.Update();
	}

	// 画面外などから消えた敵の弾を削除
	enemyBulletList_.erase(std::remove_if(enemyBulletList_.begin(), enemyBulletList_.end(), [](const EnemyBullet& bullet)
		{
			return !bullet.IsActive();
		}
	),
		enemyBulletList_.end()
	);

	// フレーム時間取得
	float deltaTime = Application::GetInstance().GetDeltaTime();

	// タイマー加算
	shotTimer_ += deltaTime;

	// 発射
	if (shotTimer_ >= shotInterval_)
	{
		// ラウンドごとの発射数だけ発射
		for (int i = 0; i < shotCount_; i++)
		{
			ShotEnemyBullet();
		}

		// 音の再生
		seMana_->PlaySE("Bullet");
		shotTimer_ = 0;
	}

	// ボーナス機更新
	bonusEnemy_.Update();
}

// 描画
void EnemyManager::Draw(void)
{
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

	// ボーナス機描画
	bonusEnemy_.Draw();
}

// 解放
void EnemyManager::Release(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	//サウンドの停止
	seMana_->StopSE("Bullet");
	seMana_->StopSE("Kill");

	// 解放
	seMana_->ReleaseSound("Kill");
	seMana_->ReleaseSound("Bullet");

	for (auto& enemy : enemyList_)
	{
		// 敵画像などを解放
		enemy.Release();
	}

	// ボーナス機解放
	bonusEnemy_.Release();

	// 敵リストを空にする
	enemyList_.clear();
	// 敵の弾の削除
	enemyBulletList_.clear();

	// 敵画像の開放
	Enemy::ReleaseResource();
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

		// ボーナス機との当たり判定
		if (bonusEnemy_.CheckHit(bullet.GetX(),bullet.GetY()))
		{
			// ボーナス得点加算
			score_ += BONUS_SCORE;

			// ハイスコア更新
			if (score_ > hiScore_)
			{
				hiScore_ = score_;
			}

			// プレイヤー弾を消す
			bullet.Disable();

			// ボーナス機だけ処理して終了
			return;
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

			// アイテム出現位置を保存
			itemSpawnX_ = enemy.GetPosX() + enemy.GetWidth() / 2.0f;
			itemSpawnY_ = enemy.GetPosY() + enemy.GetHeight() / 2.0f;

			// アイテム出現要求
			itemSpawn_ = true;

			// 敵の色によって得点の加算
			switch (enemy.GetType())
			{
			// 赤
			case Enemy::TYPE::RED:
				score_ += RED_SCORE;
				break;

			// オレンジ
			case Enemy::TYPE::ORANGE:
				score_ += ORANGE_SCORE;
				break;

			// 白
			case Enemy::TYPE::WHITE:
				score_ += WHITE_SCORE;
				break;

			// 黄色
			case Enemy::TYPE::YELLOW:
				score_ += YELLOW_SCORE;
				break;
			default:
				break;
			}

			// ハイスコアの更新
			if (score_ > hiScore_)
			{
				hiScore_ = score_;
			}

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
	for (int column = 0; column < enemyCount_; column++)
	{
		Enemy* bottomEnemy = nullptr;

		for (auto& enemy : enemyList_)
		{
			// 撃破済み
			if (!enemy.IsAlive())
			{
				continue;
			}

			// 敵が持っている列番号を取得
			int enemyColumn = enemy.GetColumn();

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

	// 発射候補
	std::vector<Enemy*> candidates;

	// 前回発射列以外を候補にする
	for (auto enemy : bottomEnemies)
	{
		// 敵が所属している列
		int column = enemy->GetColumn();

		// 前回発射した列は除外
		if (column != lastShotColumn_)
		{
			candidates.push_back(enemy);
		}
	}

	// 候補が無い場合
	// (最後の1列だけ残った場合など)
	if (candidates.empty())
	{
		candidates = bottomEnemies;
	}

	// ランダム選択
	int index = rand() % candidates.size();

	Enemy* shooter = candidates[index];

	// 今回発射した列を保存
	lastShotColumn_ = shooter->GetColumn();

	// 敵弾生成
	EnemyBullet bullet;

	bullet.Init();

	// 発射
	bullet.Shoot(static_cast<int>(shooter->GetPosX()) +shooter->GetWidth() / 2,static_cast<int>(shooter->GetPosY()) + BULLET_OFFSET_Y);

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

// 弾とプレイヤーの当たり判定
bool EnemyManager::CheckHitPlayer(const Player& player)
{
	// 全ての敵弾を調べる
	for (auto& bullet : enemyBulletList_)
	{
		// 当たった
		if (bullet.CheckHitPlayer(player))
		{
			return true;
		}
	}

	return false;
}

// プレイヤーの位置の手前に敵が来たらゲームオーバーにする(500の位置にする)
bool EnemyManager::IsReachedGameOverLine(void) const
{

	for (const auto& enemy : enemyList_)
	{
		// 撃破済みは無視
		if (!enemy.IsAlive())
		{
			continue;
		}

		// 敵が下端がラインに到達
		if (enemy.GetPosY() + enemy.GetHeight() >= GAME_OVER_LINE)
		{
			return true;
		}
	}

	return false;
}

// 敵弾リスト取得
std::vector<EnemyBullet>& EnemyManager::GetEnemyBulletList()
{
	return enemyBulletList_;
}

// スコアの取得
int EnemyManager::GetScore(void) const
{
	return score_;
}

// ハイスコアの取得
int EnemyManager::GetHiScore(void) const
{
	return hiScore_;
}

// アイテム出現要求
bool EnemyManager::IsItemSpawn(void) const
{
	return itemSpawn_;
}

// X座標取得
float EnemyManager::GetItemSpawnX(void) const
{
	return itemSpawnX_;
}

// Y座標取得
float EnemyManager::GetItemSpawnY(void) const
{
	return itemSpawnY_;
}

// 出現フラグリセット
void EnemyManager::ResetItemSpawn(void)
{
	itemSpawn_ = false;
}

// スコアの加算処理
void EnemyManager::AddScore(int score)
{
	// スコア加算
	score_ += score;

	// ハイスコア更新
	if (score_ > hiScore_)
	{
		hiScore_ = score_;
	}
}
