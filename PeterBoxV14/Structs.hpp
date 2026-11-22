#pragma once
#include "Il2CppAPI.hpp"

struct PlayFabBaseModel : Il2CppObject {};

struct PlayFabRequestCommon : PlayFabBaseModel {
    void* AuthenticationContext;
};

struct LoginWithCustomIDRequest : PlayFabRequestCommon {
    int32_t CreateAccount;
    Il2CppString* CustomId;
    void* CustomTags;
    Il2CppString* EncryptedRequest;
    void* InfoRequestParameters;
    Il2CppString* PlayerSecret;
    Il2CppString* TitleId;

    static LoginWithCustomIDRequest* create();
};

struct SteamId {
    unsigned long steamId;
};