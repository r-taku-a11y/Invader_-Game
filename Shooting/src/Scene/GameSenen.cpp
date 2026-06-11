#include <DxLib.h>
#include "GameSenen.h"
#include "../Manager/SceneManager.h"

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
	// Arduinoから受信
	network.Update();
	// プレイヤーの更新
	player.Update(network);
	// 敵の更新
	enemy.Update();

	// 敵と弾の当たり判定
	enemy.CheckHit(player.GetBullet());

	// 敵を全て倒した
	if (enemy.IsAllDead())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		return;
	}
}

// 描画
void GameSenen::Draw(void)
{
	DrawFormatString(10,100,GetColor(255, 255, 255),"X:%d",network.GetStickX());

	DrawFormatString(10,300,GetColor(255, 255, 255),"Y:%d",network.GetStickY());

	DrawFormatString(10,500,GetColor(255, 255, 255),"BTN:%d",network.GetButton());

	// プレイヤー描画
	player.Draw();
	// 敵の描画
	enemy.Draw();
}

// 解放
void GameSenen::Release(void)
{
	network.Release();
	// プレイヤーの解放
	player.Release();
	// 敵の開放
	enemy.Release();
}
