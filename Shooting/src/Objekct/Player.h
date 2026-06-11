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


	 // ゲット関数
	 Bullet& GetBullet(void);

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
	int height;			// 縦

	// ボタン状態保存用
	bool prevButton_;

	// 弾発射処理
	void shoot();
};

