#pragma once
#include <vector>
#include "../Objekct/Player.h"
#include "../Objekct/EnemyManager.h"
#include "../Objekct/EnemyBullet.h"
#include "../Objekct/Shield.h"
#include "../Objekct/Item.h"
#include "../Manager/NetworkManager.h"
#include "../Manager/RoundManager.h"

class GameSenen
{
public:
	GameSenen(void);			// コンストラクタ
	~GameSenen(void);			// デストラクタ

	// 初期化処理
	void Init(void);			// 初期化
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void Release(void);			// 解放

private:

	// クラス
	Player player;
	EnemyManager enemy;
	EnemyBullet enbullet;
	std::vector<Shield>shiedList_;
	Shield shield;
	RoundManager round;
	Item item;

	// Arduino通信
	NetworkManager& network = NetworkManager::GetInstance();

	// アイテム出現処理
	void SpawnItem(void);

	// 送信用タイマー
	int sendTimer_ ;

	// シールドを作り直す
	void ResetShield(void);

	// シールドが壊れているか判定
	bool IsShieldBroken(void)const;

	// BGMの音量
	static constexpr int BGM_VOLUME = 120;

	// Arduino送信間隔
	static constexpr int SEND_INTERVAL = 10;

	// UI表示
	static constexpr int LIFE_TEXT_X = 540;
	static constexpr int ROUND_TEXT_X = 350;

	// 残機アイコン
	static constexpr int LIFE_ICON_START_X = 620;
	static constexpr int LIFE_ICON_INTERVAL = 35;
	static constexpr int LIFE_ICON_SIZE = 24;

	// シールド
	static constexpr float SHIELD_Y = 450.0f;
	static constexpr float SHIELD_CENTER_OFFSET = 20.0f;

	// スコアアイテムの加算点
	static constexpr int ITEM_SCORE_POINT = 100;

	// 出現するアイテム数
	static constexpr int ITEM_TYPE_COUNT = 4;

	// スコアアイテムの出現重み
	// 数字が大きいほど出現しやすくなる
	static constexpr int SCORE_WEIGHT = 5;

	// パワーアップアイテムの出現重み
	static constexpr int POWER_WEIGHT = 2;

	// シールド回復アイテムの出現重み
	static constexpr int SHIELD_WEIGHT = 1;

	// 残機回復アイテムの出現重み
	static constexpr int LIFE_WEIGHT = 1;

	// アイテム出現確率（％）
	static constexpr int ITEM_SPAWN_RATE = 20;

	// 乱数の最大値
	static constexpr int RANDOM_MAX = 100;

	// ラウンドごとのシールド数
	static constexpr int SHIELD_COUNT_TABLE[5] =
	{
		8, 7, 6, 5, 4
	};

};

