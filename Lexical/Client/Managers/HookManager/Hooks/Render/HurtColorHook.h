#pragma once
#include "../FuncHook.h"

class HurtColorHook : public FuncHook {
private:
    using func_t = mce::Color(__thiscall*)(void*, mce::Color, __int64);  

    static inline func_t oFunc;

    static mce::Color HurtColorCallback(void* a1, mce::Color color, __int64 a3) {
        static HurtColor* hurtColorMod = ModuleManager::getModule<HurtColor>();

        if (!oFunc) {
            return color;
        }

        mce::Color result = oFunc(a1, color, a3);

        if (hurtColorMod->isEnabled()) {
            mce::Color newColor = color;
            newColor.r = hurtColorMod->hurtColor.r / 255.f;
            newColor.g = hurtColorMod->hurtColor.g / 255.f;
            newColor.b = hurtColorMod->hurtColor.b / 255.f;
            newColor.a = hurtColorMod->hurtColor.a / 255.f;

            return newColor;
        }

        return result;
    }
public:
    HurtColorHook() {
        OriginFunc = (void*)&oFunc;
        func = (void*)&HurtColorCallback;
    }
};
