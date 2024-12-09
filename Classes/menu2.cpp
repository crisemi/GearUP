#include "menu2.h" 
#include "RaceScene.h" 
#include "info.h"
#include "RaceScene2.h"
#include "RaceScene3.h"
#include "RaceScene4.h"
#include "RaceScene5.h"
#include "MenuScene.h"
#include "Race1v1Scene.h" 
#include "ui/CocosGUI.h" 

USING_NS_CC;

Scene* menu2::createScene() {
    return menu2::create();
}

bool menu2::init() {
    if (!Scene::init()) {
        return false;
    }
    bgMusicId = AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/video-game-boss-fiight-259885.mp3", true, 0.5f);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto fonButton = Sprite::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/R.png"); 
    fonButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(fonButton);

    auto titleLabel = Label::createWithSystemFont("Modo de juego", "Arial", 90);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2 + 200));
    this->addChild(titleLabel);
    titleLabel->setColor(Color3B::BLUE);

    auto titleLabel2 = Label::createWithSystemFont("Contrareloj", "Arial", 50);
    titleLabel2->setPosition(Vec2(origin.x + visibleSize.width / 2 - 320, origin.y + visibleSize.height / 2 - 150));
    this->addChild(titleLabel2);
    titleLabel2->setColor(Color3B::BLACK);

    auto titleLabel3 = Label::createWithSystemFont("Temporada F1", "Arial", 50);
    titleLabel3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 330, origin.y + visibleSize.height / 2 - 150));
    this->addChild(titleLabel3);
    titleLabel3->setColor(Color3B::BLACK);


    auto playButton = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/reloj.png"); 
    playButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 320, origin.y + visibleSize.height / 2));
    playButton->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton);


    auto exitButton = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/normal.png"); 
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 320, origin.y + visibleSize.height / 2 ));
    exitButton->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = RaceScene2::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(exitButton);

    auto playButton2 = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/inf.png");
    playButton2->setPosition(Vec2(origin.x + 170, origin.y + 170));
    playButton2->setScale(0.07f);
    playButton2->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = info::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton2);

    return true;
}