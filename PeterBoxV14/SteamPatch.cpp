#include "Il2CppAPI.hpp"
#include "Structs.hpp"
#include "SteamPatch.hpp"
#include "minhook.h"

// Сигнатуры должны соответствовать оригинальным методам с учетом IL2CPP (обычно добавляется указатель метода и IntPtr/void* в начале)
auto SteamInitHook(void* instance) -> bool {
    return true;
}

// CSteamID в Steamworks.NET передается/возвращается с учетом структуры
auto SteamGetIdHook(void* instance) -> SteamId {
    return SteamId{ 1000 };
}

auto SteamGetTicketHook(void* instance, void* pTicket, int cbMaxTicket, uint32_t* pcbTicket, void* eAuthSessionTicketResponse) -> uint32_t {
    return 1; // Возвращаем фейковый handle тикета
}

auto SteamDetectAwakeHook(void* instance) -> void {
    return;
}

auto Patches::SteamPatch::Start() -> void {
    auto init = Il2Cpp::Api::getMethod("com.rlabrecque.steamworks.net", "Steamworks", "SteamAPI", "Init", 0);
    if (init) MH_CreateHook((LPVOID)init->methodPointer, (LPVOID)&SteamInitHook, nullptr);

    auto getSteamId = Il2Cpp::Api::getMethod("com.rlabrecque.steamworks.net", "Steamworks", "SteamUser", "GetSteamID", 0);
    if (getSteamId) MH_CreateHook((LPVOID)getSteamId->methodPointer, (LPVOID)&SteamGetIdHook, nullptr);

    auto getAuthTicket = Il2Cpp::Api::getMethod("com.rlabrecque.steamworks.net", "Steamworks", "SteamUser", "GetAuthSessionTicket", 4);
    if (getAuthTicket) MH_CreateHook((LPVOID)getAuthTicket->methodPointer, (LPVOID)&SteamGetTicketHook, nullptr);

    auto steamDetectAwk = Il2Cpp::Api::getMethod("Assembly-CSharp", "", "SteamDetection", "Awake", 0);
    if (steamDetectAwk) MH_CreateHook((LPVOID)steamDetectAwk->methodPointer, (LPVOID)&SteamDetectAwakeHook, nullptr);
}