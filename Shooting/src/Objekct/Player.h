#pragma once
#include "Bullet.h"

// クラスの前方宣言
class NetworkManager;

class Player
{
public:
	 Player(void);			// コンストラクタ
	 ~Player(void);			// デストラクタ

	 void Init(void);		// 初期化
	 void Update(NetworkManager& network);		// 更新
	 void Draw(void);		// 描画
	 void Release(void);	// 解放


	 // 残機を減らす
	 void Damage(void);

	 // 無敵中判定
	 bool IsInvincible(void)const;

	 // ボタン状態
	 void ResetButtonState(bool currentState);

	 // ゲット関数
	 Bullet& GetBullet(void);	// 弾
	 float GetX(void)const;		// プレイヤーのX座標
	 float GetY(void)const;		// プレイヤーのY座標
	 int GetWidth(void)const;	// 横幅
	 int GetHeight(void)const;	// 高さ
	 int GetLife(void)const;	// 残機の取得
	 int GetModelHandle(void)const;	// プレイヤーモデルの取得

private:

	// 弾クラス
	Bullet bullet;

	// 画像ハンドル
	int PlayerModel_;

	// 座標
	float PosX_;
	float PosY_;

	// 移動スピード
	float moveSpeed_;

	// 画像サイズ
	int width;			// 横
	int height;			// 高さ

	// ボタン状態保存用
	bool prevButton_;

	// 弾発射処理
	void shoot();

	// 残機
	int life_;

	// 無敵状態フラグ
	bool invincible_;

	// 無敵時間のタイマー
	float incincbleTimer_;

	// 点滅タイマー
	float blinkTimer_;

	// プレイヤー設定
	static constexpr float MOVE_SPEED = 150.0f;

	// ジョイスティック関連
	static constexpr int STICK_DEADZONE_MIN = 480;
	static constexpr int STICK_DEADZONE_MAX = 560;
	static constexpr int STICK_LEFT_BORDER = 523;
	static constexpr int STICK_RIGHT_BORDER = 700;

	// 初期残機
	static constexpr int START_LIFE = 5;

	// 無敵時間
	static constexpr float INVINCIBLE_TIME = 3.0f;

	// 点滅速度
	static constexpr int BLINK_INTERVAL = 4;

	// 音量
	static constexpr int SHOOT_VOLUME = 255;

	// プレイヤー配置
	static constexpr float PLAYER_Y_OFFSET = 300.0f;

	// 弾発射位置補正
	static constexpr int BULLET_Y_OFFSET = 5;
};

