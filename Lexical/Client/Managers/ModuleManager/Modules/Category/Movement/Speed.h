#pragma once

#include "../../ModuleBase/Module.h"

class Speed : public Module {
public:
    Speed()
        : Module("Speed", "Hops from the ground.", Category::MOVEMENT),
        speed_(4.0f) {
        registerSetting(new SliderSetting<float>(
            "Speed", "Forward speed", &speed_, 4.12f, 0.f, 5.f));
        registerSetting(new BoolSetting(
            "Jump", "Toggle jumping", &jump, false));
    }

    void onClientTick() override {
        if (!Game::getLocalPlayer()) return;

        if (Game::getLocalPlayer()->isOnGround()) {
            if (IsMoving()) {
                if (jump) {
                    Game::getLocalPlayer()->jumpFromGround();
                }
            }
        }
        else {
            if (IsMoving()) {
                Vec2<float> motion = Motionize(speed_);
                Game::getLocalPlayer()->stateVector->velocity.x = motion.x;
                Game::getLocalPlayer()->stateVector->velocity.z = motion.y;
            }
        }
    }

private:
    float speed_;
    bool jump = false;
    Vec2<float> Motionize(float speed) {
        float player_yaw = Game::getLocalPlayer()->rotation->prevRot.y;

        bool w = Game::isKeyDown('W');
        bool a = Game::isKeyDown('A');
        bool s = Game::isKeyDown('S');
        bool d = Game::isKeyDown('D');

        if (w) {
            if (!a && !d) {
                player_yaw += 90.0f;
            }
            else if (a) {
                player_yaw += 45.0f;
            }
            else if (d) {
                player_yaw += 135.0f;
            }
        }
        else if (s) {
            if (!a && !d) {
                player_yaw -= 90.0f;
            }
            else if (a) {
                player_yaw -= 45.0f;
            }
            else if (d) {
                player_yaw -= 135.0f;
            }
        }
        else {
            if (!a && d) {
                player_yaw += 180.0f;
            }
        }

        float calc_yaw = player_yaw * 0.01745329251f;
        return Vec2<float>(cos(calc_yaw) * speed / 10.0f,
            sin(calc_yaw) * speed / 10.0f);
    }

    bool IsMoving() {
        return Game::isKeyDown('W') || Game::isKeyDown('A') ||
            Game::isKeyDown('S') || Game::isKeyDown('D');
    }
};
