#include "Il2CppAPI.hpp"
#include "Structs.hpp"
#include "Helpers.hpp"
#include "PlayFabANDPhoton.hpp"
#include "minhook.h"

static auto PatchPhotonANDPlayFab() -> void {
    auto serverSettings = Network::GetPhotonServerSettings();
    if (!serverSettings) return;

    auto appSettings = Il2Cpp::read<uintptr_t>(reinterpret_cast<uintptr_t>(serverSettings) + 0x18);
    if (!appSettings) return;

    Il2Cpp::write<Il2CppString*>(appSettings + 0x10, Il2Cpp::String::New("5")); //photon appid
    Il2Cpp::write<Il2CppString*>(appSettings + 0x28, Il2Cpp::String::New("9")); //photon voice

    auto playfabSharedPrivate = Network::GetPlayFabSharedPrivate();
    if (!playfabSharedPrivate) return;
    Il2Cpp::write<Il2CppString*>(reinterpret_cast<uintptr_t>(playfabSharedPrivate) + 0x18, Il2Cpp::String::New("1")); //title ID
}
static auto loginWithSteamHook(void* _request, void* onLogin, void* onError, void* customData, void* extraHeaders) -> void {
    PatchPhotonANDPlayFab();
    auto req = LoginWithCustomIDRequest::create(); // new req

    req->CreateAccount = 0x101; // create acc
    req->CustomId = SystemInfo::DeviceID(); // custom id

    LoginWithCustomID(req, onLogin, onError, customData, extraHeaders);
}

auto Patches::Playfab::Start() -> void {
    auto login = Il2Cpp::Api::getMethod("PlayFab", "PlayFab", "PlayFabClientAPI", "LoginWithSteam", 5);
    MH_CreateHook((LPVOID)login->methodPointer, (LPVOID)&loginWithSteamHook, nullptr);
}