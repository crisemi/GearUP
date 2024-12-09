#include "MenuScene.h" 
#include "Race1v1Scene2.h"
#include "Race1v1Scene3.h"
#include "Race1v1Scene4.h"
#include "Race1v1Scene5.h"
#include "Race1v1Scene.h" 
#include "ui/CocosGUI.h" 

USING_NS_CC;

Scene* MenuScene::createScene() {
    return MenuScene::create();
}

bool MenuScene::init() {
    if (!Scene::init()) {
        return false;
    }
    bgMusicId = AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/video-game-boss-fiight-259885.mp3", true, 0.5f);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto fonButton = Sprite::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/R.png");
    fonButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(fonButton);

    auto titleLabel = Label::createWithSystemFont("Selecciona la pista", "Arial", 90);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));
    this->addChild(titleLabel);
    titleLabel->setColor(Color3B::BLUE);



    auto playButton = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/nivel1.png");
    playButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 640, origin.y + visibleSize.height / 2));
    playButton->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = Race1v1Scene2::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton);


    auto exitButton = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/nivel2.png");
    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 320, origin.y + visibleSize.height / 2));
    exitButton->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = Race1v1Scene3::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(exitButton);

    auto playButton2 = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/nivel3.png");
    playButton2->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2));
    playButton2->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = Race1v1Scene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton2);


    auto exitButton2 = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/nivel4.png");
    exitButton2->setPosition(Vec2(origin.x + visibleSize.width / 2 + 320, origin.y + visibleSize.height / 2));
    exitButton2->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = Race1v1Scene4::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(exitButton2);

    auto playButton3 = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/nivel5.png");
    playButton3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 640, origin.y + visibleSize.height / 2));
    playButton3->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = Race1v1Scene5::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton3);



    return true;
}