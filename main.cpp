#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Globed/Globed.hpp>

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto versusBtn = CCMenuItemSpriteExtra::create(
            CCSprite::create("versusBtn.png"),
            this,
            menu_selector(MenuLayer::onVersus)
        );
        versusBtn->setPosition({winSize.width / 2, winSize.height / 2 - 50});
        this->addChild(versusBtn);

        log::info("Versus button injected successfully");
        return true;
    }

    void onVersus(CCObject*) {
        log::info("Versus button clicked, starting matchmaking...");
        Globed::connectToRoom("defaultRoom");
    }
};

// Initialize vote and score arrays safely
std::vector<int> votes(10, 0);
std::vector<int> scores(10, 0);
