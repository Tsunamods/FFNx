<div align="center">
  <img src=".screens/ff7.png" alt="Final Fantasy VII running on Vulkan!">
  <br><strong><small>Final Fantasy VII running on Vulkan!</small></strong>
</div>
<br><br>
<div align="center">
  <img src=".screens/ff8.png" alt="Final Fantasy VIII running on Vulkan!">
  <br><strong><small>Final Fantasy VIII running on Vulkan!</small></strong>
</div>
---

# FFNx

Next generation driver for Final Fantasy VII and Final Fantasy VIII ( with native Steam 2013 release support! )

## Introduction
Welcome to FFNx project. This is an attempt to move forward what you were used to know as [FF7_OpenGL](https://github.com/Aali132/ff7_opengl) driver, [made by Aali](http://forums.qhimm.com/index.php?topic=14922.0).

Some of the improvements that you can find on FFNx are:
- **Native** Steam support! No Game converter required.
- **Native** eStore support! No Game converter required.
- **Native** Vertical centering for Fields and Movies on Final Fantasy VII. No mods required.
- **Native** Fullscreen Battle scenes on Final Fantasy VII. No mods required.
- **Native** Menu cursor alignment in Menu scenes on Final Fantasy VII. No mods required.
- One single .dll file to be copied
- Support for 7h 1.x/2.x
- Support for newest video and audio codecs ( WEBM, H.265, etc.)
- In-Game music OGG files playback WITH LOOP support!
- Four different Rendering Backends:
  - Vulkan
  - OpenGL
  - DirectX 11
  - DirectX 12

## Modding
In order to enhance the modding experience, FFNx does also provide:
- Game rendering inspection through [RenderDoc](https://renderdoc.org/)!
- [DDS Texture support](https://beyondskyrim.org/tutorials/the-dds-texture-format) up to BC7 format, with PNG Texture support as fallback.
- Support for configurable external textures path using [mod_path](misc/FFNx.cfg#L80)
- Support for [Ficedula FF7Music](http://ff8.fr/pub/FF7Music.zip)

## Tech Stack
If you're curious to know it, FFNx is made with:
- C++ code base
- Latest MSVC available on [Visual Studio 2019 Community Edition](https://visualstudio.microsoft.com/vs/features/cplusplus/)
- [CMake](https://cmake.org/) ( as make files )
- [BGFX](https://github.com/bkaradzic/bgfx) ( as backend renderer )
- [BIMG](https://github.com/bkaradzic/bimg) ( for custom textures )
- [FFMpeg](https://www.ffmpeg.org/) 4.2.2 with H/W Accelleration support
- [VGMStream](https://github.com/julianxhokaxhiu/vgmstream) using FFMpeg as backend, [with loop support for Ogg files](https://github.com/julianxhokaxhiu/vgmstream/commit/249afed15176ba254c73055e8c5124b7e7cd4b95)
- [libconfuse](https://github.com/julianxhokaxhiu/libconfuse) 3.2.3 ( for the configuration management )
- [StackWalker](https://github.com/JochenKalmbach/StackWalker) ( for stack traces in the log file )

## Canary vs Latest Release
When you access the releases page, you will see two available download options:
- **Canary:** the latest snapshot of the code. Like a nightly, but untested.
- **Latest Release:** the official release, which is tested and should be stable enough for long gameplay sessions.

## How to install
In either way, in order to use this driver you MUST have a legal copy of the game. DO NOT ask for a copy unless bought legally.

### Final Fantasy VII

**Supported Languages:** EN, DE, FR, SP, JP*

> *: Japanese support is currently work in progress. The game starts fine but font is not rendering properly and battles do crash sometimes.

#### [1998 Eidos Release](https://www.mobygames.com/game/windows/final-fantasy-vii)
1. Install the game on this path: `C:\Games\Final Fantasy VII`
2. Update your game to v1.02 ( https://www.gamefront.com/games/final-fantasy-7-advent-children/file/final-fantasy-7-retail-v1-02-patch )
2. Download the latest `FFNx-FF7_1998` release here: https://github.com/julianxhokaxhiu/FFNx/releases
3. Extract the ZIP content next to `ff7.exe` file
4. Double click on [`FFNx.reg`](misc/FF7.reg)
5. Click on Yes.
6. Enjoy!

#### [2013 Steam Release](https://store.steampowered.com/app/39140/FINAL_FANTASY_VII/)
1. Install the game using Steam Client
2. Open the installation directory of the game ( see [How to access game files](https://steamcommunity.com/sharedfiles/filedetails/?id=760447682) )
3. Download the latest `FFNx-Steam` release here: https://github.com/julianxhokaxhiu/FFNx/releases
4. Extract the ZIP content next to your `ff7_*.exe` file ( for eg. for EN language `ff7_en.exe`)
5. Replace all files when asked.
6. Enjoy!

#### [2013 eStore Release](http://www.jp.square-enix.com/ffvii-pc-jp/)
1. Install the game using eStore installer.
2. Open the installation directory of the game
3. Download the latest `FFNx-Steam` release here: https://github.com/julianxhokaxhiu/FFNx/releases
4. Extract the ZIP content next to your `ff7_*.exe` file ( for eg. for EN language `ff7_en.exe`)
5. Replace all files when asked.
6. Enjoy!

#### [Android Release](https://play.google.com/store/apps/details?id=com.square_enix.android_googleplay.FFVII)
1. Install the game in your Android device.
2. Locate the OBB file ( usually in `Android/obb` or `/obb` in your internal storage )
3. Save the OBB file in your Windows desktop
4. Rename the OBB file extension from `.obb` to `.zip` and extract it
5. In the extracted folder, go to `ff7_1.02` directory
6. Download the latest `FFNx-Steam` release here: https://github.com/julianxhokaxhiu/FFNx/releases
7. Extract the ZIP content next to the `ff7_*.exe` files
8. Update `FFNx.cfg` flags with these values:
```
external_movie_ext = webm
external_music_path = data/music_2
external_music_ext = akb
```
9. You can now run any `ff7_*.exe` file you prefer. Enjoy!

### Final Fantasy VIII

**Supported Languages:** EN, DE, FR, SP, IT, JP

#### [2000 Squaresoft Release](https://www.mobygames.com/game/windows/final-fantasy-viii)
1. Install the game on this path: `C:\Games\Final Fantasy VIII`
2. Update your game to v1.2 ( search for `ff8_1.2.zip` or `ff8_1.2G.zip` here http://forums.qhimm.com/index.php?topic=12909.msg180223#msg180223 )
2. Download the latest `FFNx-FF8_2000` release here: https://github.com/julianxhokaxhiu/FFNx/releases
3. Extract the ZIP content next to `ff8.exe` file
4. Double click on [`FFNx.reg`](misc/FF8.reg)
5. Enjoy!

#### [2013 Steam Release](https://store.steampowered.com/app/39150/FINAL_FANTASY_VIII/)
1. Install the game using Steam Client
2. Open the installation directory of the game ( see [How to access game files](https://steamcommunity.com/sharedfiles/filedetails/?id=760447682) )
3. Download the latest `FFNx-Steam` release here: https://github.com/julianxhokaxhiu/FFNx/releases
4. Extract the ZIP content next to your `ff8_*.exe` file ( for eg. for EN language `ff8_en.exe`)
5. Replace all files when asked.
6. Enjoy!

## How to install on 7h 2.x
> **WARNING:** This method is NOT SUPPORTED officially by 7h team. Please use at your own risk. DO NOT approach 7h team if something is not working properly.

1) Download the latest version of FFNx here: https://github.com/julianxhokaxhiu/FFNx/releases
2) Unzip the newly downloaded package
3) Go to your 7h installation path (Default path is C:/7th Heaven)
4) Go to "Resources\Game Driver"
5) Rename "7H_GameDriver.dll" to "_7H_GameDriver.dll"
6) Place the "Shaders" folder from the FFNx download here
7) Rename "FFNx.dll" to "7H_GameDriver.dll"
8) Navigate to where your game is installed (wherever FF7.exe is located)
9) Place the remaining files from FFNx here and overwrite if needed
10) If there is one, rename "7H_GameDriver.dll" to "_7H_GameDriver.dll" in this folder too
11) Rename "FFNx.dll" to "7H_GameDriver.dll"
12) To configure the driver, right click "FFNx.cfg" and open it in notepad and make your edits

**NOTE**
Any lines that you alter in this file must have the # deleted from the start for it to be readable.
Once this change has been made, you will no longer be able to change these settings via 7H using the Setings, you MUST change them from THIS file.

Click Play and enjoy!

## Tweak the configuration
If you want a more advanced experience, for example using another backend renderer ( Vulkan instead of OpenGL, or DirectX 11 ) feel free to change the driver configuration file [FFNx.cfg](misc/FFNx.cfg).

## Inspect logs
If you want to check what is going on behind the scene, or you may happen to have a crash, feel free to check the `FFNx.log` file.

## Join us on Discord
If you want to join our community, you can find us on Discord: https://discord.gg/N6M6pKS

## License
See [LICENSE](LICENSE).
