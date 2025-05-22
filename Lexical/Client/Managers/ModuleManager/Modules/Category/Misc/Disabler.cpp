#include "Disabler.h"
#include <Windows.h> // for Windows time functions

Disabler::Disabler() : Module("Disabler", "Disable the anticheat", Category::MISC) {
    // addEnumSetting("Server", "NULL", { "Lifeboat" }, &Mode);
}

std::string Disabler::getModeText() {
    static char textStr[15];
    sprintf_s(textStr, 15, "Lifeboat");
    return std::string(textStr);
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
    if (Mode == 0 && (packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;

        // Modificación para que el movimiento de subida y bajada sea de 0.02 y -0.02 cada 3 ticks
        if (paip) {
            // Interpolación de 3 ticks, +0.02 y -0.02
            float perc = static_cast<float>(paip->TicksAlive % 3) / 3.0f;
            float targetY = (perc < 0.5f) ? 0.02f : -0.02f;  // +0.02 y -0.02 según el ciclo
            paip->position.y = Math::lerp(paip->position.y, paip->position.y + targetY, perc);
            paip->mMove.y = -(1.0f / 3.0f);  // Velocidad de caída ajustada a 3 ticks
            if (paip->TicksAlive % 3 == 0) {
                paip->mInputData |= InputData::StartJumping;
            }
            paip->mInputData |= InputData::Jumping;
        }
        if (mpp) {
            // Interpolación de 3 ticks, +0.02 y -0.02
            float perc = static_cast<float>(mpp->mTick % 3) / 3.0f;
            float targetY = (perc < 0.5f) ? 0.02f : -0.02f;  // +0.02 y -0.02 según el ciclo
            mpp->mPos.y = Math::lerp(mpp->mPos.y, mpp->mPos.y + targetY, perc);
            mpp->mOnGround = true;
        }
    }
}
