#include "PacketXP.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
#include <chrono>

PacketXP::PacketXP() : Module("PacketXP", "Throw exp bottles automatically.", Category::PLAYER) {
    registerSetting(new SliderSetting<int>("Delay", "NULL", &delay, 1, 0, 20));
    registerSetting(new SliderSetting<int>("Rotation Delay", "NULL", &TATADELAY, 1, 0, 20));
    registerSetting(new SliderSetting<int>("Multiplier", "How many time to use each tick", &usePerTick, 1, 1, 64));
    registerSetting(new SliderSetting<int>("Swap Penalty", "Milliseconds to wait before swapping again", &swapPenalty, 100, 0, 1000));
    registerSetting(new BoolSetting("Silent Swap", "Use silent swapping", &silentswapping, false));
    registerSetting(new BoolSetting("Rotate", "Rotate Downwards", &rotatekdekfei, false));
}

void PacketXP::onNormalTick(LocalPlayer* localPlayer) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSwapTime).count();
    if (tickDelay < delay) {
        tickDelay++;
        return;
    }
    ItemStack* carriedItem = localPlayer->getCarriedItem();
    if (silentswapping) {
        const int oldSlot = InventoryUtils::getSelectedSlot();
        if (InventoryUtils::getSelectedItemId() != 524 && elapsed >= swapPenalty) {
            InventoryUtils::SwitchTo(InventoryUtils::getItem(524));
            lastSwapTime = now;
        }
        if (InventoryUtils::getSelectedItemId() == 524) {
            for (int i = 0; i < usePerTick; i++) {
                if (carriedItem->isValid() && carriedItem->item.get()->isThrowable()) {
                    localPlayer->gameMode->baseUseItem(carriedItem);
                }
            }
            if (elapsed >= swapPenalty) {
                InventoryUtils::SwitchTo(oldSlot);
                lastSwapTime = now;
            }
        }
    }
    else {
        if (InventoryUtils::getSelectedItemId() != 524 && elapsed >= swapPenalty) {
            InventoryUtils::SwitchTo(InventoryUtils::getItem(524));
            lastSwapTime = now;
        }
        if (InventoryUtils::getSelectedItemId() == 524) {
            for (int i = 0; i < usePerTick; i++) {
                if (carriedItem->isValid() && carriedItem->item.get()->isThrowable()) {
                    localPlayer->gameMode->baseUseItem(carriedItem);
                }
            }
        }
    }
    tickDelay = 0;
}

void PacketXP::onUpdateRotation(LocalPlayer* localPlayer) {
    if (TATATA < TATADELAY) {
        TATATA++;
        return;
    }
    if (!rotatekdekfei) return;
    currentRotationX = currentRotationX + (targetRotationX - currentRotationX) * lerpSpeed;
    localPlayer->rotation->presentRot.x = currentRotationX;
    TATATA = 0;
}