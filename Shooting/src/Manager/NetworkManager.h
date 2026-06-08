#pragma once

// Wi-Fi通信管理クラス Arduino Uno R4 WiFiから送信されるJOY,X,Y,BTYを受信される

#include<winsock.h>

#pragma comment(lib,"ws2_32.lib")

class NetworkManager
{
public:

	static NetworkManager& GetInstance();	// シングルトンインスタンスの取得
	
	NetworkManager(void);		// コンストラクタ
	~NetworkManager(void);		// デストラクタ

	bool Init(void);			// 初期化
	void Update(void);			// 更新
	void Release(void);			// 解放

	int GetStickX(void)const;	// X軸取得
	int GetStickY(void)const;	// Y軸取得
	bool GetButton(void)const;	// ボタン取得

	// Arduinoへゲーム情報送信
	void SendGameInfo(int hiScore, int score, int life, int soundCommand);

private:

	// コピー禁止
	NetworkManager(const NetworkManager&) = delete;

	// 代入禁止
	NetworkManager& operator=(const NetworkManager&) = delete;

	// Arduino送信用アドレス
	sockaddr_in arduinoAddr_;

	// ソケット
	SOCKET socket_;

	// アドレス情報の設定
	sockaddr_in addr;

	// Winsock情報格納用
	WSADATA wsaData;

	// X軸
	int stickX_;

	// Y軸
	int stickY_;

	// ボタン
	bool button_;

	// 受信バッファ
	char buffer_[256];

	// 前回送信したスコアを保持(変化検知用)
	int prevHiScore_;
	int prevScore_;
	int prevLife_;

	// 前回送信したサウンドコマンド
	int prevSoundCommand_;

	// ジョイスティックの中央値
	static constexpr int STICK_CENTER = 512;

	// 通信設定
	// UDP通信で使用するポート番号
	static constexpr int PORT = 5000;

	// バッファ設定
	// ゲーム情報送信用バッファサイズ
	static constexpr int SEND_BUFFER_SIZE = 64;

	// Winsockバージョン
	static constexpr int WINSOCK_MAJOR = 2;
	static constexpr int WINSOCK_MINOR = 2;

	// Arduinoから受信するデータ形式
	// XXX : X座標(16進3桁)
	// YYY : Y座標(16進3桁)
	// B   : ボタン状態(1桁)
	static constexpr const char* RECEIVE_FORMAT = "%3X%3X%1X";

};

