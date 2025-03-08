#pragma once
class RuntimeState;

class RuntimeStateManager {
public:
    RuntimeStateManager() {
        // Initialize game states
        menu_state_ = new MenuState(this);
        play_state_ = new PlayState(this);
        game_over_state_ = new GameOverState(this);
        // Set initial state to menu state
        current_state_ = menu_state_;
    }

    void SetState(RuntimeState* state) {
        current_state_ = state;
    }

    void Update() {
        current_state_->Update();
    }

private:
    RuntimeState* current_state_;
    RuntimeState* menu_state_;
    RuntimeState* play_state_;
    RuntimeState* game_over_state_;
};

class RuntimeState {
public:
    RuntimeState(RuntimeStateManager* manager) : manager_(manager) {}
    virtual void Update() = 0;
protected:
    RuntimeStateManager* manager_;
};

class MenuState : public RuntimeState {
public:
    MenuState(RuntimeStateManager* manager) : RuntimeState(manager) {}
    void Update() override {
        // Handle menu state logic
    }
};

class PlayState : public RuntimeState {
public:
    PlayState(RuntimeStateManager* manager) : RuntimeState(manager) {}
    void Update() override {
        // Handle gameplay state logic
    }
};

class RuntimeOverState : public RuntimeState {
public:
    RuntimeOverState(RuntimeStateManager* manager) : RuntimeState(manager) {}
    void Update() override {
        // Handle game over state logic
    }
};
