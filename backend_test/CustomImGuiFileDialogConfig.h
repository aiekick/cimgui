#pragma once

// uncomment and modify defines under for customize ImGuiFileDialog

//#define MAX_FILE_DIALOG_NAME_BUFFER 1024
//#define MAX_PATH_BUFFER_SIZE 1024

#define USE_IMGUI_TABLES

#include "SDL_scancode.h"

#define USE_EXPLORATION_BY_KEYS
// Up key for explore to the top
#define IGFD_KEY_UP SDL_SCANCODE_UP
// Down key for explore to the bottom
#define IGFD_KEY_DOWN SDL_SCANCODE_DOWN
// Enter key for open directory
#define IGFD_KEY_ENTER SDL_SCANCODE_RETURN
// BackSpace for comming back to the last directory
#define IGFD_KEY_BACKSPACE SDL_SCANCODE_BACKSPACE

// widget
// filter combobox width
//#define FILTER_COMBO_WIDTH 120.0f
// button widget use for compose path
//#define IMGUI_PATH_BUTTON ImGui::Button
// standar button
//#define IMGUI_BUTTON ImGui::Button


//#define fileNameString "File Name : "
//#define buttonResetSearchString "Reset search"
//#define buttonDriveString "Drives"
//#define buttonResetPathString "Reset to current directory"
//#define buttonCreateDirString "Create Directory"

/*
Comment theses line if you not want to have customization, like icon font here
*/

#include "CustomFont.h"

// locales string
#define createDirButtonString ICON_IGFD_ADD
#define okButtonString ICON_IGFD_OK " OK"
#define cancelButtonString ICON_IGFD_CANCEL " Cancel"
#define resetButtonString ICON_IGFD_RESET
#define drivesButtonString ICON_IGFD_DRIVES
#define searchString ICON_IGFD_SEARCH
#define dirEntryString ICON_IGFD_FOLDER
#define linkEntryString ICON_IGFD_LINK
#define fileEntryString ICON_IGFD_FILE
//#define buttonResetSearchString "Reset search"
//#define buttonDriveString "Drives"
//#define buttonResetPathString "Reset to current directory"
//#define buttonCreateDirString "Create Directory"
//#define OverWriteDialogTitleString "The file Already Exist !"
//#define OverWriteDialogMessageString "Would you like to OverWrite it ?"
#define OverWriteDialogConfirmButtonString ICON_IGFD_OK " Confirm"
#define OverWriteDialogCancelButtonString ICON_IGFD_CANCEL " Cancel"

// theses icons will appear in table headers
#define USE_CUSTOM_SORTING_ICON
#define tableHeaderAscendingIcon ICON_IGFD_SORT_ASC
#define tableHeaderDescendingIcon ICON_IGFD_SORT_DESC
#define tableHeaderFileNameString " File name"
#define tableHeaderFileSizeString " Size"
#define tableHeaderFileDateString " Date"

#define USE_BOOKMARK
//#define bookmarkPaneWith 150.0f
//#define IMGUI_TOGGLE_BUTTON ToggleButton
#define bookmarksButtonString ICON_IGFD_BOOKMARK
//#define bookmarksButtonHelpString "bookmark"
#define addBookmarkButtonString ICON_IGFD_ADD
#define removeBookmarkButtonString ICON_IGFD_REMOVE
