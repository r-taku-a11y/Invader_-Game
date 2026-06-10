#pragma once

// Wi-Fi通信管理クラス Arduino Uno R4 WiFiから送信されるJOY,X,Y,BTYを受信される

#include<winsock.h>

#pragma comment(lib,"ws2_32.lib")

class NetworkManager
{
public:
	
	NetworkManager(void);		// コンストラクタ
	~NetworkManager(void);		// デストラクタ

	bool Init(void);			// 初期化
	void Update(void);			// 更新
	void Release(void);			// 解放

	int GetStickX(void)const;	// X軸取得
	int GetStickY(void)const;	// Y軸取得
	bool GetButton(void)const;	// ボタン取得

private:

	// ソケット
	SOCKET socket_;

	// X軸
	int stickX_;

	// Y軸
	int stickY_;

	// ボタン
	bool button_;

	// 受信バッファ
	char buffer_[256];

};

