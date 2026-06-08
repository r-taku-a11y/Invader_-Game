#pragma once
#include "../Objekct/Player.h"
#include "../Objekct/EnemyManager.h"
#include "../Objekct/EnemyBullet.h"
#include "../Manager/NetworkManager.h"

class GameOver
{
public:

	// メニュー数
	static constexpr int MENU_NUM = 2;

	// メニュー選択番号
	enum MENU
	{
		PLAY_AGAIN = 0,
		RETURN_TITLE
	};

	GameOver(void);				// コンストラクタ
	~GameOver(void);			// デストラクタ

	void Init(void);			// 初期化
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void Release(void);			// 解放

private:

	// クラス(シングルトン)
	NetworkManager& network = NetworkManager::GetInstance();

	// 現在選択中のメニュー
	int selectMenu_;

	// フォントハンドル
	int OverFont_;

	// ボタン
	bool oldButton_;

	// シーン遷移待ちフラグ
	bool changeSceneWait_;

	// SE再生用のフラグ
	bool gameOverSEPlayerd_;

	// フォント
	static constexpr int GAMEOVER_FONT_SIZE = 48;
	static constexpr int GAMEOVER_FONT_THICK = 3;

	// ジョイスティック判定
	static constexpr int STICK_LEFT_BORDER = 300;
	static constexpr int STICK_RIGHT_BORDER = 700;

	// 音量
	static constexpr int CLICK_VOLUME = 100;

	// 描画座標
	static constexpr int GAMEOVER_X = 225;
	static constexpr int GAMEOVER_Y = 200;

	static constexpr int PLAY_AGAIN_X = 180;
	static constexpr int PLAY_AGAIN_Y = 350;

	static constexpr int RETURN_TITLE_X = 450;
	static constexpr int RETURN_TITLE_Y = 350;

	static constexpr int GUIDE_X = 180;
	static constexpr int GUIDE_Y = 500;
};

