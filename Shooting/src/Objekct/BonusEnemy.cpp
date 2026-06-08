#include <DxLib.h>
#include "BonusEnemy.h"
#include "../Application.h"
#include "../Manager/SoundManager.h"

// コンストラクタ
BonusEnemy::BonusEnemy(void)
{
	// 画像ハンドルの初期化
	modelHandle_ = -1;

	// 座標
	x_ = 0;
	y_ = 0;

	// 速度
	speed_ = MOVE_SPEED;

	// 生存
	alive_ = false;

	// サイズ
	width_ = 0;
	height_ = 0;

	// 画像サイズ
	drawWidth_ = DRAW_WIDTH;
	drawHeight_ = DRAW_HEIGHT;
}

// デストラクタ
BonusEnemy::~BonusEnemy(void)
{
}

// 初期化
void BonusEnemy::Init(void)
{
	// 再度読み込みがないようにする
	if (modelHandle_ == -1)
	{
		// ボーナス機画像読み込み
		modelHandle_ = LoadGraph("Data/Images/UFO.png");
	}

	// サイズ取得
	GetGraphSize(modelHandle_, &width_, &height_);

	// フラグの初期化
	alive_ = false;

	// 左出現フラグ
	fromLeft_ = true;

	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の読み込み
	seMana_->LoadBGM("UFO", "Data/Sound/BGM/Ufo.mp3", false);
	seMana_->LoadSE("Destroy","Data/Sound/SE/Destroy.mp3", false);
}

// 更新
void BonusEnemy::Update(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// フレーム時間取得
	float deltaTime = Application::GetInstance().GetDeltaTime();

	// 出現していない
	if (!alive_)return;

	// 左→右
	if (fromLeft_)
	{
		// 音の再生
		seMana_->PlayBGM("UFO");
		x_ += speed_ * deltaTime;

		// 画面外
		if (x_ > Application::SCREEN_SIZE_X)
		{
			// 音の再生
			seMana_->StopBGM("UFO");
			alive_ = false;
		}
	}
	// 右→左
	else
	{
		// 音の再生
		seMana_->PlayBGM("UFO");
		x_ -= speed_ * deltaTime;

		// 画面外
		if (x_ + width_ < 0)
		{
			// 音の再生
			seMana_->StopBGM("UFO");
			alive_ = false;
		}
	}

}

// 描画
void BonusEnemy::Draw(void)
{
	if (!alive_)return;

	DrawExtendGraph((int)x_,(int)y_,(int)x_ + drawWidth_,(int)y_ + drawHeight_,modelHandle_,TRUE);

}

// 開放処理
void BonusEnemy::Release(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 音の停止
	seMana_->StopBGM("UFO");
	seMana_->StopSE("Destroy");

	// 音の開放
	seMana_->ReleaseSound("UFO");
	seMana_->ReleaseSound("Destroy");

	// 画像が存在する場合
	if (modelHandle_ != -1)
	{
		// 画像解放
		DeleteGraph(modelHandle_);

		// 無効化
		modelHandle_ = -1;
	}
}

// 出現処理
void BonusEnemy::Spawn(bool fromLeft)
{
	// 出現
	alive_ = true;

	// 出現方向保存
	fromLeft_ = fromLeft;

	// 上側に出現
	y_ = SPAWN_Y;;

	// 左から
	if (fromLeft)
	{
		// 左画面外から出現
		x_ = -drawWidth_;
	}
	// 右から
	else
	{
		x_ = Application::SCREEN_SIZE_X;
	}
}

// 撃破
void BonusEnemy::Kill(void)
{
	// インスタンス取得
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 撃破音の再生
	seMana_->SetVolumeSE("Destroy", 255);
	seMana_->PlaySE("Destroy");

	// 音の停止
	seMana_->StopBGM("UFO");

	// 撃破状態
	alive_ = false;
}

// 当たり判定
bool BonusEnemy::CheckHit(int bulletX, int bulletY)
{
	if (!alive_)
	{
		return false;
	}

	if (bulletX >= x_ &&bulletX <= x_ + drawWidth_ &&bulletY >= y_ &&bulletY <= y_ + drawHeight_)
	{
		Kill();
		return true;
	}

	return false;
}

// 生存フラグ
bool BonusEnemy::IsAlive(void) const
{
	return alive_;
}
