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

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "arduino_secrets.h"

// OLEDサイズ
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED生成
Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// 関数プロトタイプ宣言
void DrawOLED();

// Wi-Fi設定
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// PCのIPアドレス
IPAddress pcIP(
    PC_IP_1,
    PC_IP_2,
    PC_IP_3,
    PC_IP_4
);

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

// デッドゾーン
const int DEAD_ZONE = 10;

// OLED表示用データ
// ハイスコア
int hiScore = 0;

// 現在スコア
int score = 0;

// 残機
int life = 3;

// ブザー設定
const int BUZZER = 9;

// PC側から送られてくるサウンド命令(0:何もしない、1：ゲームクリア、2：ゲームオーバー)
int soundCommand = 0;

// 押した瞬間だけ音を鳴らすために使用
int oldGameButton = HIGH;


// 初期化
void setup()
{
    Serial.begin(115200);

    // OLED初期化
    display.begin(SSD1306_SWITCHCAPVCC,0x3C);

    // OLEDクリア
    display.clearDisplay();
    display.display();

    // ブザー初期化
    pinMode(BUZZER,OUTPUT);

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

    // 今回の状態を保存
    oldGameButton = button;

    // 前回との差分確認
    bool changed =
        abs(x - oldX) > DEAD_ZONE ||
        abs(y - oldY) > DEAD_ZONE ||
        button != oldButton;

    // 変化があった時だけ送信
    if (changed)
    {
        oldX = x;
        oldY = y;
        oldButton = button;

        char sendData[16];

        sprintf(sendData,"%03X%03X%1X", x, y,button);

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
    }

    // =====================
    // PCからゲーム情報受信
    // =====================

    int packetSize = udp.parsePacket();

    if (packetSize > 0)
    {
        char recvBuffer[64];

        int len = udp.read(recvBuffer, sizeof(recvBuffer) - 1);

        recvBuffer[len] = '\0';

        // デバッグ
        Serial.print("PARSE -> ");
        Serial.println(recvBuffer);

        // ハイスコア,スコア,残機
        sscanf(recvBuffer,"%d,%d,%d,%d", &hiScore,&score,&life,&soundCommand);

        Serial.print("HI SCORE : ");
        Serial.println(hiScore);

        Serial.print("SCORE : ");
        Serial.println(score);

        Serial.print("LIFE : ");
        Serial.println(life);

        Serial.println("----------------");
    }

    // PCから送られてきたサウンド命令処理
    switch (soundCommand)
    {
        // GAME CLEAR
    case 1:

        // ドレミファンファーレ

        tone(BUZZER, 523, 100);
        delay(120);

        tone(BUZZER, 659, 100);
        delay(120);

        tone(BUZZER, 784, 200);

        // 一度だけ鳴らす
        soundCommand = 0;

        break;

        // GAME OVER
    case 2:

        tone(BUZZER, 400, 300);
        delay(350);

        tone(BUZZER, 250, 500);

        // 一度だけ鳴らす
        soundCommand = 0;
        break;
    case 3:

        tone(BUZZER, 1000, 50);

        soundCommand = 0;

        break;
    default:
    break;
    }
    // OLED更新
    DrawOLED();

    delay(5);
}

// OLED表示更新
void DrawOLED()
{
    // 画面クリア
    display.clearDisplay();

    // 文字サイズ
    display.setTextSize(1);

    // 白文字
    display.setTextColor(SSD1306_WHITE);

    // ハイスコア
    display.setCursor(0, 0);
    display.print("HI SCORE");

    display.setCursor(0, 10);
    display.println(hiScore);

    // スコア
    display.setCursor(0, 25);
    display.print("SCORE");

    display.setCursor(0, 35);
    display.println(score);

    // 残機
    display.setCursor(0, 50);
    display.print("LIFE : ");
    display.println(life);

    // OLEDへ反映
    display.display();
}