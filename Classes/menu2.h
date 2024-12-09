#ifndef __MENU2_H__
#define __MENU2_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class menu2 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(menu2); 

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

#endif // __MENU_H__

#pragma once