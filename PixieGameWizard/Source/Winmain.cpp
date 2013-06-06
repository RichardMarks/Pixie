// Winmain.cpp
// Pixie GameWizard
// Designed and Developed by Richard Marks
// © Copyright 2013, Bang Bang Attack Studios
// http://www.bangbangattackstudios.com

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <Shlobj.h>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Resource.h"
#include "tinyxml2.h"

namespace localcode
{
    struct CodeFile
    {
        std::string name;
        std::string content;
    };

    class Writer
    {
    public:
        static bool Create(CodeFile& code)
        {
            std::ofstream outFile;
            outFile.open(code.name.c_str());
            if (!outFile.is_open())
            {
                return false;
            }
            outFile << code.content.c_str();
            outFile.close();
            return true;
        }
    };
};

namespace localhelper
{
    class InvalidIdentifierCharacters
    {
    public:
        bool operator()(char c)
        {
            static const char* invalid = ";()-+!@#%^&*'\":{}[]\\|/?<>,.~` ";
            for (const char* t = invalid; *t != 0; ++t)
            {
                if (*t == c)
                {
                    return true;
                }
            }
            return false;
        }
    };
};

BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

bool AddGameStateToStateList(HWND hwnd);
bool DeleteGameStateFromStateList(HWND hwnd);
void ClearStateList(HWND hwnd);
bool OpenProject(HWND hwnd);
bool SaveProject(HWND hwnd);
void GenerateCode(HWND hwnd);

std::string CreateValidClassName(std::string& stateName);
std::string GenerateGameStateHeader(std::string& stateName);
std::string GenerateGameStateSource(std::string& stateName);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
    
    HWND dialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), 0, DialogProc);

    if (!dialog)
    {
        return 1;
    }

    MSG msg;
    int status;
    while((status = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (status == -1)
        {
            return -1;
        }

        if (!IsDialogMessage(dialog, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

bool AddGameStateToStateList(HWND hwnd)
{
    static const unsigned MAX_EDIT_LENGTH = 129;
    char nameBuffer[MAX_EDIT_LENGTH];
    char prefixBuffer[MAX_EDIT_LENGTH];
    char stateCodeNameBuffer[MAX_EDIT_LENGTH * 2];

    HWND nameEditHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_NAME_EDIT);
    int count = Edit_GetTextLength(nameEditHwnd);
    int copied = Edit_GetText(nameEditHwnd, nameBuffer, count + 1);
    
    if (!strlen(nameBuffer))
    {
        MessageBox(0, "GameState Not Added", "Missing Required State Name Data", MB_OK | MB_ICONERROR); 
        return false;
    }

    HWND prefixEditHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_PREFIX_EDIT);
    count = Edit_GetTextLength(prefixEditHwnd);
    copied = Edit_GetText(prefixEditHwnd, prefixBuffer, count + 1);
    
    bool hasPrefix = strlen(prefixBuffer) > 0;

    if (hasPrefix)
    {
        sprintf_s(stateCodeNameBuffer,(MAX_EDIT_LENGTH * 2) - 1, "%s%s", prefixBuffer, nameBuffer);
    }
    else
    {
        sprintf_s(stateCodeNameBuffer,(MAX_EDIT_LENGTH * 2) - 1, "%s", nameBuffer);
    }

    HWND stateListHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_LIST);
    ListBox_AddString(stateListHwnd, stateCodeNameBuffer);

    SendMessage(nameEditHwnd, WM_SETTEXT, 0, (LPARAM)"");

    return true;
}

bool DeleteGameStateFromStateList(HWND hwnd)
{
    HWND stateListHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_LIST);
    int selectedItemIndex = ListBox_GetCurSel(stateListHwnd);

    if (LB_ERR == selectedItemIndex)
    {
        return false;
    }

    if (IDNO == MessageBox(hwnd, "Delete Selected Item?", "Are you sure?", MB_YESNO | MB_ICONQUESTION))
    {
        return false;
    }

    ListBox_DeleteString(stateListHwnd, selectedItemIndex);
    return true;
}

void ClearStateList(HWND hwnd)
{
    HWND stateListHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_LIST);
    int count = 0;
    while ((count = ListBox_GetCount(stateListHwnd)) > 0)
    {
        ListBox_DeleteString(stateListHwnd, 0);
    }
}

bool OpenProject(HWND hwnd)
{
    char fileName[4096];

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (FALSE == GetOpenFileName(&ofn))
    {
        return false;
    }


    tinyxml2::XMLDocument doc;

    if (tinyxml2::XML_NO_ERROR != doc.LoadFile(ofn.lpstrFile))
    {
        MessageBox(0, "Invalid GameWizard Project File\n\nNothing Loaded.", "User Error", MB_OK | MB_ICONERROR);  
        return false;
    }

    ClearStateList(hwnd);

    tinyxml2::XMLElement* rootNode = doc.FirstChildElement("GameWizardProject");

    for (
        tinyxml2::XMLElement* stateNode = rootNode->FirstChildElement("GameState");
        stateNode != NULL;
        stateNode = stateNode->NextSiblingElement("GameState"))
    {
        const char* stateName = stateNode->Attribute("Name");

        HWND stateListHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_LIST);
        ListBox_AddString(stateListHwnd, stateName);
    }
    
    return true;
}

bool SaveProject(HWND hwnd)
{
    HWND stateListHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_LIST);
    int stateCount = ListBox_GetCount(stateListHwnd);
    if (stateCount == 0)
    {
        MessageBox(0, "GameState List is Empty.\n\nNothing to Save.", "User Error", MB_OK | MB_ICONERROR);  
        return false;
    }

    char fileName[4096];

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (FALSE == GetSaveFileName(&ofn))
    {
        return false;
    }

    tinyxml2::XMLPrinter printer;

    printer.OpenElement("GameWizardProject");
    for (int i = 0; i < stateCount; i++)
    {
        char buffer[512];
        ListBox_GetText(stateListHwnd, i, buffer);
        printer.OpenElement("GameState");
        printer.PushAttribute("Name", buffer);
        printer.CloseElement();
    }
    printer.CloseElement();
    tinyxml2::XMLDocument doc;
    int result = doc.Parse(printer.CStr());
    if (result != tinyxml2::XML_NO_ERROR)
    {
        return false;
    }
    doc.SaveFile(ofn.lpstrFile);
    return true;
}

void GenerateCode(HWND hwnd)
{
    HWND stateListHwnd = GetDlgItem(hwnd, IDC_GAMESTATE_LIST);
    int stateCount = ListBox_GetCount(stateListHwnd);
    if (stateCount == 0)
    {
        MessageBox(0, "GameState List is Empty.\n\nNothing to Generate.", "User Error", MB_OK | MB_ICONERROR);  
        return;
    }

    char destFolderBuffer[MAX_PATH];
    BROWSEINFO folderBrowseInfo;
    ZeroMemory(&folderBrowseInfo, sizeof(BROWSEINFO));

    folderBrowseInfo.hwndOwner = hwnd;
    folderBrowseInfo.pidlRoot = NULL;
    folderBrowseInfo.pszDisplayName = destFolderBuffer;
    folderBrowseInfo.lpszTitle = "Select Folder to Save Generated Code Files";
    folderBrowseInfo.ulFlags = BIF_USENEWUI;
    folderBrowseInfo.lpfn = NULL;
    
    PIDLIST_ABSOLUTE pidl = SHBrowseForFolder(&folderBrowseInfo);
    if (pidl == NULL)
    {
        MessageBox(0, "Code Generation Canceled by User.", "Code Generation Canceled", MB_OK | MB_ICONINFORMATION);  
        return;
    }

    if (SHGetPathFromIDList(pidl, destFolderBuffer) == FALSE)
    {
        CoTaskMemFree(pidl);
        MessageBox(0, "Unable to Access Destination Folder", "Code Generation Interrupted", MB_OK | MB_ICONERROR);  
        return;
    }

    CoTaskMemFree(pidl);

    std::vector<std::string> classNames;
    for (int i = 0; i < stateCount; i++)
    {
        char buffer[512];
        ListBox_GetText(stateListHwnd, i, buffer);
        std::string stateName = buffer;
        classNames.push_back(CreateValidClassName(stateName));
    }

    std::vector<localcode::CodeFile> codeFiles;
    std::vector<std::string>::iterator nameIter;
    for (nameIter = classNames.begin(); nameIter != classNames.end(); nameIter++)
    {
        std::string className = *nameIter;

        localcode::CodeFile header;
        localcode::CodeFile source;
        
        header.content = GenerateGameStateHeader(className);
        header.name = std::string(destFolderBuffer) + std::string("\\") + className + std::string(".h");
        
        source.content = GenerateGameStateSource(className);
        source.name = std::string(destFolderBuffer) + std::string("\\") + className + std::string(".cpp");

        codeFiles.push_back(header);
        codeFiles.push_back(source);
    }

    std::vector<std::string> failures;
    std::vector<localcode::CodeFile>::iterator codeIter;    
    for (codeIter = codeFiles.begin(); codeIter != codeFiles.end(); codeIter++)
    {
        if (!localcode::Writer::Create(*codeIter))
        {
            failures.push_back((*codeIter).name);
        }
    }

    if (failures.size() > 0)
    {
        std::stringstream failStream;
        failStream << "The following files could not be generated:" << std::endl << std::endl;
        unsigned count = (unsigned)min(20, failures.size());

        for (unsigned i = 0; i < count; i++)
        {
            failStream << failures.at(i) << std::endl;
        }

        if (count < failures.size())
        {
            failStream << std::endl << "(" << failures.size() - count << ") failures not displayed." << std::endl;
        }

        MessageBox(0, failStream.str().c_str(), "There were some failures...", MB_OK | MB_ICONWARNING);
    }

    std::stringstream resultStream;
    resultStream << "Generated " << codeFiles.size() << " files in " << destFolderBuffer;
    MessageBox(0, resultStream.str().c_str(), "Code Generation Complete", MB_OK | MB_ICONINFORMATION);

}

std::string CreateValidClassName(std::string& stateName)
{
    std::string className = stateName + "State";

    className.erase(std::remove_if(className.begin(), className.end(), localhelper::InvalidIdentifierCharacters()), className.end());

    if (className[0] >= 'a' && className[0] <= 'z')
    {
        className[0] -= 'a' - 'A';
    }

    return className;
}

std::string GenerateGameStateHeader(std::string& stateName)
{
    std::stringstream codeStream;

    codeStream
        << "// " << stateName << ".h" << std::endl
        << "// Generated by GameWizard 1.0 | www.bangbangattackstudios.com" << std::endl
        << "#ifndef __" << stateName << "_H__" << std::endl
        << "#define __" << stateName << "_H__" << std::endl << std::endl
        << "#include \"Pixie.h\"" << std::endl << std::endl
        << "class " << stateName << " : public pixie::GameState, public pixie::SpriteEventListener" << std::endl
        << "{" << std::endl
        << "public:" << std::endl
        << "    static const char* ID;" << std::endl
        << "    static void Register(pixie::GameStateManager&);" << std::endl << std::endl
        << "private:" << std::endl
        << "    static pixie::GameState* CreateInstance();" << std::endl << std::endl
        << "    " << stateName << "();" << std::endl
        << "    virtual void Update(float deltaTime);" << std::endl
        << "    virtual void OnSpriteEvent(pixie::StringId eventId, void* userData);" << std::endl << std::endl
        << "};" << std::endl << std::endl
        << "#endif // __" << stateName << "_H__" << std::endl;

    return codeStream.str();
}

std::string GenerateGameStateSource(std::string& stateName)
{
    std::stringstream codeStream;

    codeStream
        << "// " << stateName << ".cpp" << std::endl
        << "// Generated by GameWizard 1.0 | www.bangbangattackstudios.com" << std::endl
        << "#include \"" << stateName << ".h\"" << std::endl << std::endl
        << "const char* " << stateName << "::ID = \"" << stateName << "\";" << std::endl << std::endl
        << "void " << stateName << "::Register(pixie::GameStateManager& gameStateManager)" << std::endl
        << "{" << std::endl
        << "    gameStateManager.RegisterGameState(" << stateName << "::ID, " << stateName << "::CreateInstance);" << std::endl
        << "}" << std::endl << std::endl
        << "pixie::GameState* " << stateName << "::CreateInstance()" << std::endl
        << "{" << std::endl
        << "    return new " << stateName << ";" << std::endl << std::endl
        << stateName << "::" << stateName << "()" << std::endl
        << "{" << std::endl
        << "}" << std::endl << std::endl
        << "void " << stateName << "::Update(float deltaTime)" << std::endl
        << "{" << std::endl
        << "}" << std::endl << std::endl
        << "void " << stateName << "::OnSpriteEvent(pixie::StringId eventId, void* userData)" << std::endl
        << "{" << std::endl
        << "}" << std::endl << std::endl;

    return codeStream.str();
}

BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch(message)
    {
    case WM_INITDIALOG:
        {
            CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
            HICON icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_MAIN_ICON));
            SendMessage(hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon);
            return TRUE;
        } break;

    case WM_COMMAND:
        {
            if (0 == HIWORD(wParam))
            {
                switch(LOWORD(wParam))
                {
                case IDM_FILE_NEW: 
                    { 
                        ClearStateList(hwnd);
                    } break;

                case IDM_FILE_OPEN: 
                    { 
                        OpenProject(hwnd);
                    } break;

                case IDM_FILE_SAVE: 
                    { 
                        SaveProject(hwnd);
                    } break;

                case IDM_FILE_EXIT: 
                    { 
                        SendMessage(hwnd, WM_CLOSE, 0, 0); 
                    } break;

                case IDM_BUILD_GENERATE_CODE: 
                    { 
                        GenerateCode(hwnd);
                    } break;

                case IDM_HELP_ABOUT: 
                    { 
                        MessageBox(0, 
                            "Pixie GameWizard 1.0\n\n"
                            "Designed and Developed by\n"
                            "Richard Marks\n\n"
                            "© Copyright 2013, Bang Bang Attack Studios\n"
                            "http://www.bangbangattackstudios.com\n\n"
                            "The version of Pixie this tool was designed for may be found at:\n"
                            "https://github.com/RichardMarks/Pixie\n\n"
                            "Pixie is a public domain 2D game engine for C++, created by Mattias Gustavsson\n"
                            "You can read more about Pixie at:\n"
                            "http://www.pixieuniversity.com", "About", MB_OK); 
                    } break;

                case IDC_ADD_GAMESTATE_BUTTON: 
                    { 
                        AddGameStateToStateList(hwnd);
                    } break;

                case IDC_DELETE_GAMESTATE_BUTTON: 
                    { 
                        DeleteGameStateFromStateList(hwnd);
                    } break;
                }
            }
            return TRUE;
        } break;

    case WM_CLOSE:
        {
            if (IDYES == MessageBox(hwnd, "Really Quit?", "Application Closing", MB_YESNO | MB_ICONQUESTION))
            {
                DestroyWindow(hwnd);
            }
            return TRUE;
        } break;

    case WM_DESTROY:
        {
            CoUninitialize();
            PostQuitMessage(0);
            return TRUE;
        } break;
    }
    return FALSE;
}