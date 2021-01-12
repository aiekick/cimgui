Project(cimgui_sdl)
cmake_minimum_required(VERSION 3.1)
if(WIN32) #to mingw work as all the others
set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif(WIN32)
#general settings

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../imgui/backends)
	set(BAKENDS_FOLDER "../imgui/backends/")
else()
	set(BAKENDS_FOLDER "../imgui/examples/")
endif()

message(STATUS "BAKENDS_FOLDER : ${BAKENDS_FOLDER}")

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../imgui/imgui_tables.cpp)
	set(TABLES_SOURCE "../imgui/imgui_tables.cpp")
else()
	set(TABLES_SOURCE "")
endif()

message(STATUS "TABLES_SOURCE : ${TABLES_SOURCE}")

include_directories(../imgui)
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

include_directories(../ImGuiFileDialog)
add_definitions(-DCUSTOM_IMGUIFILEDIALOG_CONFIG="../backend_test/CustomImGuiFileDialogConfig.h")

include_directories(../)
set(IMGUI_SOURCES ../cimgui.cpp 
../imgui/imgui.cpp 
../imgui/imgui_draw.cpp 
../imgui/imgui_demo.cpp 
../imgui/imgui_widgets.cpp
../ImGuiFileDialog/ImGuiFileDialog.cpp
../ImGuiFileDialog/ImGuiFileDialog.h
${TABLES_SOURCE} 
)

set(IMGUI_SOURCES_sdl)
set(IMGUI_LIBRARIES )

if (WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

#optional adding freetype
option(IMGUI_FREETYPE "add Freetype2" OFF)

if(IMGUI_FREETYPE)
	FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
	list(APPEND IMGUI_LIBRARIES freetype)
	list(APPEND IMGUI_SOURCES ../imgui/misc/freetype/imgui_freetype.cpp)
	add_definitions("-DCIMGUI_FREETYPE=1")
endif(IMGUI_FREETYPE)

#opengl3
list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_opengl3.cpp ../imgui/examples/libs/gl3w/GL/gl3w.c)
list(APPEND IMGUI_SOURCES ./cimgui_extras.cpp)
include_directories(../imgui/examples/libs/gl3w)
if(WIN32)
       list(APPEND IMGUI_LIBRARIES opengl32)
else(WIN32)#Unix
       list(APPEND IMGUI_LIBRARIES GL)
endif(WIN32)


#sdl2
list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_sdl.cpp)
if(DEFINED SDL_PATH)
	message(STATUS "SDL_PATH defined as " ${SDL_PATH})
	FIND_PACKAGE(SDL2 REQUIRED PATHS ${SDL_PATH})
	message(STATUS "SDL2_LIBRARIES defined as " ${SDL2_LIBRARIES})
	message(STATUS "SDL_INCLUDE defined as " ${SDL_INCLUDE})
	message(STATUS "SDL2_DLL defined as " ${SDL2_DLL})
	##get_target_property(SDL_INCLUDE SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
	if ("${SDL_INCLUDE}" STREQUAL "" OR "${SDL_INCLUDE}" STREQUAL "SDL_INCLUDE-NOTFOUND") #if not found latest SDL2 cmake config use older
		message(STATUS "sdlinclude2 is " ${SDL2_INCLUDE_DIRS})
		include_directories(${SDL2_INCLUDE_DIRS})
		set(IMGUI_SDL_LIBRARY ${SDL2_LIBRARIES})
		message(STATUS IMGUI_SDL_LIBRARY ${SDL2_LIBRARIES})
	else()#use new one SDL2 config
		include_directories(${SDL_INCLUDE})
		set(IMGUI_SDL_LIBRARY SDL2::SDL2)
		set(SDL_MAIN SDL2::SDL2main)
		message(STATUS ${SDL_MAIN} ${IMGUI_SDL_LIBRARY})
	endif()
else(DEFINED SDL_PATH)
	message(STATUS "SDL_PATH not defined")
	set(IMGUI_SDL_LIBRARY SDL2)
endif(DEFINED SDL_PATH)

add_library(cimgui_sdl SHARED ${IMGUI_SOURCES})
target_link_libraries(cimgui_sdl ${IMGUI_LIBRARIES} ${IMGUI_SDL_LIBRARY})

 #copy dll
add_custom_command ( TARGET cimgui_sdl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

file(GLOB SRC_TREE 
${CMAKE_CURRENT_SOURCE_DIR}/*.c
${CMAKE_CURRENT_SOURCE_DIR}/*.h)

#using library
include_directories(../generator/output/)
add_executable(test_sdl ${SRC_TREE})
if (MINGW)
target_link_options(test_sdl PRIVATE "-mconsole")
endif()
target_link_libraries(test_sdl ${IMGUI_SDL_LIBRARY} cimgui_sdl)

