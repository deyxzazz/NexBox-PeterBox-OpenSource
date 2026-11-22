#pragma once
#include <string_view>
#include "Il2CppAPI.hpp"
#include "Structs.hpp"

using str_view = const char*;

inline auto ResolveMethod = []<typename T>(T & fn, str_view assembly, str_view ns,
    str_view klass, str_view method, int argc = 0) {
    auto* m = Il2Cpp::Api::getMethod(assembly, ns, klass, method, argc);
    fn = (m && m->methodPointer) ? reinterpret_cast<T>(m->methodPointer) : nullptr;
};

inline auto LoginWithCustomIDRequest::create() -> LoginWithCustomIDRequest* {
	using namespace Il2Cpp;

	static auto klass = Api::getClass(Api::getAssembly("PlayFab"), "PlayFab.ClientModels", "LoginWithCustomIDRequest");
	return static_cast<LoginWithCustomIDRequest*>(Fn::object_new(klass));
}

inline void (*LoginWithCustomID)(LoginWithCustomIDRequest*, void*, void*, void*, void*) = nullptr;

namespace Network {
	inline void* (*GetPhotonServerSettings)() = nullptr;
	inline void* (*GetPlayFabSharedPrivate)() = nullptr;
}

namespace SystemInfo {
	inline Il2CppString* (*DeviceID)() = nullptr;
}