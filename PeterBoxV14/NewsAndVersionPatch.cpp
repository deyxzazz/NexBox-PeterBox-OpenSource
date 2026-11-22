#include "Il2CppAPI.hpp"
#include "NewsAndVersionPatch.hpp"
#include "minhook.h"

// твои новостэ
static const char* NewNewsLink =
"https://raw.githubusercontent.com/";

// oригинальные новости GoreBox
static const char* OldNewsLink =
"gist.githubusercontent.com/GDevF2Games/ad43789723430941037f94d0f44dba06/raw"; //v1.15.17.16

using o_webReqGet_t = void(*)(Il2CppString* uri);

static o_webReqGet_t o_webReqGet = nullptr;

static auto UnityWebReqGet(Il2CppString* uri) -> void
{
    if (uri && uri->Contains(OldNewsLink))
    {
        uri = Il2Cpp::String::New(NewNewsLink);
    }

    return o_webReqGet(uri);
}

auto Patches::NewsAndVersion::Start() -> void
{
    auto UnityWebReq = Il2Cpp::Api::getMethod(
        "UnityEngine.UnityWebRequestModule",
        "UnityEngine.Networking",
        "UnityWebRequest",
        "Get",
        1
    );

    if (!UnityWebReq)
        return;

    MH_CreateHook(
        (LPVOID)UnityWebReq->methodPointer,
        (LPVOID)&UnityWebReqGet,
        (LPVOID*)&o_webReqGet
    );
}