#include <cstdio>
#include <DxLib.h>

#include "NetworkManager.h"

NetworkManager& NetworkManager::GetInstance()
{
	static NetworkManager instance;

	return instance;
}

// コンストラクタ
NetworkManager::NetworkManager(void)
{
	socket_ = INVALID_SOCKET;

	// 中央値
	stickX_ = STICK_CENTER;
	stickY_ = STICK_CENTER;

	button_ = false;

	// 前回送信の初期化
	prevHiScore_ = -1;
	prevScore_ = -1;
	prevLife_ = -1;
	prevSoundCommand_ = -1;

	// Arduino送信用アドレス初期化
	ZeroMemory(&arduinoAddr_,sizeof(arduinoAddr_));
}

// デストラクタ
NetworkManager::~NetworkManager(void)
{
	Release();
}

// 初期化
bool NetworkManager::Init(void)
{
	// 前回送信の初期化
	prevHiScore_ = -1;
	prevScore_ = -1;
	prevLife_ = -1;
	prevSoundCommand_ = -1;

	// 既に初期化済みか確認
	if (socket_ != INVALID_SOCKET)
	{
		return true;
	}

	// ボタン状態の初期化
	button_ = false;

	// Winsock初期化(windowsでソケット通信を使用するため)
	if (WSAStartup(MAKEWORD(WINSOCK_MAJOR, WINSOCK_MINOR), &wsaData) != 0)
	{
		return false;
	}

	// UDPソケット生成 (AF_INET :IPv4, SOCK_DGRAM : UDP通信)
	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// ソケットの生成の失敗
	if (socket_ == INVALID_SOCKET)
	{
		return false;
	}

	// IPv4を使用
	addr.sin_family = AF_INET;

	// ポート番号5000
	addr.sin_port = htons(PORT);

	// 全てのネットワークアダプタから受信可能
	addr.sin_addr.s_addr = INADDR_ANY;

	// ポート5,000待受開始
	if (bind(socket_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}

	// ノンブロッキング設定
	// recvfrom()で受信データがなくても処理が停止しないようにする
	u_long mode = 1;

	ioctlsocket(socket_, FIONBIO, &mode);

	// 初期化成功
	// printfDx("Network Init Success\n");

	return true;
}

// 更新
void NetworkManager::Update(void)
{

	// 送信元情報の格納
	sockaddr_in from;

	// 送信元アドレスサイズ
	int fromSize = sizeof(from);

	// UDPデータ受信(受信できなかった場合はsizeに0以下が返る)
	int size = recvfrom(socket_, buffer_, sizeof(buffer_) - 1, 0, (sockaddr*)&from, &fromSize);

	// 送信失敗
	if (size <= 0)
	{
		return;
	}

	// OLEDへスコア情報を返送するため保存する
	arduinoAddr_ = from;

	// 文字列終端を追加
	buffer_[size] = '\0';

	// 受信データ格納用
	int x, y,btn;

	// 受信データ解析(Arduino側からXXXYYYBの形式で送られてくる)
	// 例：X = 0x200 Y = 0x180 BTN = 1
	if (sscanf_s(buffer_, RECEIVE_FORMAT, &x, &y, &btn) == 3)
	{
		// ジョイスティックの値の保存
		stickX_ = x;
		stickY_ = y;

		// ボタンの状態の保存
		// 0:押されている
		// 1:押されていない
		button_ = (btn == 0);
	}
}

// 解放
void NetworkManager::Release(void)
{
	// ソケットが有効か確認
	if (socket_ != INVALID_SOCKET)
	{
		// ソケットを閉じ、通信を終了させる
		closesocket(socket_);

		// 無効化状態にする
		socket_ = INVALID_SOCKET;
	}

	// 通信関連リソースを開放
	WSACleanup();

	// デバック表示
	// printfDx("Network Release\n");
}

// X軸取得
int NetworkManager::GetStickX(void) const
{
	return stickX_;
}

// Y軸取得
int NetworkManager::GetStickY(void) const
{
	return stickY_;
}

// ボタン取得
bool NetworkManager::GetButton(void) const
{
	return button_;
}

// Arduinoへゲーム情報送信 (PLED表示用)
void NetworkManager::SendGameInfo(int hiScore, int score, int life, int soundCommand)
{
	// Arduinoのアドレスは未取得
	if (arduinoAddr_.sin_addr.s_addr == 0)return;
	 
	// 変更検知処理
	if (hiScore == prevHiScore_ && score == prevScore_ && life == prevLife_ && soundCommand == prevSoundCommand_)
	{
		return;
	}

	// 送信用バッファ
	char sendData[SEND_BUFFER_SIZE];

	// 文字列作成
	sprintf_s(sendData,"%d,%d,%d,%d",hiScore,score,life,soundCommand);

	// Arduinoへ送信
	sendto(socket_,sendData,static_cast<int>(strlen(sendData)),0,(sockaddr*)&arduinoAddr_,sizeof(arduinoAddr_));

	// 今回送信した値を保存(次回の変更判定時にしようする)
	prevHiScore_ = hiScore;
	prevScore_ = score;
	prevLife_ = life;
	prevSoundCommand_ = soundCommand;
}
