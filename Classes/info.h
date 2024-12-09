#ifndef __INFO_H__
#define __INFO_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class info : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(info);

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

#endif // __INFO_H__

#pragma once
