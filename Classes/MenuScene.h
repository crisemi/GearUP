#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class MenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(MenuScene);

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

#endif // __MENUSCENE_H__

#pragma once
