#include "DiscordRPC.hpp"
#include <discord_rpc.h>

#pragma comment(lib, "discord-rpc.lib")

namespace
{
    bool Initialized = false;
}

void DiscordRPC::Init()
{
    DiscordEventHandlers handlers{};

    Discord_Initialize(
        "твой айди приложиха ахахахахах иди нахзуй блять",
        &handlers,
        1,
        nullptr
    );

    DiscordRichPresence presence{};

    presence.details = "logo";
    presence.state = "Playing (your reborn name)";
    presence.largeImageKey = "logo";
    presence.largeImageText = "NexBox";

    Discord_UpdatePresence(&presence);

    Initialized = true;
}

void DiscordRPC::UpdateStatus(const char* state)
{
    if (!Initialized)
        return;

    DiscordRichPresence presence{};

    presence.details = "NexBox";
    presence.state = state;
    presence.largeImageKey = "logo";
    presence.largeImageText = "NexBox";

    Discord_UpdatePresence(&presence);
}

void DiscordRPC::Shutdown()
{
    if (!Initialized)
        return;

    Discord_Shutdown();
    Initialized = false;
}