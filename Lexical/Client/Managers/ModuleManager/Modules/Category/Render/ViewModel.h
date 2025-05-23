#pragma once
#include "../../ModuleBase/Module.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
class ViewModel : public Module {
public:
    int swingType = 0; // 0 = Flux, 1 = Vanilla
    bool enableTranslation = true;
    bool enableScaling = true;
    bool enableRotation = true;
    bool resetToDefaults = false;
    bool shouldBlockForViewModel = false;
    Vec3<float> translationValues = Vec3<float>(0.f, 0.f, 0.f);
    Vec3<float> scalingValues = Vec3<float>(1.f, 1.f, 1.f);
    Vec3<float> rotationValues = Vec3<float>(0.1f, 0.f, 0.f);
    float rotationAngle = 0.f;
    int blockAnim = 0;
    void* fluxSwingAddr = (void*)(MemoryUtil::findSignature("E8 ? ? ? ? F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? 48 8B ? F3 0F ? ? 48 8B"));
    void* ogBytes[5] = { 0 };

    ViewModel() : Module("ViewModel", "Customize the appearance of your hand in the game.", Category::RENDER) {
        registerSetting(new EnumSetting("Swing Type", "Choose the swing animation.", { "Flux", "Vanilla" }, &swingType, 0));
        registerSetting(new EnumSetting("Block Type", "Choose the block animation.", { "Flux", "Test", "None" }, &blockAnim, 0));
        registerSetting(new BoolSetting("Enable Translation", "Enable or disable hand translation.", &enableTranslation, true));
        registerSetting(new BoolSetting("Enable Scaling", "Enable or disable hand scaling.", &enableScaling, true));
        registerSetting(new BoolSetting("Enable Rotation", "Enable or disable hand rotation.", &enableRotation, true));
        registerSetting(new SliderSetting<float>("Translation X", "Set the translation along the X-axis.", &translationValues.x, 0.f, -2.f, 2.f));
        registerSetting(new SliderSetting<float>("Translation Y", "Set the translation along the Y-axis.", &translationValues.y, 0.f, -2.f, 2.f));
        registerSetting(new SliderSetting<float>("Translation Z", "Set the translation along the Z-axis.", &translationValues.z, 0.f, -5.f, 2.f));
        registerSetting(new SliderSetting<float>("Scaling X", "Set the scaling along the X-axis.", &scalingValues.x, 1.f, -3.f, 3.f));
        registerSetting(new SliderSetting<float>("Scaling Y", "Set the scaling along the Y-axis.", &scalingValues.y, 1.f, -2.f, 2.f));
        registerSetting(new SliderSetting<float>("Scaling Z", "Set the scaling along the Z-axis.", &scalingValues.z, 1.f, -2.f, 5.f));
        registerSetting(new SliderSetting<float>("Rotation X", "Set the rotation along the X-axis.", &rotationValues.x, 0.f, 0.f, 2.f));
        registerSetting(new SliderSetting<float>("Rotation Y", "Set the rotation along the Y-axis.", &rotationValues.y, 0.f, 0.f, 2.f));
        registerSetting(new SliderSetting<float>("Rotation Z", "Set the rotation along the Z-axis.", &rotationValues.z, 0.f, 0.f, 2.f));
        registerSetting(new SliderSetting<float>("Rotation Angle", "Adjust the overall rotation angle of the hand.", &rotationAngle, 0.f, -6.3f, 6.3f));
        registerSetting(new BoolSetting("Reset to Defaults", "Resets all settings to their default values.", &resetToDefaults, false));
    }

    bool isSwordEquipped() {
        int selectedItem = InventoryUtils::getSelectedItemId();
        return selectedItem == 314 || selectedItem == 318 || selectedItem == 313 || selectedItem == 329 || selectedItem == 322 || selectedItem == 622;
    }

    void onEnable() override {
        if (fluxSwingAddr != nullptr)
            MemoryUtil::readBytes(fluxSwingAddr, ogBytes, 5);
    }

    void onDisable() override {
        if (fluxSwingAddr != nullptr && swingType == 0)
            MemoryUtil::writeBytes((uintptr_t)fluxSwingAddr, (char*)ogBytes, 5);
    }

    void onMatrixRender(glm::mat4* matrix) override {
        glm::mat4& mat = *matrix;

        if (swingType == 0) {
            MemoryUtil::writeBytes((uintptr_t)fluxSwingAddr, "\x90\x90\x90\x90\x90", 5);
        }
        else {
            MemoryUtil::writeBytes((uintptr_t)fluxSwingAddr, (char*)ogBytes, 5);
        }

        if (resetToDefaults) {
            translationValues = Vec3<float>(0.f, 0.f, 0.f);
            scalingValues = Vec3<float>(1.f, 1.f, 1.f);
            rotationValues = Vec3<float>(0.1f, 0.f, 0.f);
            rotationAngle = 0.f;
            resetToDefaults = false;
        }

        if (enableTranslation)
            mat = glm::translate<float>(mat, glm::vec3(translationValues.x, translationValues.y, translationValues.z));
        if (enableScaling)
            mat = glm::scale<float>(mat, glm::vec3(scalingValues.x, scalingValues.y, scalingValues.z));
        if (enableRotation)
            mat = glm::rotate<float>(mat, rotationAngle, glm::vec3(rotationValues.x, rotationValues.y, rotationValues.z));
        /* blocking start */
        if (blockAnim == 0) { //flux
            if ((GetAsyncKeyState(VK_RBUTTON)) && Game::canUseMoveKeys() && isSwordEquipped() || shouldBlockForViewModel && Game::canUseMoveKeys() && isSwordEquipped()) {
                mat = glm::translate<float>(mat, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
                mat = glm::translate<float>(mat, glm::vec3(-0.1f, 0.15f, -0.2f));
                mat = glm::translate<float>(mat, glm::vec3(-0.24F, 0.25f, -0.20F));
                mat = glm::rotate<float>(mat, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
                mat = glm::rotate<float>(mat, 1.30F, glm::vec3(4.0F, 0.0F, 0.0F));
                mat = glm::rotate<float>(mat, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
            }
        }
        if (blockAnim == 1) { //test
            if ((GetAsyncKeyState(VK_RBUTTON)) && Game::canUseMoveKeys() && isSwordEquipped() || shouldBlockForViewModel && Game::canUseMoveKeys() && isSwordEquipped()) {
                mat = glm::rotate<float>(mat, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
                mat = glm::rotate<float>(mat, 1.30F, glm::vec3(4.0F, 0.0F, 0.0F));
                mat = glm::rotate<float>(mat, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
            }
        }
        /* blocking end */
    }

    void onD2DRender() override {}
};
