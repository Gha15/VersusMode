#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

/* We use a custom namespace or class to handle our Versus logic 
   without cluttering the global scope.
*/
class VersusManager {
public:
    static bool isInMatch;
    static void startMatch() {
        isInMatch = true;
        log::info("Versus match started!");
    }
};

bool VersusManager::isInMatch = false;

// --- Hooks ---

// 1. Adding a Versus Button to the Main Menu
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("main-menu");
        
        // Create our Versus Button using the logo we added to resources
        auto spr = CCSprite::createWithSpriteFrameName("logo.png");
        if (!spr) {
            // Fallback if logo fails to load
            spr = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
            spr->setScale(0.4f);
        }

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(MyMenuLayer::onVersusMode)
        );
        btn->setID("versus-button");

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onVersusMode(CCObject*) {
        FLAlertLayer::create(
            "Versus Mode",
            "Waiting for <cl>Globed</c> connection...",
            "OK"
        )->show();
    }
};

// 2. Handling Level UI for Thumbnails and Versus Stats
class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        // Check if we should display Versus UI elements
        if (VersusManager::isInMatch) {
            auto label = CCLabelBMFont::create("Versus Match Active", "goldFont.fnt");
            label->setScale(0.5f);
            label->setPosition({100, 100});
            this->addChild(label);
        }

        return true;
    }
};

/* This ensures that the mod logic initializes correctly 
   within the Geode framework.
*/
$on_mod(Loaded) {
    log::info("Versus Mode mod loaded successfully!");
}
