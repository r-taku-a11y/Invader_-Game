#pragma once
class RoundManager
{
public:

	RoundManager(void);			// コンストラクタ
	~RoundManager(void);		// デストラクタ

	void Init(void);			// 初期化
	void NextRound(void);		// 次のラウンドに進む
	int GetRound(void)const;	// 現在のラウンドの取得
	bool IsFinalRound(void)const;	// 最終ラウンドの判定

private:

	// 現在のラウンド
	int currentRound_;

	// 最大ラウンド数
	static const int MAX_ROUND = 5;
};

