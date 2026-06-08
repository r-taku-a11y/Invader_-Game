#pragma once


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

	// デバックの描画
	void DrawDebug(void);

private:

};

