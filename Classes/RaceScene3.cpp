// RaceScene.cpp
#include "RaceScene3.h"
#include "RaceScene.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"
#include "menu2.h"

USING_NS_CC;

Scene* RaceScene3::createScene()
{
    return RaceScene3::create();
}

void RaceScene3::initializeOpponents() {

    std::vector<std::string> opponentSprites = {
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/al.png",
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/ha.png",
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/will.png"
    };


    std::vector<Vec2> trackWaypoints = {

        Vec2(3955, 80),
        Vec2(750, 80),
        Vec2(650, 780),
        Vec2(210, 780),
        Vec2(210, 1180),
        Vec2(-710, 1180),
        Vec2(-710, 1580),
        Vec2(-1710, 1580),
        Vec2(-1710, -1000),
        Vec2(3955, -1000),
        Vec2(3955, -300),
    };


    for (int i = 0; i < 3; i++) {

        auto opponent = Sprite::create(opponentSprites[i]);


        if (i == 0) {
            opponent->setPosition(Vec2(3860, -740));
        }
        if (i == 1) {
            opponent->setPosition(Vec2(4115, -1000));
        }
        if (i == 2) {
            opponent->setPosition(Vec2(3860, -1060)); 
        }


        this->addChild(opponent);
        opponents.push_back(opponent);

        std::vector<Vec2> uniqueWaypoints = trackWaypoints;
        for (auto& waypoint : uniqueWaypoints) {

            waypoint.x += (rand() % 131 - 80) * (i + 1) * 0.5f;
            waypoint.y += (rand() % 131 - 80) * (i + 1) * 0.5f;
        }


        OpponentData data;
        data.speed = 0;
        data.angle = 0.0f;
        data.targetAngle = 0.0f;
        data.currentWaypoint = 0;
        data.currentLap = 0;
        data.waypoints = uniqueWaypoints;

        opponentData.push_back(data);
    }
}

void RaceScene3::updateOpponents(float delta) {
    float maxOpponentSpeed = 14.0f;
    float opponentAcceleration = 0.1f;
    float waypointRadius = 200.0f;
    float rotationSpeed = 320.0f * delta;
    float collisionRadius = 100.0f;


    for (int i = 0; i < opponents.size(); i++) {
        auto& data = opponentData[i];
        auto opponent = opponents[i];


        bool hasCollision = false;
        Vec2 avoidanceDirection;
        Vec2 currentPos = opponent->getPosition();


        for (int j = 0; j < opponents.size(); j++) {
            if (i != j) {
                Vec2 otherPos = opponents[j]->getPosition();
                float distance = currentPos.distance(otherPos);

                if (distance < collisionRadius) {
                    hasCollision = true;

                    Vec2 awayFromOther = currentPos - otherPos;
                    awayFromOther.normalize();
                    avoidanceDirection += awayFromOther;
                }
            }
        }

        if (hasCollision) {

            if (avoidanceDirection.length() > 0) {
                avoidanceDirection.normalize();
            }


            float avoidanceAngle = CC_RADIANS_TO_DEGREES(atan2(avoidanceDirection.y, avoidanceDirection.x)) - 90.0f;
            float angleDiff = avoidanceAngle - data.angle;

            while (angleDiff > 180.0f) angleDiff -= 360.0f;
            while (angleDiff < -180.0f) angleDiff += 360.0f;

            if (abs(angleDiff) > rotationSpeed) {
                if (angleDiff > 0) {
                    data.angle += rotationSpeed;
                }
                else {
                    data.angle -= rotationSpeed;
                }
            }
        }
        else {

            Vec2 targetWaypoint = getNextWaypoint(i);
            Vec2 direction = targetWaypoint - currentPos;
            float distanceToWaypoint = direction.length();

            if (distanceToWaypoint > 0) {
                direction.normalize();
            }

            float targetAngle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) - 90.0f;
            float angleDiff = targetAngle - data.angle;

            while (angleDiff > 180.0f) angleDiff -= 360.0f;
            while (angleDiff < -180.0f) angleDiff += 360.0f;

            if (abs(angleDiff) > rotationSpeed) {
                if (angleDiff > 0) {
                    data.angle += rotationSpeed;
                }
                else {
                    data.angle -= rotationSpeed;
                }
            }
            else {
                data.angle = targetAngle;
            }

            float angleToWaypoint = abs(angleDiff);
            float speedMultiplier = 1.0f - (angleToWaypoint / 180.0f) * 0.5f;

            data.speed += opponentAcceleration * speedMultiplier;
            if (data.speed > maxOpponentSpeed * speedMultiplier) {
                data.speed = maxOpponentSpeed * speedMultiplier;
            }
            for (int i = 0; i < opponents.size(); i++) {
                for (size_t i = 0; i < borders.size(); i++) {
                    auto box = opponent->getBoundingBox();
                    auto borderBox = borders[i]->getBoundingBox();
                    if (box.intersectsRect(borderBox)) {
                        data.speed = 0;
                        float bounceDistance = 20.0f;
                        Vec2 bounceDirection = Vec2(-sin(data.angle), -cos(data.angle)) * bounceDistance;
                        opponent->setPosition(opponent->getPosition() + bounceDirection);
                    }
                }
            }


            if (distanceToWaypoint < waypointRadius) {
                data.currentWaypoint = (data.currentWaypoint + 1) % data.waypoints.size();
                if (data.currentWaypoint == 0) {
                    data.currentLap++;
                    if (data.currentLap >= 3) {
                        opponentfin = true;
                        showEndGameMenu(false);
                        return;
                    }
                }
            }
        }


        float moveAngle = CC_DEGREES_TO_RADIANS(data.angle + 90.0f);
        Vec2 movement(cos(moveAngle) * data.speed, sin(moveAngle) * data.speed);
        Vec2 newPos = currentPos + movement;


        opponent->setPosition(newPos);
        opponent->setRotation(-data.angle);
    }
}

Vec2 RaceScene3::getNextWaypoint(int opponentIndex) {
    return opponentData[opponentIndex].waypoints[opponentData[opponentIndex].currentWaypoint];
}

float RaceScene3::calculateAngleToWaypoint(const Vec2& currentPos, const Vec2& targetPos) {
    Vec2 direction = targetPos - currentPos;
    float angleRadians = atan2(direction.y, direction.x);
    return CC_RADIANS_TO_DEGREES(angleRadians);
}






void RaceScene3::setupCamera() {
    cameraNode = Node::create();
    this->addChild(cameraNode);

    auto gameLayer = Node::create();
    gameLayer->setName("gameLayer");
    cameraNode->addChild(gameLayer);

    trackSprite->retain();
    trackSprite->removeFromParent();
    gameLayer->addChild(trackSprite, -1);

    for (auto points : tracks) {
        points->retain();
        points->removeFromParent();
        gameLayer->addChild(points);
    }

    for (auto border : borders) {
        border->retain();
        border->removeFromParent();
        gameLayer->addChild(border);
    }

    for (auto checkpoint : checkpoints) {
        checkpoint->retain();
        checkpoint->removeFromParent();
        gameLayer->addChild(checkpoint, -1);
    }

    finishLineSprite->retain();
    finishLineSprite->removeFromParent();
    gameLayer->addChild(finishLineSprite, -1);

    carSprite->retain();
    carSprite->removeFromParent();
    gameLayer->addChild(carSprite);

    for (auto opponent : opponents) {
        opponent->retain();
        opponent->removeFromParent();
        gameLayer->addChild(opponent);
    }


    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto camera = Follow::create(carSprite, Rect::ZERO);
    cameraNode->runAction(camera);


    lapLabel->setPosition(Vec2(100, visibleSize.height - 50));

    lapLabel->retain();
    lapLabel->removeFromParent();
    this->addChild(lapLabel);
}

void RaceScene3::setupPowerUps() {
    normalMaxSpeed = maxSpeed;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    powerUpLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
    powerUpLabel->setPosition(Vec2(visibleSize.width - 150, visibleSize.height - 150));
    this->addChild(powerUpLabel);


    this->schedule([this](float dt) {
        spawnPowerUp();
        }, 10.0f, "spawn_powerups");
}

void RaceScene3::spawnPowerUp() {
    std::vector<Vec2> spawnPoints = {
        Vec2(-1710, 500),
        Vec2(1140,150),
        Vec2(1100, -900)
    };

    int randomIndex = rand() % spawnPoints.size();
    Vec2 position = spawnPoints[randomIndex];

    std::string type = (rand() % 2 == 0) ? "speed" : "invulnerability";

    std::string imageName = (type == "speed") ? "boost.png" : "shield.png";
    auto sprite = Sprite::create(imageName);
    sprite->setPosition(position);
    sprite->setScale(1.5f);

    auto moveUp = MoveBy::create(1.0f, Vec2(0, 10));
    auto moveDown = moveUp->reverse();
    auto sequence = Sequence::create(moveUp, moveDown, nullptr);
    sprite->runAction(RepeatForever::create(sequence));

    auto gameLayer = cameraNode->getChildByName("gameLayer");
    gameLayer->addChild(sprite);

    PowerUp powerUp;
    powerUp.sprite = sprite;
    powerUp.type = type;
    powerUp.active = false;
    powerUp.duration = 5.0f;
    powerUp.timer = 0;

    powerUps.push_back(powerUp);
}

void RaceScene3::checkPowerUpCollisions() {
    auto playerBox = carSprite->getBoundingBox();

    for (auto& powerUp : powerUps) {
        if (!powerUp.active && powerUp.sprite != nullptr) {
            if (playerBox.intersectsRect(powerUp.sprite->getBoundingBox())) {

                activatePowerUp(powerUp.type);

                AudioEngine::play2d("powerup.mp3");


                powerUp.sprite->removeFromParent();
                powerUp.sprite = nullptr;
                powerUp.active = true;
                powerUp.timer = powerUp.duration;
            }
        }
    }
}

void RaceScene3::activatePowerUp(const std::string& type) {
    if (type == "speed") {
        maxSpeed = normalMaxSpeed * 1.5f;
        powerUpLabel->setString("SPEED BOOST!");
        powerUpLabel->setColor(Color3B::YELLOW);


        this->scheduleOnce([this](float dt) {
            maxSpeed = normalMaxSpeed;
            powerUpLabel->setString("");
            }, 5.0f, "remove_speed_boost");

    }
    else if (type == "invulnerability") {
        isInvulnerable = true;
        powerUpLabel->setString("INVULNERABLE!");
        powerUpLabel->setColor(Color3B::GREEN);


        this->scheduleOnce([this](float dt) {
            isInvulnerable = false;
            powerUpLabel->setString("");
            }, 5.0f, "remove_invulnerability");
    }
}

void RaceScene3::updatePowerUps(float delta) {
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        if (it->active) {
            it->timer -= delta;
            if (it->timer <= 0) {
                it = powerUps.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}


void RaceScene3::setupMinimap() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    minimapSize = Vec2(150, 100);
    minimapOrigin = Vec2(visibleSize.width - minimapSize.x - 20,
        visibleSize.height - minimapSize.y - 20);


    trackBounds = trackSprite->getBoundingBox();


    float scaleX = minimapSize.x / trackBounds.size.width;
    float scaleY = minimapSize.y / trackBounds.size.height;
    minimapScale = std::min(scaleX, scaleY);

    minimapBorder = DrawNode::create();
    Vec2 vertices[] = {
        minimapOrigin,
        Vec2(minimapOrigin.x + minimapSize.x, minimapOrigin.y),
        Vec2(minimapOrigin.x + minimapSize.x, minimapOrigin.y + minimapSize.y),
        Vec2(minimapOrigin.x, minimapOrigin.y + minimapSize.y)
    };
    minimapBorder->drawPolygon(vertices, 4, Color4F(0, 0, 0, 0.5), 1, Color4F::WHITE);
    this->addChild(minimapBorder, 10);

    minimapTrack = Sprite::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/race3.png");
    minimapTrack->setScale(minimapScale);
    minimapTrack->setPosition(minimapOrigin + minimapSize / 2);
    minimapTrack->setOpacity(128);
    this->addChild(minimapTrack, 11);

    for (int i = 0; i < opponents.size() + 1; i++) {
        auto dot = DrawNode::create();
        minimapDots.push_back(dot);
        this->addChild(dot, 12);
    }
}

void RaceScene3::updateMinimap() {
    auto worldToMinimap = [this](const Vec2& worldPos) -> Vec2 {

        Vec2 relativePos = worldPos - trackBounds.origin;


        Vec2 scaledPos = relativePos * minimapScale;


        float offsetY = 3.0f;
        return minimapOrigin + scaledPos + Vec2(0, offsetY);
        };


    Vec2 playerMinimapPos = worldToMinimap(carSprite->getPosition());
    minimapDots[0]->clear();
    minimapDots[0]->drawDot(playerMinimapPos, 3, Color4F::YELLOW);

    for (int i = 0; i < opponents.size(); i++) {
        Vec2 opponentMinimapPos = worldToMinimap(opponents[i]->getPosition());
        minimapDots[i + 1]->clear();
        if (i + 1 == 1) {
            minimapDots[i + 1]->drawDot(opponentMinimapPos, 3, Color4F::WHITE);
        }
        if (i + 1 == 2) {
            minimapDots[i + 1]->drawDot(opponentMinimapPos, 3, Color4F::BLACK);
        }
        if (i + 1 == 3) {
            minimapDots[i + 1]->drawDot(opponentMinimapPos, 3, Color4F::BLUE);
        }

    }

    auto clampToMinimap = [this](const Vec2& pos) -> Vec2 {
        return Vec2(
            clampValue(pos.x, minimapOrigin.x, minimapOrigin.x + minimapSize.x),
            clampValue(pos.y, minimapOrigin.y, minimapOrigin.y + minimapSize.y)
        );
        };
}

void RaceScene3::startCountdown()
{


    trafficLight = Sprite::create("tod.png");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    trafficLight->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.7));
    trafficLight->setScale(0.5f);
    this->addChild(trafficLight, 100);


    auto countLabel = Label::createWithTTF("3", "fonts/Marker Felt.ttf", 100);
    countLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.3));
    this->addChild(countLabel, 100);


    auto delay = DelayTime::create(1.0f);
    auto seq = Sequence::create(

        CallFunc::create([=]() {
            countLabel->setString("3");
            trafficLight->setTexture("red.PNG");
            AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/beep.mp3");
            }),
        delay,

        CallFunc::create([=]() {
            countLabel->setString("2");
            trafficLight->setTexture("yellow.PNG");
            AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/beep.mp3");
            }),
        delay,

        CallFunc::create([=]() {
            countLabel->setString("1");
            trafficLight->setTexture("yellow.PNG");
            AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/beep.mp3");
            }),
        delay,

        CallFunc::create([=]() {
            countLabel->setString("GO!");
            trafficLight->setTexture("green.PNG");
            AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/start.mp3");
            this->startRace();
            }),
        delay,

        CallFunc::create([=]() {
            countLabel->removeFromParent();
            trafficLight->removeFromParent();
            }),
        nullptr
    );

    this->runAction(seq);
}

void RaceScene3::startRace()
{
    raceStarted = true;
    raceTime = 0;
}

void RaceScene3::showCarSelectionMenu()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto backgroundLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(backgroundLayer, 100);

    auto titleLabel = Label::createWithTTF("Selecciona tu auto", "fonts/Marker Felt.ttf", 48);
    titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.8));
    backgroundLayer->addChild(titleLabel);

    std::vector<std::string> carImages = {
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/am.png",
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/mc.png",
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/fe.png",
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/me.png",
        "C:/Users/crisl/OneDrive/Documentos/carreras/Resources/rb.png"
    };


    for (int i = 0; i < carImages.size(); ++i) {
        auto carButton = ui::Button::create(carImages[i]);


        float xPos = visibleSize.width / (carImages.size() + 1) * (i + 1);
        carButton->setPosition(Vec2(xPos, visibleSize.height / 2));
        carButton->setScale(2.0f);
        carButton->addClickEventListener([this, carImages, backgroundLayer, i](Ref* sender) {
            selectedCarImagePath = carImages[i];
            backgroundLayer->removeFromParent();
            initializeRaceScene();
            });

        backgroundLayer->addChild(carButton);
    }
}

void RaceScene3::initializeRaceScene()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    bgMusicId = AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/high-energy-gabber-metal-chiptune-fusion-263703.mp3", true, 0.5f);

    trackSprite = Sprite::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/race3.png");
    trackSprite->setPosition(visibleSize / 2);
    this->addChild(trackSprite, -1);

    carSprite = Sprite::create(selectedCarImagePath);
    carSprite->setPosition(Vec2(4115, -640));
    this->addChild(carSprite);

    initializeOpponents();

    Vec2 borderPositions[] = {

        Vec2(4035,325),
        Vec2(3635,325),
        Vec2(3235,325),
        Vec2(2835,325),
        Vec2(2435,325),
        Vec2(2035,325),
        Vec2(1635,325),
        Vec2(1235,325),
        Vec2(1135,325),
        Vec2(3470,-180),
        Vec2(3070,-180),
        Vec2(2670,-180),
        Vec2(2270,-180),
        Vec2(1870,-180),
        Vec2(1470,-180),
        Vec2(1070,-180),
        Vec2(670,-180),
        Vec2(685,1050),
        Vec2(150,540),
        Vec2(-280,915),
        Vec2(-680,915),
        Vec2(-780,915),
        Vec2(200,1427),
        Vec2(-200,1427),
        Vec2(-300,1427),
        Vec2(-700,1827),
        Vec2(-1100,1827),
        Vec2(-1500,1827),
        Vec2(-1900,1827),
        Vec2(-1100,1300),
        Vec2(200,-1227),
        Vec2(-200,-1227),
        Vec2(-600,-1227),
        Vec2(-1000,-1227),
        Vec2(-1400,-1227),
        Vec2(-1800,-1227),
        Vec2(500,-1227),
        Vec2(790,-1227),
        Vec2(1000,-1227),
        Vec2(1400,-1227),
        Vec2(1800,-1227),
        Vec2(2200,-1227),
        Vec2(2600,-1227),
        Vec2(3000,-1227),
        Vec2(3400,-1227),
        Vec2(3800,-1227),
        Vec2(4200,-1227),
        Vec2(-255,-767),
        Vec2(-600,-767),
        Vec2(-1000,-767),
        Vec2(-1100,-767),
        Vec2(200,-767),
        Vec2(600,-767),
        Vec2(1000,-767),
        Vec2(1400,-767),
        Vec2(1800,-767),
        Vec2(2200,-767),
        Vec2(2600,-767),
        Vec2(3000,-767),
        Vec2(3377,-767),

    };

    Vec2 borderPositionss[] = {

        Vec2(4235,125),
        Vec2(4235,-325),
        Vec2(4235,-725),
        Vec2(4235,-1127),
        Vec2(3670,-557),
        Vec2(3670,-380),
        Vec2(850,900),
        Vec2(850,640),
        Vec2(445,40),
        Vec2(445,230),
        Vec2(-10,640),
        Vec2(410,1370),
        Vec2(-2000,1627),
        Vec2(-2000,1370),
        Vec2(-2000,1020),
        Vec2(-2000,770),
        Vec2(-2000,420),
        Vec2(-2000,170),
        Vec2(-2000,-220),
        Vec2(-2000,-570),
        Vec2(-2000,-920),
        Vec2(-2000,-1270),
        Vec2(-1400,1100),
        Vec2(-1400,1000),
        Vec2(-1400,600),
        Vec2(-1400,420),
        Vec2(-1400,170),
        Vec2(-1400,-220),
        Vec2(-1400,-500),
        Vec2(-10,-500),
        Vec2(-500,1627),
        Vec2(-900,1100),
    };

    Vec2 track[] = {
        Vec2(3955, 80),
        Vec2(750, 80),
        Vec2(650, 780),
        Vec2(210, 780),
        Vec2(210, 1180),
        Vec2(-710, 1180),
        Vec2(-710, 1580),
        Vec2(-1710, 1580),
        Vec2(-1710, -1000),
        Vec2(3955, -1000),
        Vec2(3955, -300),
    };

    for (const auto& pos : track) {
        auto points = Sprite::create("border.png");
        points->setPosition(pos);
        points->setScale(1.0f);
        points->setOpacity(0);
        this->addChild(points);
        tracks.push_back(points);
    }
    for (const auto& pos : borderPositions) {
        auto border = Sprite::create("checkpoints.png");
        border->setPosition(pos);
        border->setScale(4.2f);
        border->setOpacity(0);
        this->addChild(border);
        borders.push_back(border);
    }
    for (const auto& pos : borderPositionss) {
        auto border = Sprite::create("checkpoint.png");
        border->setPosition(pos);
        border->setScale(4.2f);
        border->setOpacity(0);
        this->addChild(border);
        borders.push_back(border);
    }



    Vec2 checkpointPositions[] = {
        Vec2(1500, 60),
        Vec2(-200, -1000)
    };


    for (const auto& pos : checkpointPositions) {
        auto checkpoint = Sprite::create("checkpoint.png");
        checkpoint->setPosition(pos);
        checkpoint->setScale(5.2f);
        checkpoint->setOpacity(0);
        this->addChild(checkpoint, -1);
        checkpoints.push_back(checkpoint);
        checkpointsReached.push_back(false);
    }



    finishLineSprite = Sprite::create("meta.png");
    finishLineSprite->setPosition(Vec2(3955, -300));
    finishLineSprite->setScale(5.3f);
    this->addChild(finishLineSprite, -1);
    crossedFinishLine = false;
    canCompleteLap = false;



    lapLabel = Label::createWithTTF("Lap: 0 \ntime: 0", "fonts/Marker Felt.ttf", 24);
    lapLabel->setPosition(Vec2(100, 300));
    this->addChild(lapLabel);


    setupCamera();
    setupMinimap();





    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        keyMap[keyCode] = true;
        };
    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        keyMap[keyCode] = false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    this->scheduleUpdate();

    playerlaps = 0;
    gameFinished = false;
    opponentfin = false;
    raceStarted = false;
    raceTime = 0;


    setupPowerUps();


    this->scheduleOnce([this](float dt) {
        startCountdown();
        }, 1.0f, "start_countdown");
}


bool RaceScene3::init()
{
    if (!Scene::init())
        return false;

    showCarSelectionMenu();

    return true;
}



bool RaceScene3::isKeyPressed(EventKeyboard::KeyCode keyCode) {
    return keyMap[keyCode];
}

void RaceScene3::update(float delta)
{
    if (!raceStarted) {
        return;
    }
    if (!hasCollision) {
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_W) || isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
            speed += acceleration;
            if (speed > maxSpeed) speed = maxSpeed;


            //AudioEngine::play2d("ruuuuun.mp3");
        }
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_S) || isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
            speed -= deceleration;
            if (speed < 0) speed = 0;
        }
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_A) || isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
            angle -= 0.03f;
            carAngle -= 1.72f;

        }
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_D) || isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
            angle += 0.03f;
            carAngle += 1.72f;

        }
        if (!isKeyPressed(EventKeyboard::KeyCode::KEY_S) && !isKeyPressed(EventKeyboard::KeyCode::KEY_W) && !isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) && !isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
            speed -= friccion;
            if (speed < 0) speed = 0;
        }


    }
    else {

        collisionTimer -= delta;
        if (collisionTimer <= 0.0f) {
            hasCollision = false;
        }
    }

    carSprite->setRotation(carAngle);
    Vec2 newPos = carSprite->getPosition() + Vec2(speed * sin(angle), speed * cos(angle));
    carSprite->setPosition(newPos);



    auto playe = carSprite->getPosition();
    auto playerBox = carSprite->getBoundingBox();
    auto finishBox = finishLineSprite->getBoundingBox();


    for (size_t i = 0; i < borders.size(); i++) {
        auto borderBox = borders[i]->getBoundingBox();
        if (borderBox.containsPoint(playe)) {
            speed = 0;
            float bounceDistance = 20.0f;
            Vec2 bounceDirection = Vec2(-sin(angle), -cos(angle)) * bounceDistance;
            //carSprite->setPosition(carSprite->getPosition() + bounceDirection);
        }
    }

    bool allCheckpointsReached = true;
    for (size_t i = 0; i < checkpoints.size(); i++) {
        auto checkpointBox = checkpoints[i]->getBoundingBox();
        if (playerBox.intersectsRect(checkpointBox)) {
            checkpointsReached[i] = true;
        }
        if (!checkpointsReached[i]) {
            allCheckpointsReached = false;
        }
    }


    canCompleteLap = allCheckpointsReached;

    if (playerBox.intersectsRect(finishBox)) {
        if (!crossedFinishLine && canCompleteLap) {
            playerlaps++;
            crossedFinishLine = true;

            for (size_t i = 0; i < checkpointsReached.size(); i++) {
                checkpointsReached[i] = false;
            }
            canCompleteLap = false;
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto lapCompleteLabel = Label::createWithTTF("Lap " + std::to_string(playerlaps) + "/3",
                "fonts/Marker Felt.ttf", 48);
            lapCompleteLabel->setPosition(visibleSize / 2);
            this->addChild(lapCompleteLabel);


            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            lapCompleteLabel->runAction(Sequence::create(fadeOut, remove, nullptr));
        }
    }
    else {
        crossedFinishLine = false;
    }
    if (playerlaps == 3 && !opponentfin) {
        showEndGameMenu(true);
        return;
    }






    raceTime += delta;
    lapLabel->setString("Lap: " + std::to_string(playerlaps) + "\ntime: " + std::to_string(raceTime));
    updateOpponents(delta);
    updateMinimap();

    checkPowerUpCollisions();
    updatePowerUps(delta);
    if (!isInvulnerable) {
        checkCollisions();
    }


}


void RaceScene3::showEndGameMenu(bool playerWon) {

    auto menuLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(menuLayer);
    stopBackgroundMusic();


    std::string message = playerWon ? "Ganaste! en " + std::to_string(raceTime) + "seg" : "Perdiste";
    auto label = Label::createWithSystemFont(message, "Arial", 36);
    label->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) + Vec2(0, 50));
    label->setColor(Color3B::WHITE);
    menuLayer->addChild(label);


    if (playerWon) {
        auto retryButton = ui::Button::create("play.png");
        retryButton->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) + Vec2(-220, -50));
        retryButton->addClickEventListener([=](Ref* sender) {

            auto newScene = RaceScene::createScene();
            Director::getInstance()->replaceScene(newScene);
            });
        menuLayer->addChild(retryButton);


        auto exitButton = ui::Button::create("exit.png");
        exitButton->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) + Vec2(220, -50));
        exitButton->addClickEventListener([=](Ref* sender) {
            auto newScene = menu2::createScene();
            Director::getInstance()->replaceScene(newScene);
            });
        menuLayer->addChild(exitButton);
    }
    else {
        auto retryButton = ui::Button::create("reset.png");
        retryButton->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) + Vec2(-220, -50));
        retryButton->addClickEventListener([=](Ref* sender) {

            auto newScene = RaceScene3::createScene();
            Director::getInstance()->replaceScene(newScene);
            });
        menuLayer->addChild(retryButton);


        auto exitButton = ui::Button::create("exit.png");
        exitButton->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2) + Vec2(220, -50));
        exitButton->addClickEventListener([=](Ref* sender) {
            auto newScene = menu2::createScene();
            Director::getInstance()->replaceScene(newScene);
            });
        menuLayer->addChild(exitButton);
    }
}


void RaceScene3::checkCollisions() {
    Vec2 playerCenter = carSprite->getPosition();
    float playerRadius = std::min(carSprite->getContentSize().width,
        carSprite->getContentSize().height) * carSprite->getScale() * 0.8f;

    for (int i = 0; i < opponents.size(); i++) {
        Vec2 opponentCenter = opponents[i]->getPosition();
        float opponentRadius = std::min(opponents[i]->getContentSize().width,
            opponents[i]->getContentSize().height) * opponents[i]->getScale() * 0.8f;

        float distance = playerCenter.distance(opponentCenter);
        float minDistance = playerRadius + opponentRadius;

        if (distance < minDistance) {
            hasCollision = true;
            speed = 0;
            opponentData[i].speed = 0;

            collisionTimer = 0.0f;

            float bounceDistance = 20.0f;
            Vec2 bounceDirection = Vec2(-sin(angle), -cos(angle)) * bounceDistance;

            carSprite->setPosition(carSprite->getPosition() + bounceDirection);

            break;
        }
    }
}