#pragma once
#include "../../ModuleBase/Module.h"

class EntityFly : public Module {
private:
	float hSpeed = 1.f;
	float currentSpeed = 1.f;
	float vSpeed = 0.5f;
	float Glide = -0.02f;
	std::vector<Actor*> targetList;
public:
	EntityFly();
	void onNormalTick(LocalPlayer* localPlayer) override;
};