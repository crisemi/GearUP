// RaceScene.h


#ifndef __RACE1v1_SCENE4_H__
#define __RACE1v1_SCENE4_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"
#include <vector>



class Race1v1Scene4 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta) override;

    void showEndGameMenu();
    void checkCollisions();


    bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);


    CREATE_FUNC(Race1v1Scene4);

private:
    std::string selectedCarImagePath;
    void showCarSelectionMenu();
    void initializeRaceScene();
    int bgMusicId = -1;

    void stopBackgroundMusic() {
        if (bgMusicId != -1) {
            cocos2d::AudioEngine::stop(bgMusicId);
            bgMusicId = -1;
        }
    }
    cocos2d::Sprite* carSprite;
    cocos2d::Sprite* trackSprite;
    cocos2d::Label* lapLabel;
    cocos2d::Label* endLabel;

    float speed = 0.0f;
    float maxSpeed = 15.0f;
    const float acceleration = 0.1f;
    const float deceleration = 0.2f;
    const float friccion = 0.03f;
    float raceTime = 0.0f;
    int playerlaps = 0;
    int rivallaps1 = 0;
    int rivallaps2 = 0;
    int rivallaps3 = 0;
    bool gameFinished = false;
    bool opponentfin = false;
    const float finishLineY = 650.0f;
    float carAngle = 0.0f;
    float angle = 0.0f;
    bool hasCollision = false;
    float collisionTimer = 0.0f;

    cocos2d::Node* cameraNode;
    cocos2d::Follow* cameraFollow;
    void setupCamera();


    cocos2d::DrawNode* minimapBorder;
    cocos2d::DrawNode* minimapContent;
    std::vector<cocos2d::DrawNode*> minimapDots;
    void setupMinimap();
    void updateMinimap();
    cocos2d::Sprite* minimapTrack;
    float minimapScale;
    cocos2d::Rect trackBounds;
    cocos2d::Vec2 minimapOrigin;
    cocos2d::Vec2 minimapSize;

    cocos2d::Sprite* finishLineSprite;
    bool crossedFinishLine;

    std::vector<cocos2d::Sprite*> checkpoints;
    std::vector<bool> checkpointsReached;
    bool canCompleteLap;

    bool raceStarted = false;
    void startCountdown();
    void startRace();
    cocos2d::Sprite* trafficLight = nullptr;

    float clampValue(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    struct PowerUp {
        cocos2d::Sprite* sprite;
        std::string type;
        bool active;
        float duration;
        float timer;
    };
    std::vector<PowerUp> powerUps;
    bool isInvulnerable = false;
    float normalMaxSpeed;
    void setupPowerUps();
    void updatePowerUps(float delta);
    void activatePowerUp(const std::string& type);
    void spawnPowerUp();
    void checkPowerUpCollisions();
    cocos2d::Label* powerUpLabel;



    std::vector<cocos2d::Sprite*> borders;


    std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
};

#endif // __RACE1v1_SCENE4_H__


