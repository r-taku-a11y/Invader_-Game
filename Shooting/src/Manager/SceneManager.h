#pragma once


class SceneManager
{
public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,			// タイトル
		GAMESENEN,		// ゲームシーン
		GAMEOVER,		// ゲームオーバー	
		GAMECLEAR		// ゲームクリア
	};

	// インスタンスの取得
	static SceneManager& GetInstance();

	// シーン変更
	void ChangeScene(SCENE_ID scene);

	// 現在シーン取得
	SCENE_ID GetScene() const;

private:

	// コンストラクタ
	SceneManager(void);
	// デストラクタ
	~SceneManager(void);

	/*
	 コピーコンストラクタ禁止
	 SceneManagerはシングルトンのため
	 コピーして複数生成されるのと防ぐ
	*/
	SceneManager(const SceneManager&) = delete;

	// コピーの禁止SceneManager同氏の代入を禁止
	SceneManager& operator=(const SceneManager&)=delete;

	// 現在のシーン
	SCENE_ID sceneID_;
};

