#pragma once
#include <DxLib.h>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

class SoundManager
{

public:

	// クリエイトインスタンス(ここでSoundManagerを作る一回のみ)
	static void CreateInstance(void);

	// インスタンスを取得するメソッド(シングルトン)
	static SoundManager* GetInstance();

	// インスタンスを削除するメソッド(ゲームの終了前に必ず呼ぶ)
	static void DeleteInstance();

	// 効果音(SE)をロードする
	bool LoadSE(const std::string& key, const char* fileName, bool is3DSound = true);
	// BGMをロードする
	bool LoadBGM(const std::string& key, const char* fileName, bool is3DSound = true);

	// 効果音(SE)の再生する
	void PlaySE(const std::string& key);
	// BGMの再生
	void PlayBGM(const std::string& key);

	// 効果音(SE)の停止
	void StopSE(const std::string& key);
	// BGMの停止
	void StopBGM(const std::string& key);

	// SEが再生されているのか
	bool IsPlayingSE(const std::string& seName);

	// 更新
	// 効果音(SE)の音量の設定処理
	void SetVolumeSE(const std::string& key, int volume);

	// BGMの音量の設定
	void SetVolumeBGM(const std::string& key, int volume);

	// 解放処理
	void ReleaseSound(const std::string& key);

	// 強制的にSEを最初から再生
	void PlaySEForce(const std::string& key);

private:

	// コンストラクタとデストラクタをprivateに置くことにより
	// 他のところで使えないようにしている（外部参照を防ぐ）
	// コンストラクタ
	SoundManager();

	// デストラクタ
	~SoundManager();

	// コピーコンストラクタ
	// この処理は変数にSoundManagerを入れることができないようにする処理
	// これにより他のところで絶対使えないものになる
	SoundManager(const SoundManager& other);

	// シングルトンのインスタンスへのポインタ
	static SoundManager* instance_;

	// 効果音のハンドルを格納するマップ
	// キーとハンドルのペア
	std::unordered_map<std::string, int> seHandles;
	std::unordered_map<std::string, int> bgmHandles;

	// ハンドルID
	int bgmHandle;		// BGM
};

