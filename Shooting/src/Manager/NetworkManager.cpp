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
	stickX_ = 512;
	stickY_ = 512;

	button_ = false;
}

// デストラクタ
NetworkManager::~NetworkManager(void)
{
	Release();
}

// 初期化
bool NetworkManager::Init(void)
{
	if (socket_ != INVALID_SOCKET)
	{
		return true;
	}

	button_ = false;

	WSADATA wsaData;

	// Winsock初期化
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}

	// UDPソケット生成
	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (socket_ == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(5000);
	addr.sin_addr.s_addr = INADDR_ANY;

	// ポート5,000待受
	if (bind(socket_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}

	// ノンブロッキング
	u_long mode = 1;

	ioctlsocket(socket_, FIONBIO, &mode);

	return true;

	printfDx("Network Init Success\n");
}

// 更新
void NetworkManager::Update(void)
{
	sockaddr_in from;

	int fromSize = sizeof(from);

	int size = recvfrom(socket_, buffer_, sizeof(buffer_) - 1, 0, (sockaddr*)&from, &fromSize);

	if (size <= 0)
	{
		return;
	}

	buffer_[size] = '\0';

	int x, y,btn;

	if (sscanf_s(buffer_, "%3X%3X%1X", &x, &y, &btn) == 3)
	{
		stickX_ = x;
		stickY_ = y;

		button_ = (btn == 1);
	}
	
	DrawFormatString(10,10,GetColor(255, 255, 255),"%s",buffer_
	);
}

// 解放
void NetworkManager::Release(void)
{
	if (socket_ != INVALID_SOCKET)
	{
		closesocket(socket_);

		socket_ = INVALID_SOCKET;
	}

	WSACleanup();

	printfDx("Network Release\n");
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
