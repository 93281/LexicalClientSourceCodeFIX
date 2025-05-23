#pragma once
#include "../FuncHook.h"
#include "../../../../../Renderer/D2D.h"
#include "../../../../Client.h"
#include <Windows.h>

class PresentHook : public FuncHook {
private:
    using present_t = HRESULT(__thiscall*)(IDXGISwapChain3*, UINT, UINT);
    static inline present_t oPresent;

    static HRESULT presentCallback(IDXGISwapChain3* swapChain, UINT syncInterval, UINT flags) {

        if (!Client::isInitialized())
            return oPresent(swapChain, syncInterval, flags);

        {
            static bool initDeltaTime = false;
            static std::chrono::steady_clock::time_point lastTime;
            static std::chrono::steady_clock::time_point currentTime;

            currentTime = std::chrono::steady_clock::now();

            if (initDeltaTime)
                D2D::deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;

            lastTime = currentTime;
            initDeltaTime = true;
        }

        HWND window = FindWindowA(nullptr, "Minecraft");

        if (!window)
            return oPresent(swapChain, syncInterval, flags);

        ID3D12Device* d3d12Device = nullptr;
        ID3D11Device* d3d11Device = nullptr;

        HRESULT hr = swapChain->GetDevice(IID_PPV_ARGS(&d3d12Device));
        if (SUCCEEDED(hr) && d3d12Device != nullptr) {
            d3d12Device->Release();
            return oPresent(swapChain, syncInterval, flags);
        }

        hr = swapChain->GetDevice(IID_PPV_ARGS(&d3d11Device));
        if (SUCCEEDED(hr) && d3d11Device != nullptr) {

            D2D::NewFrame(swapChain, d3d11Device, (float)GetDpiForWindow(window));
            D2D::Render();
            D2D::EndFrame();

            d3d11Device->Release();
        }

        return oPresent(swapChain, syncInterval, flags);
    }

public:
    PresentHook() {
        OriginFunc = (void*)&oPresent;
        func = (void*)&presentCallback;
    }
};
