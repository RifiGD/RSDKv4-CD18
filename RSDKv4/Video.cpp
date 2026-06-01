#include "RetroEngine.hpp"
#include <string>

// Most of this code is copied from the RSDKv3 Decompilation.
// Obviously, there's some changes I had to do to match the mobile video files

void PlayVideoFile(char *filePath)
{
    char pathBuffer[0x100];
    int len = StrLength(filePath);

    // Just... no
    // if (StrComp(filePath + ((size_t)len - 2), "us")) {
    //     filePath[len - 2] = 0;
    // }

    StrCopy(pathBuffer, "videos/");
    StrAdd(pathBuffer, filePath);
    StrAdd(pathBuffer, ".mp4");

    bool addPath = true;
    
    // Fixes ".ani" ".Ani" bug and any other case differences
    char pathLower[0x100];
    memset(pathLower, 0, sizeof(char) * 0x100);
    for (int c = 0; c < strlen(pathBuffer); ++c) {
        pathLower[c] = tolower(pathBuffer[c]);
    }

#if RETRO_USE_MOD_LOADER
    for (int m = 0; m < modList.size(); ++m) {
        if (modList[m].active) {
            std::map<std::string, std::string>::const_iterator iter = modList[m].fileMap.find(pathLower);
            if (iter != modList[m].fileMap.cend()) {
                StrCopy(pathBuffer, iter->second.c_str());
                // Engine.forceFolder   = true;
                Engine.usingDataFile = false;
                addPath              = false;
                break;
            }
        }
    }
#endif

    char filepath[0x100];
    if (addPath) {
#if RETRO_PLATFORM == RETRO_UWP
        static char resourcePath[256] = { 0 };

        if (strlen(resourcePath) == 0) {
            auto folder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
            auto path   = to_string(folder.Path());

            std::copy(path.begin(), path.end(), resourcePath);
        }

        sprintf(filepath, "%s/%s", resourcePath, pathBuffer);
#elif RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_ANDROID
        sprintf(filepath, "%s/%s", gamePath, pathBuffer);
#else
        sprintf(filepath, "%s%s", BASE_PATH, pathBuffer);
#endif
    }
    else {
        sprintf(filepath, "%s", pathBuffer);
    }

    FileIO *file = fOpen(filepath, "rb");
    if (file) {
        PrintLog("Loaded File '%s'!", filepath);
#if RETRO_PLATFORM == RETRO_WIN
    // this just plays the video in your default video player
    // windows only cuz... uh... :durr:
        char cmd[0x200];

        IniParser ini;

        // tried my best :sob:
        // technically nothing is stopping you from like...
        // clicking back into the main window while the video plays
        disableFocusPause = 1;
        sprintf(cmd, "start %s", pathBuffer);
        system(cmd);
#endif

//         callbacks.read     = videoRead;
//         callbacks.close    = videoClose;
//         callbacks.userdata = (void *)file;
// #if RETRO_USING_SDL2 && !RETRO_USING_OPENGL
//         videoDecoder = THEORAPLAY_startDecode(&callbacks, /*FPS*/ 30, THEORAPLAY_VIDFMT_IYUV, GetGlobalVariableByName("Options.Soundtrack") ? 1 : 0);
// #endif

//         // TODO: does SDL1.2 support YUV?
// #if RETRO_USING_SDL1 && !RETRO_USING_OPENGL
//         videoDecoder = THEORAPLAY_startDecode(&callbacks, /*FPS*/ 30, THEORAPLAY_VIDFMT_RGBA, GetGlobalVariableByName("Options.Soundtrack") ? 1 : 0);
// #endif

// #if RETRO_USING_OPENGL
//         videoDecoder = THEORAPLAY_startDecode(&callbacks, /*FPS*/ 30, THEORAPLAY_VIDFMT_RGBA, GetGlobalVariableByName("Options.Soundtrack") ? 1 : 0);
// #endif

//         if (!videoDecoder) {
//             PrintLog("Video Decoder Error!");
//             return;
//         }
//         while (!videoVidData) {
//             if (!videoVidData)
//                 videoVidData = THEORAPLAY_getVideo(videoDecoder);
//         }
//         if (!videoVidData) {
//             PrintLog("Video Error!");
//             return;
//         }

//         videoWidth  = videoVidData->width;
//         videoHeight = videoVidData->height;
//         // commit video Aspect Ratio.
//         videoAR = float(videoWidth) / float(videoHeight);

//         SetupVideoBuffer(videoWidth, videoHeight);
//         vidBaseticks = SDL_GetTicks();
//         vidFrameMS   = (videoVidData->fps == 0.0) ? 0 : ((Uint32)(1000.0 / videoVidData->fps));
//         videoPlaying = 1; // playing ogv
//         trackID      = TRACK_COUNT - 1;

//         videoSkipped    = false;
//         Engine.gameMode = ENGINE_VIDEOWAIT;
    }
    else {
        PrintLog("Couldn't find file '%s'!", filepath);
    }
    
}