# 🌐 NexBox (PeterBox) • C++ Multiplayer 
<img width="1280" height="719" alt="изображение" src="https://github.com/user-attachments/assets/3229da03-aa3e-43f6-990b-e6e5b8ede52c" />


Native **C++** multiplayer modification and runtime proxy library (`version.dll`) for **NexBox (PitahBox)**, enabling custom networking, server redirection, and live memory/function hooks without modifying original game binaries.

### ✨ How It Works & Features
- **Proxy DLL Injection (`version.dll`):** Transparently forwards original Windows `version.dll` exports while initializing the custom multiplayer runtime on game startup.
- **Custom Multiplayer Networking:** Replaces/redirects stock networking endpoints to custom master & game servers.
- **Runtime Hooking & Patching:** Intercepts game loop, player synchronization, and session events in memory.
- **Zero Binary Modification:** Drop-in `version.dll` deployment straight into the game root folder.

---

### 🛠️ Tech Stack
- **Language:** C++ (C++17 / C++20)
- **Techniques:** DLL Hijacking / Export Forwarding (`version.dll`), Function Hooking, Memory Patching
- **Build System:** Visual Studio 2022 (MSVC x64)
- **Platform:** Windows (x64)

---

### 🚀 Building from Source
1. Clone the repository:
   ```bash
   git clone https://github.com/deyxzazz/NexBox-PeterBox-OpenSource
   ```
2. Open the `.sln` file in **Visual Studio 2022**.
3. Set build configuration to **Release | x64** and press **Ctrl + Shift + B**.
4. Copy the compiled `version.dll` into the game directory next to the main executable.

---

### 👤 Authors
- **GitHub:** [@deyxzazz](https://github.com/deyxzazz)
- **Telegram:** [@NexBoxRe](@NexBoxRe)
- **Telegram2:** [@PetrZvarka](@PetrZvarka)
- **Telegram3:** [@dottextlol](@dottextlol)
