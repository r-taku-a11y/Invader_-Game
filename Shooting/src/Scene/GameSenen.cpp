#include <DxLib.h>
#include "GameSenen.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Application.h"

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

	// 受信
	network.Update();

	// タイマーの初期化
	sendTimer_ = 0;

	// プレイヤーの初期化
	player.Init();

	// 現在のボタン状態を同期
	player.ResetButtonState(network.GetButton());

	// ラウンドの初期化
	round.Init();

	// 敵の初期化
	enemy.Init(round.GetRound());

	// シールドの生成
	ResetShield();

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の読み込み
	seMana_->LoadBGM("GameSenen", "Data/Sound/BGM/Game.mp3", false);
	
}

// 更新
void GameSenen::Update(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	// Arduinoから受信
	network.Update();
	// プレイヤーの更新
	player.Update(network);
	// 敵の更新
	enemy.Update();
	// BGMの音の設定
	seMana_->SetVolumeBGM("GameSenen", BGM_VOLUME);

	// シールドと敵弾の当たり判定
	for (auto& bullet : enemy.GetEnemyBulletList())
	{
		// すでに消えてるなら弾は判定しない
		if (!bullet.IsActive())
		{
			continue;
		}

		// 全てのシールドとの当たり判定を行う
		for (auto& shield : shiedList_)
		{
			// シールドに当たった
			if (shield.checkHit(bullet.GetX(),bullet.GetY()))
			{
				// シールドに当たった敵弾を消す
				bullet.Disable();
				break;
			}
		}
	}

	// プレイヤー弾とシールドの当たり判定
	if (player.GetBullet().IsActive())
	{
		// 全てのシールドと当たり判定を行う
		for (auto& shield : shiedList_)
		{
			// シールドに当たった
			if (shield.checkHit(player.GetBullet().GetX(),player.GetBullet().GetY()))
			{
				// シールドに当たったプレイヤーの弾を消す
				player.GetBullet().Disable();
				break;
			}
		}
	}

	// 敵と弾の当たり判定
	enemy.CheckHit(player.GetBullet());

	// 敵の弾とプレイヤーの当たり判定
	if (!player.IsInvincible())
	{
		if (enemy.CheckHitPlayer(player))
		{

			// 残機を減らす
			player.Damage();

			if (player.GetLife() <= 0)
			{
				// 音の停止
				seMana_->StopBGM("GameSenen");
				// GAME OVER音をArduinoへ通知
				NetworkManager::GetInstance().SendGameInfo(enemy.GetHiScore(),enemy.GetScore(),player.GetLife(),2);
				// ゲームオーバーへ遷移
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
				return;
			}

		}
	}
	
	// 敵が防衛ライン到達
	if (enemy.IsReachedGameOverLine())
	{
		// 音の停止
		seMana_->StopBGM("GameSenen");
		// GAME OVER音をArduinoへ通知
		NetworkManager::GetInstance().SendGameInfo(enemy.GetHiScore(), enemy.GetScore(), player.GetLife(), 2);
		// ゲームオーバーへ遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		return;
	}

	// 敵を全て倒した
	if (enemy.IsAllDead())
	{
		// まだ最終ラウンドではない
		if (!round.IsFinalRound())
		{
			// 次のラウンドに進む
			round.NextRound();

			// 次のラウンドの敵の生成
			enemy.Init(round.GetRound());

			// シールド復活
			ResetShield();

			return;
		}
		// 音の停止
		seMana_->StopBGM("GameSenen");
		// GAME CLEAR音をArduinoへ通知
		NetworkManager::GetInstance().SendGameInfo(enemy.GetHiScore(),enemy.GetScore(),player.GetLife(),1);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		return;
	}

	// 毎フレーム送らず、軽く制御する
	sendTimer_++;

	if (sendTimer_ >= SEND_INTERVAL)
	{
		sendTimer_ = 0;

		// スコア変化時のみ内部で送信される
		// Arduino OLEDへゲーム情報送信
		NetworkManager::GetInstance().SendGameInfo(enemy.GetHiScore(), enemy.GetScore(), player.GetLife(),0);
	}

	// BGMの再生
	seMana_->PlayBGM("GameSenen");

}

// 描画
void GameSenen::Draw(void)
{
	// プレイヤー描画
	player.Draw();

	// 敵の描画
	enemy.Draw();

	// ハイスコアの表示
	DrawFormatString(10, 10, GetColor(255, 255, 255), "HI SCORE : %d", enemy.GetHiScore());

	// スコア表示
	DrawFormatString(150, 10, GetColor(255, 255, 255), "SCORE : %05d", enemy.GetScore());

	// 残機の表示
	DrawString(LIFE_TEXT_X, 10, "LIFE", GetColor(255, 255, 255));

	// 現在のラウンドの表示
	DrawFormatString(ROUND_TEXT_X, 10, GetColor(255, 255, 255), "ROUND:%d", round.GetRound());

	// シールド描画
	for (auto& shield : shiedList_)
	{
		shield.Draw();
	}

	// 残機アイコンの描画
	for (int i = 0; i < player.GetLife();i++)
	{
		DrawExtendGraph(LIFE_ICON_START_X + i * LIFE_ICON_INTERVAL,5, LIFE_ICON_START_X + i * LIFE_ICON_INTERVAL + LIFE_ICON_SIZE, 5 + LIFE_ICON_SIZE, player.GetModelHandle(), TRUE);
	}

	// デバック
	//DrawLine(0,500,800,500,GetColor(255, 0, 0));

	//DrawFormatString(10, 100, GetColor(255, 255, 255), "X:%d", network.GetStickX());

	//DrawFormatString(10, 300, GetColor(255, 255, 255), "Y:%d", network.GetStickY());

	//DrawFormatString(10, 500, GetColor(255, 255, 255), "BTN:%d", network.GetButton());
}

// 解放
void GameSenen::Release(void)
{

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();
	// 音の停止
	seMana_->StopBGM("GameSenen");
	// 音の開放
	seMana_->ReleaseSound("GameSenen");

	// ネットワークの開放
	network.Release();
	// プレイヤーの解放
	player.Release();
	// 敵の開放
	enemy.Release();
}

// シールド再生成
void GameSenen::ResetShield(void)
{
	// 一旦空にする
	shiedList_.clear();

	// ラウンドごとに変更
	int shieldCount = SHIELD_COUNT_TABLE[round.GetRound() - 1];

	// 等間隔に配置させる
	float spacing = static_cast<float>(Application::SCREEN_SIZE_X) / (shieldCount + 1);

	for (int i = 0; i < shieldCount; i++)
	{
		float x = spacing * (i + 1) - SHIELD_CENTER_OFFSET;

		shield.Init(x, SHIELD_Y);
		shiedList_.push_back(shield);
	}
}
