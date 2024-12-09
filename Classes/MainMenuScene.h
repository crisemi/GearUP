#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class MainMenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(MainMenuScene);

private:
    int bgMusicId = -1;

    void stopBackgroundMusic() {
        if (bgMusicId != -1) {
            cocos2d::AudioEngine::stop(bgMusicId);
            bgMusicId = -1;
        }
    }
    cocos2d::Sprite* fondo;
    void botonCallback(Ref* sender);
};

#endif // __MAIN_MENU_SCENE_H__

