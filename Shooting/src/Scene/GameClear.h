#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Manager/NetworkManager.h"

class GameClear
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

	GameClear(void);		// コンストラクタ
	~GameClear(void);		// デストラクタ

	void Init(void);		// 初期化
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放処理

private:

	// クラス
	NetworkManager& network = NetworkManager::GetInstance();

	// 現在選択中のメニュー
	int selectMenu_;

	// フォントハンドル
	int ClearFont_;

	// ボタン
	bool oldButton_;

	// シーン遷移待ちフラグ
	bool changeSceneWait_;

	// SE再生用のフラグ
	bool gameClearSEPlayerd_;

	// フォント
	static constexpr int CLEAR_FONT_SIZE = 48;
	static constexpr int CLEAR_FONT_THICK = 3;

	// ジョイスティック判定
	static constexpr int STICK_LEFT_BORDER = 300;
	static constexpr int STICK_RIGHT_BORDER = 700;

	// 音量
	static constexpr int CLEAR_SE_VOLUME = 255;

	// タイトル表示位置
	static constexpr int CLEAR_TEXT_X = 200;
	static constexpr int CLEAR_TEXT_Y = 200;

	// メニュー位置
	static constexpr int MENU_Y = 350;
	static constexpr int PLAY_AGAIN_X = 180;
	static constexpr int RETURN_TITLE_X = 450;

	// 操作説明
	static constexpr int GUIDE_X = 150;
	static constexpr int GUIDE_Y = 500;
};

