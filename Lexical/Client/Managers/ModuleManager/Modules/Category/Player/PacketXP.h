#pragma once
#include "../../ModuleBase/Module.h"

class PacketXP : public Module {
private:
	int delay = 1;
	int usePerTick = 1;
	int tickDelay = 0;
	int swapPenalty = 0;
	bool rotatekdekfei = false;
	bool silentswapping = false;
	int TATADELAY = 0;
	int TATATA = 0;
	float targetRotationX = 80.0f;
	float currentRotationX = 0.0f;
	float lerpSpeed = 0.1f;
	std::chrono::time_point<std::chrono::steady_clock> lastSwapTime;
public:
	PacketXP();
	void onNormalTick(LocalPlayer* localPlayer) override;
	void onUpdateRotation(LocalPlayer* localPlayer) override;
};