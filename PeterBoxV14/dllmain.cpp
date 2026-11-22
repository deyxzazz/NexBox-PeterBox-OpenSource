#include <windows.h>
#include "Minhook.h"
#include "IL2CPPApi.hpp"
#include "VersionDLLProxy.hpp"
#include "PlayFabANDPhoton.hpp"
#include "ResolveSDK.hpp"
#include "NewsAndVersionPatch.hpp"
#include "SteamPatch.hpp"
#include "DiscordRPC.hpp"
#include "AntiCheat.hpp"

uintptr_t GameAssembly = NULL;

DWORD WINAPI MainThread(LPVOID lpParam) {
    while (!(GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll"))) {
        Sleep(200);
    }

    Il2Cpp::init(GameAssembly);

    if (!AntiCheat::StartupCheck())
    {
        TerminateProcess(
            GetCurrentProcess(),
            0);

        return 0;
    }

    MH_Initialize();

    AntiCheat::Init();

    DiscordRPC::Init();

    Main::Resolve::Start();

    Patches::SteamPatch::Start();
    Patches::Playfab::Start();
    Patches::NewsAndVersion::Start();

    MH_EnableHook(MH_ALL_HOOKS);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID lpRes) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        Main::Proxy::Start();
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);
    }
    return TRUE;
}