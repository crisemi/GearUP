#include "info.h" 
#include "menu2.h" 
#include "ui/CocosGUI.h" 

USING_NS_CC;

Scene* info::createScene() {
    return info::create();
}

bool info::init() {
    if (!Scene::init()) {
        return false;
    }
    bgMusicId = AudioEngine::play2d("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/video-game-boss-fiight-259885.mp3", true, 0.5f);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto fonButton = Sprite::create("C:/Users/crisl/OneDrive/Documentos/carreras/Resources/R.png");
    fonButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(fonButton);

    auto titleLabel = Label::createWithSystemFont("Como jugar", "Arial", 90);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));
    this->addChild(titleLabel);
    titleLabel->setColor(Color3B::BLUE);

    auto titleLabel2 = Label::createWithSystemFont("Flecha <-/a: Girar a la izquierda", "Arial", 30);
    titleLabel2->setPosition(Vec2(origin.x + visibleSize.width / 2 - 420, origin.y + visibleSize.height / 2 - 150));
    this->addChild(titleLabel2);
    titleLabel2->setColor(Color3B::BLACK);

    auto titleLabel3 = Label::createWithSystemFont("Flecha ->/d: Girar a la derecha", "Arial", 30);
    titleLabel3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 420, origin.y + visibleSize.height / 2 - 150));
    this->addChild(titleLabel3);
    titleLabel3->setColor(Color3B::BLACK);

    auto titleLabel4 = Label::createWithSystemFont("Flecha ^/w: acelerar", "Arial", 30);
    titleLabel4->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2));
    this->addChild(titleLabel4);
    titleLabel4->setColor(Color3B::BLACK);

    auto titleLabel5 = Label::createWithSystemFont("Flecha v/d: frenar", "Arial", 30);
    titleLabel5->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 150));
    this->addChild(titleLabel5);
    titleLabel5->setColor(Color3B::BLACK);

    auto titleLabel6 = Label::createWithSystemFont("Hay dos modos de juego, en el modo temporada F1 comienzas en el circuito mas facil y cada que ganas", "Arial", 30);
    titleLabel6->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 300));
    this->addChild(titleLabel6);
    titleLabel6->setColor(Color3B::BLACK);

    auto titleLabel8 = Label::createWithSystemFont("una carrera avanzas a una mas complicada hasta ganar el campeonato con 5 victorias, el modo ", "Arial", 30);
    titleLabel8->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 350));
    this->addChild(titleLabel8);
    titleLabel8->setColor(Color3B::BLACK);

    auto titleLabel7 = Label::createWithSystemFont("contrareloj te deja practicar y ver tus tiempos en cada una de las pistasa 3 vueltas ", "Arial", 30);
    titleLabel7->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 400));
    this->addChild(titleLabel7);
    titleLabel7->setColor(Color3B::BLACK);


    auto playButton = ui::Button::create("exit.png");
    playButton->setPosition(Vec2(origin.x + visibleSize.width - 170, origin.y + visibleSize.height - 170));
    playButton->addClickEventListener([=](Ref* sender) {
        stopBackgroundMusic();
        auto raceScene = menu2::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, raceScene));
        });
    this->addChild(playButton);


    return true;
}