#include "AntiCheat.hpp"

#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <fstream>
#include <cwchar>


namespace fs = std::filesystem;

namespace
{
    void Log(const char* text)
    {
        std::ofstream file(
            "fuck you.log",
            std::ios::app);

        file << text << std::endl;
    }

    bool ProcessExists(const wchar_t* target)
    {
        HANDLE snapshot =
            CreateToolhelp32Snapshot(
                TH32CS_SNAPPROCESS,
                0);

        if (snapshot == INVALID_HANDLE_VALUE)
            return false;

        PROCESSENTRY32 pe{};
        pe.dwSize = sizeof(pe);

        if (Process32First(snapshot, &pe))
        {
            do
            {
                if (_wcsicmp(pe.szExeFile, target) == 0)
                {
                    CloseHandle(snapshot);
                    return true;
                }

            } while (Process32Next(snapshot, &pe));
        }

        CloseHandle(snapshot);
        return false;
    }

    bool CheckDebugger()
    {
        if (IsDebuggerPresent())
        {
            Log("Debugger detected");
            return true;
        }

        BOOL remote = FALSE;

        CheckRemoteDebuggerPresent(
            GetCurrentProcess(),
            &remote);

        if (remote)
        {
            Log("Remote debugger detected");
            return true;
        }

        return false;
    }

    bool CheckProcesses()
    {
        const wchar_t* blacklist[] =
        {
            L"cheatengine-x86_64.exe",
            L"cheatengine-i386.exe",
            L"x64dbg.exe",
            L"x32dbg.exe",
            L"ollydbg.exe",
            L"ProcessHacker.exe",
            L"dnSpy.exe",
            L"MegaDumper.exe",
            L"Extreme Injector.exe"
        };

        for (auto process : blacklist)
        {
            if (ProcessExists(process))
            {
                Log("Forbidden process detected");
                return true;
            }
        }

        return false;
    }

    bool CheckFiles()
    {
        const char* forbidden[] =
        {
            "BepInEx",
            "MelonLoader",
            "Mods",
            "dotnet",
            "Plugins",

            "BepInEx.cfg",
            ".doorstop_version",
            "doorstop_config.ini",
            "MelonLoader.dll",

            "UserLibs",

            "winhttp.dll",
            "dxgi.dll",
            "dinput8.dll",

            "0Harmony.dll",
            "Mono.Cecil.dll",
            "MonoMod.RuntimeDetour.dll"
        };

        for (auto item : forbidden)
        {
            if (fs::exists(item))
            {
                Log(item);
                return true;
            }
        }

        return false;
    }

    DWORD WINAPI Worker(LPVOID)
    {
        while (true)
        {
            if (CheckDebugger())
            {
                TerminateProcess(
                    GetCurrentProcess(),
                    0);

                return 0;
            }

            if (CheckProcesses())
            {
                TerminateProcess(
                    GetCurrentProcess(),
                    0);

                return 0;
            }

            if (CheckFiles())
            {
                TerminateProcess(
                    GetCurrentProcess(),
                    0);

                return 0;
            }

            Sleep(500);
        }

        return 0;
    }
}

bool AntiCheat::StartupCheck()
{
    if (CheckDebugger())
        return false;

    if (CheckProcesses())
        return false;

    if (CheckFiles())
        return false;

    return true;
}

void AntiCheat::Init()
{
    CreateThread(
        nullptr,
        0,
        Worker,
        nullptr,
        0,
        nullptr);
}
//this govno a ne anticheat