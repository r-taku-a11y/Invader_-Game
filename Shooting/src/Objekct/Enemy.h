#pragma once

class Enemy
{
public:

	// 敵の種類
	enum class TYPE
	{
		RED,									// 赤
		ORANGE,									// オレンジ
		WHITE,									// 白
		YELLOW									// 黄色
	};

	Enemy(void);								// コンストラクタ
	~Enemy(void);								// デストラクタ

	void Init(TYPE type,float x,float y);		// 初期化
	void Update(void);							// 更新
	void Draw(void);							// 描画
	void Release(void);							// 解放

	// 生存管理
	bool IsAlive(void)const;					// 生存しているのか取得
	void kill(void);							// 敵を撃破の状態にする

	// 座標の取得
	float GetPosX(void)const;					// X座標の取得
	float GetPosY(void)const;					// Y座標の取得

	// 座標変更
	void SetPosX(float x);						// X座標の設定
	void SetPosY(float y);						// Y座標の設定

	// サイズ取得
	int GetWidth(void)const;					// 横幅の取得
	int GetHeight(void)const;					// 高さの取得

	// タイプの取得
	TYPE GetType(void)const;						// 敵のタイプの取得

	// 画像のロード
	static void LoadResource(void);
	// 画像の解放
	static void ReleaseResource(void);

	// 列番号の設定
	void SetColumn(int column);

	// 列番号の取得
	int GetColumn() const;

private:

	// 画像ハンドル
	int modelHandle_;

	// 座標
	float PosX_;
	float PosY_;

	// 画像サイズ
	int width;				// 横幅
	int height;				// 高さ

	// 生存フラグ
	bool isAlive_;

	// 敵の種類
	TYPE type_;

	// 敵画像共有ハンドル
	static int redHandle_;
	static int orangeHandle_;
	static int whiteHandle_;
	static int yellowHandle_;

	// 敵の列番号
	int column_;

	// 撃破音の音量
	static constexpr int KILL_SE_VOLUME = 150;
};

