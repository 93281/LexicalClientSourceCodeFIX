#include "Disabler.h"
#include <Windows.h> 

Disabler::Disabler() : Module("Disabler", "Disable the anticheat", Category::MISC) {
    registerSetting(new EnumSetting("Server", "NULL", { "Lifeboat", "Guyser" }, &Mode, 1));
}

std::string Disabler::getModeText() {
    if (Mode == 0) {
        return "Lifeboat";
    }
    else if (Mode == 1) {
        return "Guyser";
    }
    return "Unknown";
}


static __int64 ms;
static DWORD lastMS = GetTickCount();
static __int64 timeMS = -1;
static DWORD getCurrentMs() {
    return GetTickCount();
}

static __int64 getElapsedTime() {
    return getCurrentMs() - ms;
}

static void resetTime() {
    lastMS = getCurrentMs();
    timeMS = getCurrentMs();
}

static bool hasTimedElapsed(__int64 time, bool reset) {
    if (getCurrentMs() - lastMS > time) {
        if (reset)
            resetTime();
        return true;
    }
    return false;
}

void Disabler::onSendPacket(Packet* packet) {

    /* lifeboat*/
    if (Mode == 0 && (packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        if (paip) {
            float perc = static_cast<float>(paip->TicksAlive % 3) / 3.0f;
            float targetY = (perc < 0.5f) ? 0.02f : -0.02f;
            paip->position.y = Math::lerp(paip->position.y, paip->position.y + targetY, perc);
            paip->mMove.y = -(1.0f / 3.0f);
            if (paip->TicksAlive % 3 == 0) {
                paip->mInputData |= InputData::StartJumping;
            }
            paip->mInputData |= InputData::Jumping;
        }
        if (mpp) {
            float perc = static_cast<float>(mpp->mTick % 3) / 3.0f;
            float targetY = (perc < 0.5f) ? 0.02f : -0.02f;
            mpp->mPos.y = Math::lerp(mpp->mPos.y, mpp->mPos.y + targetY, perc);
            mpp->mOnGround = true;
        }
    }

    /* guyser */
    if (Mode == 1 && (packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        if (paip) {
            if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
                hitResult->type = HitResultType::AIR;
            }
        }
        if (mpp) {
            if (HitResult* hitResult = Game::getLocalPlayer()->level->getHitResult()) {
                hitResult->type = HitResultType::AIR;
            }
        }
    }

}
