# Arduino

/*
    Arduino Uno R4 WiFi
    ジョイスティック送信

    A0 : X軸
    A1 : Y軸
    D2 : ボタン

    UDPでDxLib側へ送信
*/

#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"

// Wi-Fi設定
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// ゲームPCのIPアドレス
IPAddress pcIP(192, 168, 11, 4);

// 通信ポート
const int PORT = 5000;

// UDP
WiFiUDP udp;

// ジョイスティック
const int VRX = A0;
const int VRY = A1;
const int SW  = 2;

// 前回送信値
int oldX = -1;
int oldY = -1;
int oldButton = -1;

// 変化量閾値
const int DEAD_ZONE = 10;

// 初期化
void setup()
{
    Serial.begin(115200);

    pinMode(SW, INPUT_PULLUP);

    while (!Serial)
    {
    }

    Serial.println("START");

    // WiFiモジュール確認
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("WiFi MODULE NOT FOUND");

        while (true)
        {
        }
    }

    Serial.println("WiFi MODULE OK");

    // WiFi接続
    Serial.println("WiFi Connecting...");

    while (WiFi.begin(ssid, password) != WL_CONNECTED)
    {
        Serial.println("Connecting...");
        delay(1000);
    }

    Serial.println("WiFi Connected");

    Serial.print("Arduino IP : ");
    Serial.println(WiFi.localIP());

    // UDP開始
    udp.begin(PORT);

    Serial.println("UDP Ready");
}

// メインループ
void loop()
{
    // ジョイスティック取得
    int x = analogRead(VRX);
    int y = analogRead(VRY);

    // ボタン取得
    int button = digitalRead(SW);

    // 前回値との差分確認
    bool changed =
        abs(x - oldX) > DEAD_ZONE ||
        abs(y - oldY) > DEAD_ZONE ||
        button != oldButton;

    // 変化が少ない場合は送信しない
    if (!changed)
    {
        delay(5);
        return;
    }

    // 前回値更新
    oldX = x;
    oldY = y;
    oldButton = button;

    // 送信データ
    char sendData[16];

    // 16進数7バイト形式
    sprintf(
        sendData,
        "%03X%03X%1X",
        x,
        y,
        button
    );

    // UDP送信
    if (udp.beginPacket(pcIP, PORT))
    {
        udp.write(
            (const uint8_t*)sendData,
            strlen(sendData)
        );

        udp.endPacket();

        Serial.print("SEND : ");
        Serial.println(sendData);
    }

    delay(5);
}