#include "RoundManager.h"

// コンストラクタ
RoundManager::RoundManager(void)
{
}

// デストラクタ
RoundManager::~RoundManager(void)
{
}

// 初期化
void RoundManager::Init(void)
{
	// ラウンド1から開始
	currentRound_ = 1;
}

// 次のラウンドに進ませる
void RoundManager::NextRound(void)
{
	// 最大ラウンド未満なら進める
	if (currentRound_ < MAX_ROUND)
	{
		currentRound_++;
	}
}

// 現在ラウンド取得
int RoundManager::GetRound(void) const
{
	return currentRound_;
}

// 最終ラウンドの判定
bool RoundManager::IsFinalRound(void) const
{
	return currentRound_ >= MAX_ROUND;
}
