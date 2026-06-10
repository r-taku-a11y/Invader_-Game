#include <DxLib.h>
#include "GameSenen.h"

// コンストラクタ
GameSenen::GameSenen(void)
{
}

// デストラクタ
GameSenen::~GameSenen(void)
{
}

// 初期化
void GameSenen::Init(void)
{
	// UDP通信開始
	network.Init();
	// プレイヤーの初期化
	player.Init();
	// 敵の初期化
	enemy.Init();
}

// 更新
void GameSenen::Update(void)
{
	// プレイヤーの更新
	player.Update();
	// 敵の更新
	enemy.Update();
}

// 描画
void GameSenen::Draw(void)
{
	// プレイヤー描画
	player.Draw();
	// 敵の描画
	enemy.Draw();
}

// 解放
void GameSenen::Release(void)
{
	// プレイヤーの解放
	player.Release();
	// 敵の開放
	enemy.Release();
}
