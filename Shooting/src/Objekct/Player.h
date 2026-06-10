#pragma once

class Player
{
public:
	 Player(void);			// コンストラクタ
	 ~Player(void);			// デストラクタ

	 void Init(void);		// 初期化
	 void Update(void);		// 更新
	 void Draw(void);		// 描画
	 void Release(void);	// 解放

private:

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
};

