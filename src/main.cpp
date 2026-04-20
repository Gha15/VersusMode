#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <globed/api.hpp>
#include <random>

using namespace geode::prelude;

class VersusManager : public CCNode {
public:
    int m_playerPoints = 0;
    float m_voteTimer = 15.0f;
    bool m_isVoting = false;
    int m_myVoteID = 0;
    int m_opponentVoteID = 0;
    
    CCLabelBMFont* m_timerLabel = nullptr;
    CCSprite* m_thumbSprite = nullptr;

    // Singleton Pattern
    static VersusManager* get() {
        static auto instance = new VersusManager();
        return instance;
    }

    // Packet ID for Globed communication
    static inline const std::string VOTE_PACKET = "gha16.versusmode/vote";

    // --- THUMBNAIL LOGIC ---
    void updateThumbnail(int levelID, CCNode* parent) {
        if (m_thumbSprite) m_thumbSprite->removeFromParent();

        auto thumbName = fmt::format("{}.png", levelID);
        m_thumbSprite = CCSprite::create(thumbName.c_str());

        if (!m_thumbSprite) {
            m_thumbSprite = CCSprite::createWithSpriteFrameName("GJ_placeholder_001.png");
        }

        m_thumbSprite->setScale(0.5f);
        m_thumbSprite->setPosition({parent->getContentSize().width / 2, parent->getContentSize().height / 2 + 20});
        parent->addChild(m_thumbSprite);
    }

    // --- NETWORKING ---
    void sendVote(int levelID) {
        if (!globed::is_connected()) return;
        m_myVoteID = levelID;
        // Send the ID as a string through Globed
        globed::send_packet(VOTE_PACKET, std::to_string(levelID));
        Notification::create(fmt::format("Voted for ID: {}", levelID), NotificationIcon::Success)->show();
    }

    void onVoteReceived(std::string const& data) {
        try {
            m_opponentVoteID = std::stoi(data);
            Notification::create("Opponent has voted!")->show();
        } catch (...) {}
    }

    // --- MATCHMAKING UI ---
    void startMatchmaking(CCNode* parent) {
        if (!globed::is_connected() || globed::get_room_player_count() < 2) {
            FLAlertLayer::create("VersusMode", "Need <cy>at least 2 players</c> in Globed room!", "OK")->show();
            return;
        }

        m_isVoting = true;
        m_voteTimer = 15.0f;
        m_myVoteID = 0;
        m_opponentVoteID = 0;

        // Visual Timer
        m_timerLabel = CCLabelBMFont::create("15", "goldFont.fnt");
        m_timerLabel->setPosition({CCDirector::get()->getWinSize().width / 2, CCDirector::get()->getWinSize().height - 30});
        parent->addChild(m_timerLabel, 100);
        
        // Schedule the update
        CCScheduler::get()->scheduleSelector(schedule_selector(VersusManager::updateTimer), this, 1.0f, false);

        // Selection Popup
        auto popup = Geode::createQuickPopup("Versus Vote", "Select a level to duel!", "Cancel", "Confirm", nullptr);
        auto winSize = popup->m_mainLayer->getContentSize();

        auto menu = CCMenu::create();
        menu->setLayout(RowLayout::create()->setGap(5.f));
        menu->setPosition({winSize.width / 2, 40});

        auto glm = GameLevelManager::sharedState();

        // Helper for category buttons
        auto addVoteBtn = [&](const char* label, int id, ccColor3B color) {
            auto btnSpr = ButtonSprite::create(label, 30, true, "bigFont.fnt", "GJ_button_01.png", 25.f, 0.5f);
            btnSpr->setColor(color);
            auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(VersusManager::onCategoryClick));
            btn->setTag(id);
            menu->addChild(btn);
        };

        addVoteBtn("Daily", glm->m_dailyLevelID, {255, 255, 255});
        addVoteBtn("Weekly", glm->m_weeklyLevelID, {255, 100, 100});
        addVoteBtn("Tower", 5001, {100, 255, 100});
        addVoteBtn("Dash", 22, {100, 100, 255});

        menu->updateLayout();
        popup->m_mainLayer->addChild(menu);
        popup->show();
    }

    void onCategoryClick(CCObject* sender) {
        this->sendVote(sender->getTag());
    }

    void updateTimer(float dt) {
        m_voteTimer -= 1.0f;
        if (m_timerLabel) {
            m_timerLabel->setString(fmt::format("{}", (int)m_voteTimer).c_str());
        }
        if (m_voteTimer <= 0) {
            CCScheduler::get()->unscheduleSelector(schedule_selector(VersusManager::updateTimer), this);
            if (m_timerLabel) m_timerLabel->removeFromParent();
            finalizeMatch();
        }
    }

    // --- GAME START ---
    void finalizeMatch() {
        m_isVoting = false;
        int finalID = 0;

        // Use a shared seed for both players to pick the same level
        if (m_myVoteID != 0 && m_opponentVoteID != 0) {
            std::mt19937 gen(static_cast<unsigned int>(m_myVoteID + m_opponentVoteID));
            std::uniform_int_distribution<> dis(0, 1);
            finalID = (dis(gen) == 0) ? m_myVoteID : m_opponentVoteID;
        } 
        else {
            finalID = (m_myVoteID != 0) ? m_myVoteID : (m_opponentVoteID != 0 ? m_opponentVoteID : 1);
        }

        auto level = GameLevelManager::sharedState()->getMainLevel(finalID, false);
        if (level) {
            auto scene = PlayLayer::scene(level, false, false);
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, scene));
        }
    }
};

// --- MENU HOOK ---
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        if (auto menu = this->getChildByID("right-side-menu")) {
            // Find and hide the "official" greyed-out versus button
            if (auto oldBtn = menu->getChildByID("versus-button")) {
                oldBtn->setVisible(false);
                
                // Create our active version using the wiki sprite
                auto spr = CCSprite::createWithSpriteFrameName("gj_vsBtn_001.png");
                if (!spr) {
                    spr = ButtonSprite::create("VS", 40, true, "goldFont.fnt", "GJ_button_01.png", 30.f, 0.6f);
                } else {
                    spr->setScale(0.7f);
                }
                
                auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyMenuLayer::onVersusClick));
                btn->setID("active-versus-button");
                menu->addChild(btn);
                menu->updateLayout();
            }
        }
        return true;
    }

    void onVersusClick(CCObject*) { 
        VersusManager::get()->startMatchmaking(this); 
    }
};

// --- PROGRESS HOOK ---
class $modify(MyPlayLayer, PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();
        
        auto vm = VersusManager::get();
        if (m_level->m_stars > 0) {
            vm->m_playerPoints += (int)m_level->m_stars;
            Notification::create(fmt::format("Points: {}/20", vm->m_playerPoints))->show();
            
            if (vm->m_playerPoints >= 20) {
                FLAlertLayer::create("Victory!", "You reached 20 points first!", "OK")->show();
                vm->m_playerPoints = 0;
            }
        }
    }
};

// --- MOD ENTRY POINT ---
$on_mod_load {
    // Listen for incoming votes from the opponent via Globed
    globed::listen(VersusManager::VOTE_PACKET, [](std::string const& data) {
        VersusManager::get()->onVoteReceived(data);
    });
}