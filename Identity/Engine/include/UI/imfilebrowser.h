
//The base of this code was not created by team LFG, we have simply modified it to suit our needs.
/*
MIT License

Copyright (c) 2019-2020 Zhuang Guan

https://github.com/AirGuanZ

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <Export.h>
#include <array>
#include <cstring>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../../../Editor/include/Core/Core.h"

#ifndef IMGUI_VERSION
#   error "include imgui.h before this header"
#endif

using ImGuiFileBrowserFlags = int;

enum ImGuiFileBrowserFlags_
{
    ImGuiFileBrowserFlags_SelectDirectory    = 1 << 0, // select directory instead of regular file
    ImGuiFileBrowserFlags_EnterNewFilename   = 1 << 1, // allow user to enter new filename when selecting regular file
    ImGuiFileBrowserFlags_NoModal            = 1 << 2, // file browsing window is modal by default. specify this to use a popup window
    ImGuiFileBrowserFlags_NoTitleBar         = 1 << 3, // hide window title bar
    ImGuiFileBrowserFlags_NoStatusBar        = 1 << 4, // hide status bar at the bottom of browsing window
    ImGuiFileBrowserFlags_CloseOnEsc         = 1 << 5, // close file browser when pressing 'ESC'
    ImGuiFileBrowserFlags_CreateNewDir       = 1 << 6, // allow user to create new directory
};

namespace Engine::UI
{
    class API_ENGINE FileBrowser
    {
    public:
        static FileBrowser* GetInstance()
        {
            if (!m_instance)
                m_instance = new FileBrowser();

            return m_instance;
        }
        // pwd is set to current working directory by default
        

        FileBrowser &operator=(const FileBrowser &copyFrom);

        // set the window size (in pixels)
        // default is (700, 450)
        void SetWindowSize(int width, int height) noexcept;

        // set the window title text
        void SetTitle(std::string title);

        // open the browsing window
        void Open();

        // close the browsing window
        void Close();

        // the browsing window is opened or not
        bool IsOpened() const noexcept;

        // display the browsing window if opened
        void Display();

        // returns true when there is a selected filename and the "ok" button was clicked
        bool HasSelected() const noexcept;

        // set current browsing directory
        bool SetPwd(const std::filesystem::path &pwd =
                                    std::filesystem::current_path());

        // returns selected filename. make sense only when HasSelected returns true
        std::filesystem::path GetSelected() const;

        // set selected filename to empty
        void ClearSelected();

        // set file type filters. eg. { ".h", ".cpp", ".hpp", ".cc", ".inl" }
        void SetTypeFilters(const std::vector<const char*> &typeFilters);

    private:

        static inline FileBrowser* m_instance = nullptr;

        explicit FileBrowser(ImGuiFileBrowserFlags flags = 0);

        FileBrowser(const FileBrowser& copyFrom);

        class ScopeGuard
        {
            std::function<void()> func_;

        public:

            template<typename T>
            explicit ScopeGuard(T func) : func_(std::move(func)) { }
            ~ScopeGuard() { func_(); }
        };

        void SetPwdUncatched(const std::filesystem::path &pwd);

#ifdef _WIN32
        static std::uint32_t GetDrivesBitMask();
#endif

        int width_;
        int height_;
        ImGuiFileBrowserFlags flags_;

        std::string title_;
        std::string openLabel_;

        bool openFlag_;
        bool closeFlag_;
        bool isOpened_;
        bool ok_;

        std::string statusStr_;

        std::vector<const char*> typeFilters_;
        int typeFilterIndex_;

        std::filesystem::path pwd_;
        std::filesystem::path selectedFilename_;

        struct FileRecord
        {
            bool isDir = false;
            std::filesystem::path name;
            std::string showName;
            std::filesystem::path extension;
        };
        std::vector<FileRecord> fileRecords_;

        // IMPROVE: truncate when selectedFilename_.length() > inputNameBuf_.size() - 1
        static constexpr size_t INPUT_NAME_BUF_SIZE = 512;
        std::unique_ptr<std::array<char, INPUT_NAME_BUF_SIZE>> inputNameBuf_;

        std::string openNewDirLabel_;
        std::unique_ptr<std::array<char, INPUT_NAME_BUF_SIZE>> newDirNameBuf_;

#ifdef _WIN32
        uint32_t drives_;
#endif
    };
} // namespace ImGui

inline Engine::UI::FileBrowser::FileBrowser(ImGuiFileBrowserFlags flags)
    : width_(700), height_(450), flags_(flags),
      openFlag_(false), closeFlag_(false), isOpened_(false), ok_(false),
      inputNameBuf_(std::make_unique<std::array<char, INPUT_NAME_BUF_SIZE>>())
{
    if(flags_ & ImGuiFileBrowserFlags_CreateNewDir)
        newDirNameBuf_ = std::make_unique<
                                std::array<char, INPUT_NAME_BUF_SIZE>>();

    inputNameBuf_->at(0) = '\0';
    SetTitle("file browser");
    SetPwd(std::filesystem::current_path());

    typeFilters_.clear();
    typeFilterIndex_ = 0;

#ifdef _WIN32
    drives_ = GetDrivesBitMask();
#endif
}

inline Engine::UI::FileBrowser::FileBrowser(const FileBrowser &copyFrom)
    : FileBrowser()
{
    *this = copyFrom;
}

inline Engine::UI::FileBrowser & Engine::UI::FileBrowser::operator=(
    const Engine::UI::FileBrowser &copyFrom)
{
    flags_ = copyFrom.flags_;
    SetTitle(copyFrom.title_);

    openFlag_  = copyFrom.openFlag_;
    closeFlag_ = copyFrom.closeFlag_;
    isOpened_  = copyFrom.isOpened_;
    ok_        = copyFrom.ok_;
    
    statusStr_ = "";
    pwd_ = copyFrom.pwd_;
    selectedFilename_ = copyFrom.selectedFilename_;

    fileRecords_ = copyFrom.fileRecords_;

    *inputNameBuf_ = *copyFrom.inputNameBuf_;

    if(flags_ & ImGuiFileBrowserFlags_CreateNewDir)
    {
        newDirNameBuf_ = std::make_unique<
                                std::array<char, INPUT_NAME_BUF_SIZE>>();
        *newDirNameBuf_ = *copyFrom.newDirNameBuf_;
    }

    return *this;
}

inline void Engine::UI::FileBrowser::SetWindowSize(int width, int height) noexcept
{
    assert(width > 0 && height > 0);
    width_  = width;
    height_ = height;
}

inline void Engine::UI::FileBrowser::SetTitle(std::string title)
{
    title_ = std::move(title);
    openLabel_ = title_ + "##filebrowser_" +
                 std::to_string(reinterpret_cast<size_t>(this));
    openNewDirLabel_ = "new dir##new_dir_" +
                       std::to_string(reinterpret_cast<size_t>(this));
}

inline void Engine::UI::FileBrowser::Open()
{
    ClearSelected();
    statusStr_ = std::string();
    openFlag_ = true;
    closeFlag_ = false;
}

inline void Engine::UI::FileBrowser::Close()
{
    ClearSelected();
    statusStr_ = std::string();
    closeFlag_ = true;
    openFlag_ = false;
}

inline bool Engine::UI::FileBrowser::IsOpened() const noexcept
{
    return isOpened_;
}

inline void Engine::UI::FileBrowser::Display()
{
    ImGui::PushID(this);
    ScopeGuard exitThis([this]
    {
        openFlag_ = false;
        closeFlag_ = false;
        ImGui::PopID();
    });

    if(openFlag_)
        ImGui::OpenPopup(openLabel_.c_str());
    isOpened_ = false;

    // open the popup window

    if(openFlag_ && (flags_ & ImGuiFileBrowserFlags_NoModal))
    {
        ImGui::SetNextWindowSize(
            ImVec2(static_cast<float>(width_), static_cast<float>(height_)));
    }
    else
    {
        ImGui::SetNextWindowSize(
            ImVec2(static_cast<float>(width_), static_cast<float>(height_)),
            ImGuiCond_FirstUseEver);
    }
    if(flags_ & ImGuiFileBrowserFlags_NoModal)
    {
        if(!ImGui::BeginPopup(openLabel_.c_str()))
            return;
    }
    else if(!ImGui::BeginPopupModal(openLabel_.c_str(), nullptr,
                                    flags_ & ImGuiFileBrowserFlags_NoTitleBar ?
                                        ImGuiWindowFlags_NoTitleBar : 0))
    {
        return;
    }
    isOpened_ = true;
    ScopeGuard endPopup([] { ImGui::EndPopup(); });

    // display elements in pwd

#ifdef _WIN32
    char currentDrive = static_cast<char>(pwd_.c_str()[0]);
    char driveStr[] = { currentDrive, ':', '\0' };

    ImGui::PushItemWidth(4 * ImGui::GetFontSize());
    if(ImGui::BeginCombo("##select_drive", driveStr))
    {
        ScopeGuard guard([&] { ImGui::EndCombo(); });
        for(int i = 0; i < 26; ++i)
        {
            if(!(drives_ & (1 << i)))
                continue;
            char driveCh = static_cast<char>('A' + i);
            char selectableStr[] = { driveCh, ':', '\0' };
            bool selected = currentDrive == driveCh;
            if(ImGui::Selectable(selectableStr, selected) && !selected)
            {
                char newPwd[] = { driveCh, ':', '\\', '\0' };
                SetPwd(newPwd);
            }
        }
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
#endif

    int secIdx = 0, newPwdLastSecIdx = -1;
    for(auto &sec : pwd_)
    {
#ifdef _WIN32
        if(secIdx == 1)
        {
            ++secIdx;
            continue;
        }
#endif
        ImGui::PushID(secIdx);
        if(secIdx > 0)
            ImGui::SameLine();
        if(ImGui::SmallButton(sec.u8string().c_str()))
            newPwdLastSecIdx = secIdx;
        ImGui::PopID();
        ++secIdx;
    }

    if(newPwdLastSecIdx >= 0)
    {
        int i = 0;
        std::filesystem::path newPwd;
        for(auto &sec : pwd_)
        {
            if(i++ > newPwdLastSecIdx)
                break;
            newPwd /= sec;
        }
#ifdef _WIN32
        if(newPwdLastSecIdx == 0)
            newPwd /= "\\";
#endif
        SetPwd(newPwd);
    }

    ImGui::SameLine();

    if(ImGui::SmallButton("*"))
        SetPwd(pwd_);

    if(newDirNameBuf_)
    {
        ImGui::SameLine();
        if(ImGui::SmallButton("+"))
        {
            ImGui::OpenPopup(openNewDirLabel_.c_str());
            (*newDirNameBuf_)[0] = '\0';
        }

        if(ImGui::BeginPopup(openNewDirLabel_.c_str()))
        {
            ScopeGuard endNewDirPopup([] { ImGui::EndPopup(); });

            ImGui::InputText("name", newDirNameBuf_->data(), newDirNameBuf_->size());
            ImGui::SameLine();

            if(ImGui::Button("ok") && (*newDirNameBuf_)[0] != '\0')
            {
                ScopeGuard closeNewDirPopup([] { ImGui::CloseCurrentPopup(); });
                if(create_directory(pwd_ / newDirNameBuf_->data()))
                    SetPwd(pwd_);
                else
                {
                    statusStr_ = "failed to create " +
                                 std::string(newDirNameBuf_->data());
                }
            }
        }
    }

    // browse files in a child window

    float reserveHeight = ImGui::GetFrameHeightWithSpacing();
    std::filesystem::path newPwd; bool setNewPwd = false;
    if(!(flags_ & ImGuiFileBrowserFlags_SelectDirectory) &&
       (flags_ & ImGuiFileBrowserFlags_EnterNewFilename))
        reserveHeight += ImGui::GetFrameHeightWithSpacing();
    {
        ImGui::BeginChild("ch", ImVec2(0, -reserveHeight), true,
                          (flags_ & ImGuiFileBrowserFlags_NoModal) ?
                              ImGuiWindowFlags_AlwaysHorizontalScrollbar : 0);
        ScopeGuard endChild([] { ImGui::EndChild(); });

        for(auto &rsc : fileRecords_)
        {
            if (!rsc.isDir && typeFilters_.size() > 0 &&
                static_cast<size_t>(typeFilterIndex_) < typeFilters_.size() &&
                !(rsc.extension == typeFilters_[typeFilterIndex_]))
                continue;

            if(!rsc.name.empty() && rsc.name.c_str()[0] == '$')
                continue;

            const bool selected = selectedFilename_ == rsc.name;
            if(ImGui::Selectable(rsc.showName.c_str(), selected,
                                 ImGuiSelectableFlags_DontClosePopups))
            {
                if(selected)
                {
                    selectedFilename_ = std::filesystem::path();
                    (*inputNameBuf_)[0] = '\0';
                }
                else if(rsc.name != "..")
                {
                    if((rsc.isDir && (flags_ & ImGuiFileBrowserFlags_SelectDirectory)) ||
                       (!rsc.isDir && !(flags_ & ImGuiFileBrowserFlags_SelectDirectory)))
                    {
                        selectedFilename_ = rsc.name;
                        if(!(flags_ & ImGuiFileBrowserFlags_SelectDirectory))
                        {
#ifdef _MSC_VER
                            strcpy_s(inputNameBuf_->data(), inputNameBuf_->size(),
                                     selectedFilename_.u8string().c_str());
#else
                            std::strncpy(inputNameBuf_->data(), selectedFilename_.u8string().c_str(),
                                         inputNameBuf_->size());
#endif
                        }
                    }
                }
            }

            if(ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0) && rsc.isDir)
            {
                setNewPwd = true;
                newPwd = (rsc.name != "..") ? (pwd_ / rsc.name) :
                                               pwd_.parent_path();
            }
        }
    }

    if(setNewPwd)
        SetPwd(newPwd);

    if(!(flags_ & ImGuiFileBrowserFlags_SelectDirectory) &&
       (flags_ & ImGuiFileBrowserFlags_EnterNewFilename))
    {
        ImGui::PushID(this);
        ScopeGuard popTextID([] { ImGui::PopID(); });

        ImGui::PushItemWidth(-1);
        if(ImGui::InputText("", inputNameBuf_->data(), inputNameBuf_->size()))
            selectedFilename_ = inputNameBuf_->data();
        ImGui::PopItemWidth();
    }

    if(!(flags_ & ImGuiFileBrowserFlags_SelectDirectory))
    {
        if(ImGui::Button(" ok ") && !selectedFilename_.empty())
        {
            ok_ = true;
            ImGui::CloseCurrentPopup();
        }
    }
    else
    {
        if(selectedFilename_.empty())
        {
            if(ImGui::Button(" ok "))
            {
                ok_ = true;
                ImGui::CloseCurrentPopup();
            }
        }
        else if(ImGui::Button("open"))
            SetPwd(pwd_ / selectedFilename_);
    }

    ImGui::SameLine();

    int escIdx = ImGui::GetIO().KeyMap[ImGuiKey_Escape];
    if(ImGui::Button("cancel") || closeFlag_ ||
        ((flags_ & ImGuiFileBrowserFlags_CloseOnEsc) &&
            ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
         escIdx >= 0 && ImGui::IsKeyPressed(escIdx)))
        ImGui::CloseCurrentPopup();

    if(!statusStr_.empty() && !(flags_ & ImGuiFileBrowserFlags_NoStatusBar))
    {
        ImGui::SameLine();
        ImGui::Text("%s", statusStr_.c_str());
    }

    if (!typeFilters_.empty())
    {
        ImGui::SameLine();
        ImGui::PushItemWidth(8 * ImGui::GetFontSize());
        ImGui::Combo("##type_filters", &typeFilterIndex_,
                     typeFilters_.data(), int(typeFilters_.size()));
        ImGui::PopItemWidth();
    }
}

inline bool Engine::UI::FileBrowser::HasSelected() const noexcept
{
    return ok_;
}

inline bool Engine::UI::FileBrowser::SetPwd(const std::filesystem::path &pwd)
{
    try
    {
        SetPwdUncatched(pwd);
        return true;
    }
    catch(const std::exception &err)
    {
        statusStr_ = std::string("last error: ") + err.what();
    }
    catch(...)
    {
        statusStr_ = "last error: unknown";
    }

    SetPwdUncatched(std::filesystem::current_path());
    return false;
}

inline std::filesystem::path Engine::UI::FileBrowser::GetSelected() const
{
    return pwd_ / selectedFilename_;
}

inline void Engine::UI::FileBrowser::ClearSelected()
{
    selectedFilename_ = std::string();
    (*inputNameBuf_)[0] = '\0';
    ok_ = false;
}

inline void Engine::UI::FileBrowser::SetTypeFilters(
    const std::vector<const char*> &typeFilters)
{
    typeFilters_ = typeFilters;
    typeFilterIndex_ = 0;
}

inline void Engine::UI::FileBrowser::SetPwdUncatched(const std::filesystem::path &pwd)
{
    fileRecords_ = { FileRecord{ true, "..", "[D] ..", "" } };

    for(auto &p : std::filesystem::directory_iterator(pwd))
    {
        FileRecord rcd;

        if(p.is_regular_file())
            rcd.isDir = false;
        else if(p.is_directory())
            rcd.isDir = true;
        else
            continue;

        rcd.name = p.path().filename();
        if(rcd.name.empty())
            continue;

        rcd.extension = p.path().filename().extension();

        rcd.showName = (rcd.isDir ? "[D] " : "[F] ") +
                        p.path().filename().u8string();
        fileRecords_.push_back(rcd);
    }

    std::sort(fileRecords_.begin(), fileRecords_.end(),
        [](const FileRecord &L, const FileRecord &R)
    {
        return (L.isDir ^ R.isDir) ? L.isDir : (L.name < R.name);
    });

    pwd_ = absolute(pwd);
    selectedFilename_ = std::string();
    (*inputNameBuf_)[0] = '\0';
}

#ifdef _WIN32

#ifndef _INC_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN

#define IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif // #ifndef WIN32_LEAN_AND_MEAN

#include <Windows.h>

#ifdef IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN
#undef IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif // #ifdef IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN

#endif // #ifdef _INC_WINDOWS

inline std::uint32_t Engine::UI::FileBrowser::GetDrivesBitMask()
{
    DWORD mask = GetLogicalDrives();
    uint32_t ret = 0;
    for(int i = 0; i < 26; ++i)
    {
        if(!(mask & (1 << i)))
            continue;
        char rootName[4] = { static_cast<char>('A' + i), ':', '\\', '\0' };
        UINT type = GetDriveTypeA(rootName);
        if(type == DRIVE_REMOVABLE || type == DRIVE_FIXED)
            ret |= (1 << i);
    }
    return ret;
}

#endif
