#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Globed/Globed.hpp>

using namespace geode::prelude;

// Global state
std::vector<int> votes;
std::vector<int> scores;

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
        if (!Globed::connectToRoom("defaultRoom")) {
            log::error("Failed to connect to Globed room!");
        } else {
            log::info("Connected to Globed room successfully");
            startVoting(2); // Example: 2 players
        }
    }
};

// Voting logic
void startVoting(int numPlayers) {
    votes.assign(numPlayers, 0);
    log::info("Voting started with {} players", numPlayers);
}

void recordVote(int playerId, int levelId) {
    if (playerId < votes.size()) {
        votes[playerId] = levelId;
        log::info("Player {} voted for level {}", playerId, levelId);
    } else {
        log::error("Invalid player ID {}", playerId);
    }
}

// Scoring logic
void resetScores(int numPlayers) {
    scores.assign(numPlayers, 0);
    log::info("Scores reset for {} players", numPlayers);
}

void addScore(int playerId) {
    if (playerId < scores.size()) {
        scores[playerId]++;
        log::info("Player {} score incremented to {}", playerId, scores[playerId]);
    }
}
