#include "MainMenuScene.h"
#include "menu2.h"
#include "RaceScene.h" 
#include "Race1v1Scene.h" 
#include "ui/CocosGUI.h" 

USING_NS_CC;

Scene* MainMenuScene::createScene() {
    return MainMenuScene::create();
}

bool MainMenuScene::init() {
    if (!Scene::init()) {
        return false;
    }
    bgMusicId = AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/Formula 1 - Brian Tyler.mp3", true, 0.5f);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto fonButton = Sprite::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/fondo.png"); 
    fonButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    fonButton->setScale(1.74229f);
    this->addChild(fonButton);

    auto titleLabel = Label::createWithSystemFont("F1 GearUP", "Arial", 90);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600, origin.y + visibleSize.height / 2));
    this->addChild(titleLabel);

    auto playButton = ui::Button::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/playy.png"); 
    playButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    playButton->setOpacity(0);
    playButton->setScale(9.6);
    playButton->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = menu2::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton);

    auto titleLabel2 = Label::createWithSystemFont("click para", "Arial", 40);
    titleLabel2->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600, origin.y + visibleSize.height / 2 - 80));
    this->addChild(titleLabel2);
    auto titleLabel3 = Label::createWithSystemFont("jugar", "Arial", 40);
    titleLabel3->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600, origin.y + visibleSize.height / 2 - 110));
    this->addChild(titleLabel3);

    return true;
}