#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_extras.h"
#include "cimgui_impl.h"
#include "ImGuiFileDialog.h"

#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

SDL_Window* window = NULL;

int main(int argc, char* argv[])
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("failed to init: %s", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // and prepare OpenGL stuff
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

    window = SDL_CreateWindow(
        "Hello", 0, 0, 1024, 768,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);  // enable vsync

    // Initialize OpenGL loader for cimgui_sdl
    bool err = Do_gl3wInit() != 0;
    if (err)
    {
        SDL_Log("Failed to initialize OpenGL loader for cimgui_sdl!");
        return 1;
    }

    // check opengl version sdl uses
    SDL_Log("opengl version: %s", (char*)glGetString(GL_VERSION));

    // setup imgui
    igCreateContext(NULL);

    //set docking
    ImGuiIO* ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
    ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
#endif

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(NULL);

    #include "CustomFont.c"
    ImFontAtlas_AddFontDefault(ioptr->Fonts, NULL);
    const ImWchar icons_ranges[3] = { ICON_IGFD_MIN, ICON_IGFD_MAX, 0 };
    ImFontConfig* icons_config = ImFontConfig_ImFontConfig();
    icons_config->MergeMode = true;
    icons_config->PixelSnapH = true;
    ImFontAtlas_AddFontFromMemoryCompressedBase85TTF(ioptr->Fonts, FONT_ICON_BUFFER_NAME_IGFD, 15.0f, icons_config, icons_ranges);
    ImFontConfig_destroy(icons_config);

    // create ImGuiFileDialog
    ImGuiFileDialog* cfiledialog = IGFD_Create();
    // define some files color
    IGFD_SetExtentionInfos2(cfiledialog, ".c", 1.0f, 1.0f, 0.0f, 0.9f, "");
    IGFD_SetExtentionInfos2(cfiledialog, ".h", 0.2f, 1.0f, 0.0f, 0.9f, "");

    bool showDemoWindow = true;
    bool showAnotherWindow = false;
    ImVec4 clearColor;
    clearColor.x = 0.45f;
    clearColor.y = 0.55f;
    clearColor.z = 0.60f;
    clearColor.w = 1.00f;

    bool quit = false;
    while (!quit)
    {
        SDL_Event e;

        // we need to call SDL_PollEvent to let window rendered, otherwise
        // no window will be shown
        while (SDL_PollEvent(&e) != 0)
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window))
                quit = true;
        }

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        igNewFrame();

        if (showDemoWindow)
            igShowDemoWindow(&showDemoWindow);

        // show a simple window that we created ourselves.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            
            igBegin("Hello, world!", NULL, 0);
            igText("This is some useful text");
            igCheckbox("Demo window", &showDemoWindow);
            igCheckbox("Another window", &showAnotherWindow);

            igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0);
            igColorEdit3("clear color", (float*)&clearColor, 0);

            if (igButton("Open File", buttonSize))
            {
                IGFD_OpenDialog(cfiledialog, 
                    "filedlg",                              // dialog key (make it possible to have different treatment reagrding the dialog key
                    "Open a File",                          // dialog title
                    "c files(*.c/*.h){.c,.h}",              // dialog filter syntax : simple => .h,.c,.pp, etc and collections : text1{filter0,filter1,filter2}, text2{filter0,filter1,filter2}, etc..
                    ".",                                    // base directory for files scan
                    "",                                     // base filename
                    0,                                      // a fucntion for display a right pane if you want
                    0.0f,                                   // base width of the pane
                    0,                                      // count selection : 0 infinite, 1 one file (default), n (n files)
                    "User data !",                          // some user datas
                    ImGuiFileDialogFlags_ConfirmOverwrite); // ImGuiFileDialogFlags
            }

            if (igButton("Button", buttonSize))
                counter++;
            igSameLine(0.0f, -1.0f);
            igText("counter = %d", counter);

            igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
            igEnd();

            // display of file dialog
            ImVec2 maxSize;
            maxSize.x = ioptr->DisplaySize.x * 0.8f;
            maxSize.y = ioptr->DisplaySize.y * 0.8f;
            ImVec2 minSize;
            minSize.x = maxSize.x * 0.25f;
            minSize.y = maxSize.y * 0.25f;
            
            // display dialog
            if (IGFD_DisplayDialog(cfiledialog, "filedlg", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
            {
                if (IGFD_IsOk(cfiledialog)) // result ok
                {
                    char* cfilePathName = IGFD_GetFilePathName(cfiledialog);
                    printf("GetFilePathName : %s\n", cfilePathName);
                    char* cfilePath = IGFD_GetCurrentPath(cfiledialog);
                    printf("GetCurrentPath : %s\n", cfilePath);
                    char* cfilter = IGFD_GetCurrentFilter(cfiledialog);
                    printf("GetCurrentFilter : %s\n", cfilter);
                    // here convert from string because a string was passed as a userDatas, but it can be what you want
                    void* cdatas = IGFD_GetUserDatas(cfiledialog);
                    if (cdatas)
                        printf("GetUserDatas : %s\n", (const char*)cdatas);

                    struct IGFD_Selection csel = IGFD_GetSelection(cfiledialog); // multi selection
                    printf("Selection :\n");
                    for (int i = 0; i < (int)csel.count; i++)
                    {
                        printf("(%i) FileName %s => path %s\n", i, csel.table[i].fileName, csel.table[i].filePathName);
                    }
                    // action

                    // destroy
                    if (cfilePathName) free(cfilePathName);
                    if (cfilePath) free(cfilePath);
                    if (cfilter) free(cfilter);

                    IGFD_Selection_DestroyContent(&csel);
                }
                IGFD_CloseDialog(cfiledialog);
            }
        }

        if (showAnotherWindow)
        {
            igBegin("imgui Another Window", &showAnotherWindow, 0);
            igText("Hello from imgui");
            ImVec2 buttonSize;
            buttonSize.x = 0; buttonSize.y = 0;
            if (igButton("Close me", buttonSize))
            {
                showAnotherWindow = false;
            }
            igEnd();
        }

        // render
        igRender();
        SDL_GL_MakeCurrent(window, gl_context);
        glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
        if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            igUpdatePlatformWindows();
            igRenderPlatformWindowsDefault(NULL, NULL);
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
#endif
        SDL_GL_SwapWindow(window);
    }

    // destroy ImGuiFileDialog
    IGFD_Destroy(cfiledialog);

    // clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    igDestroyContext(NULL);

    SDL_GL_DeleteContext(gl_context);
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();

    return 0;
}