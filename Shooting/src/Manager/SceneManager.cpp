#include "SceneManager.h"

// コンストラクタ
SceneManager::SceneManager(void)
{
    sceneID_ = SCENE_ID::TITLE;
}

// デストラクタ
SceneManager::~SceneManager(void)
{
}

// インスタンスの取得
SceneManager& SceneManager::GetInstance()
{
    static SceneManager instance;

    return instance;
}

// シーン変更
void SceneManager::ChangeScene(SCENE_ID scene)
{
    sceneID_ = scene;
}

// 現在シーン取得
SceneManager::SCENE_ID SceneManager::GetScene() const
{
    return sceneID_;
}


