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

	bool oldButton_;

};

