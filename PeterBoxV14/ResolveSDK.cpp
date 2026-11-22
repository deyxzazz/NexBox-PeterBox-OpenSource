#include "Il2CppAPI.hpp"
#include "Structs.hpp"
#include "Helpers.hpp"
#include "ResolveSDK.hpp"

auto Main::Resolve::Start() -> void {
	ResolveMethod(LoginWithCustomID, "PlayFab", "PlayFab", "PlayFabClientAPI", "LoginWithCustomID", 5);
	ResolveMethod(SystemInfo::DeviceID, "UnityEngine", "UnityEngine", "SystemInfo", "get_deviceUniqueIdentifier", 0);
	ResolveMethod(Network::GetPlayFabSharedPrivate, "PlayFab", "PlayFab", "PlayFabSettings", "get_PlayFabSharedPrivate", 0);
	ResolveMethod(Network::GetPhotonServerSettings, "Assembly-CSharp", "Photon.Pun", "PhotonNetwork", "get_PhotonServerSettings", 0);
}