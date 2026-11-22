#pragma once
#include <windows.h>
#include <string>
#include <string_view>
#include <cstdint>

struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppImage;
struct Il2CppClass;
struct MonitorData;
struct Il2CppString;
struct Il2CppThread;

struct Il2CppObject {
    Il2CppClass* klass;
    MonitorData* monitor;
};

struct MethodInfo {
    void* methodPointer;
};

struct Il2CppString {
    void* klass;
    void* monitor;
    int32_t length;
    char16_t chars[1];

    std::wstring ToWString() const {
        if (!this || length <= 0) return L"";
        return std::wstring(reinterpret_cast<const wchar_t*>(chars), length);
    }

    std::string ToString() const {
        std::wstring wstr = ToWString();
        if (wstr.empty()) return "";
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }

    bool Contains(std::string_view value) const {
        if (!this || length <= 0) return false;
        std::string currentStr = ToString();
        return currentStr.find(value) != std::string::npos;
    }

    bool Contains(std::wstring_view value) const {
        if (!this || length <= 0) return false;
        std::wstring currentWStr = ToWString();
        return currentWStr.find(value) != std::wstring::npos;
    }
};

namespace Il2Cpp {
    namespace Fn {
        inline Il2CppDomain* (*domain_get)() = nullptr;
        inline Il2CppAssembly* (*domain_assembly_open)(Il2CppDomain*, const char*) = nullptr;
        inline Il2CppImage* (*assembly_get_image)(Il2CppAssembly*) = nullptr;
        inline Il2CppClass* (*class_from_name)(Il2CppImage*, const char*, const char*) = nullptr;
        inline MethodInfo* (*class_get_method_from_name)(Il2CppClass*, const char*, int) = nullptr;
        inline Il2CppObject* (*object_new)(Il2CppClass*) = nullptr;
        inline Il2CppString* (*string_new)(const char*) = nullptr;
        inline Il2CppThread* (*thread_attach)(Il2CppDomain*) = nullptr;
    }

    inline bool init(uintptr_t base) {
        auto get_export = [base](const char* name) {
            return reinterpret_cast<void*>(GetProcAddress((HMODULE)base, name));
            };

        Fn::domain_get = (Il2CppDomain * (*)())get_export("il2cpp_domain_get");
        Fn::domain_assembly_open = (Il2CppAssembly * (*)(Il2CppDomain*, const char*))get_export("il2cpp_domain_assembly_open");
        Fn::assembly_get_image = (Il2CppImage * (*)(Il2CppAssembly*))get_export("il2cpp_assembly_get_image");
        Fn::class_from_name = (Il2CppClass * (*)(Il2CppImage*, const char*, const char*))get_export("il2cpp_class_from_name");
        Fn::class_get_method_from_name = (MethodInfo * (*)(Il2CppClass*, const char*, int))get_export("il2cpp_class_get_method_from_name");
        Fn::object_new = (Il2CppObject * (*)(Il2CppClass*))get_export("il2cpp_object_new");
        Fn::string_new = (Il2CppString * (*)(const char*))get_export("il2cpp_string_new");
        Fn::thread_attach = (Il2CppThread * (*)(Il2CppDomain*))get_export("il2cpp_thread_attach");

        return Fn::domain_get != nullptr && Fn::string_new != nullptr;
    }

    inline void attachThread() {
        if (Fn::thread_attach && Fn::domain_get) {
            auto domain = Fn::domain_get();
            if (domain) {
                Fn::thread_attach(domain);
            }
        }
    }

    struct Api {
        static Il2CppImage* getAssembly(std::string_view name) {
            auto domain = Fn::domain_get();
            if (!domain) return nullptr;

            std::string fullName(name);
            if (fullName.find(".dll") == std::string::npos) {
                fullName += ".dll";
            }

            auto assembly = Fn::domain_assembly_open(domain, fullName.c_str());
            return assembly ? Fn::assembly_get_image(assembly) : nullptr;
        }

        static Il2CppClass* getClass(Il2CppImage* image, std::string_view nameSpace, std::string_view className) {
            if (!image) return nullptr;
            return Fn::class_from_name(image, nameSpace.data(), className.data());
        }

        static MethodInfo* getMethod(Il2CppClass* klass, std::string_view name, int argCount = 0) {
            if (!klass) return nullptr;
            return Fn::class_get_method_from_name(klass, name.data(), argCount);
        }

        static MethodInfo* getMethod(std::string_view assemblyName, std::string_view nameSpace, std::string_view className, std::string_view methodName, int argCount = 0) {
            auto img = getAssembly(assemblyName);
            auto klass = getClass(img, nameSpace, className);
            return getMethod(klass, methodName, argCount);
        }
    };

    struct String {
        static Il2CppString* New(const char* str) {
            if (!Fn::string_new) return nullptr;
            attachThread();
            return Fn::string_new(str);
        }
    };

    template <typename T>
    inline void write(uintptr_t address, T value) {
        if (!address) return;
        DWORD oldProtect;
        if (VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect)) {
            *reinterpret_cast<T*>(address) = value;
            VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(T), oldProtect, &oldProtect);
        }
    }

    template <typename T>
    inline T read(uintptr_t address) {
        if (!address) return T{};
        return *reinterpret_cast<T*>(address);
    }
}