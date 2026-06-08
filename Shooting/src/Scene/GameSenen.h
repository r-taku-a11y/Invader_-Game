#pragma once
#include <vector>
#include "../Objekct/Player.h"
#include "../Objekct/EnemyManager.h"
#include "../Objekct/EnemyBullet.h"
#include "../Objekct/Shield.h"
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

	// Arduino通信
	NetworkManager& network = NetworkManager::GetInstance();

	// 送信用タイマー
	int sendTimer_ ;

	// シールドを作り直す
	void ResetShield(void);

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

	// ラウンドごとのシールド数
	static constexpr int SHIELD_COUNT_TABLE[5] =
	{
		8, 7, 6, 5, 4
	};

};

