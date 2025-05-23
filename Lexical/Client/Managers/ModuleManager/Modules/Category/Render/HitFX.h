#pragma once
#include "../../ModuleBase/Module.h"
#include <vector>
#include <unordered_map>
#include "../../../ModuleManager.h"
struct FXParticle {
    Vec3<float> pos;
    Vec3<float> velocity;
    float lifetime;
    float maxLifetime;
    UIColor color;

    FXParticle(const Vec3<float>& p, const Vec3<float>& v, float life, const UIColor& col)
        : pos(p), velocity(v), lifetime(life), maxLifetime(life), color(col) {
    }
};

class HitFX : public Module {
private:
    std::unordered_map<Actor*, std::vector<FXParticle>> particles;
    
    float particleLifetime = 1.0f;
    UIColor color = UIColor(255, 0, 0);
    int particleCount = 10;

public:
    HitFX();
    void onLevelRender() override;
    void onNormalTick(LocalPlayer* localPlayer) override;
};
