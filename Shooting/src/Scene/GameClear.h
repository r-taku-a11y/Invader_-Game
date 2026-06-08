#pragma once
#include <DxLib.h>
#include"../Application.h"
#include"../Common/Vector2.h"

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

	// コンストラクタ
	GameClear(void);

	// デストラクタ
	~GameClear(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放処理
	void Release(void);

private:

	// 現在選択中のメニュー
	int selectMenu_;

	// 画像関係
	int gameClearHandle_;

};

