#include "resource.h"
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <string.h>

int limit = 300;
int delay = 250;

int folder_exists(WCHAR *fullPath) {
    WCHAR longPath[MAX_PATH * 4];

    // Add \\?\ prefix if not already present
    if (wcsncmp(fullPath, L"\\\\?\\", 4) != 0) {
        swprintf(longPath, MAX_PATH * 4, L"\\\\?\\%ls", fullPath);
    } else {
        swprintf(longPath, MAX_PATH * 4, L"%ls", fullPath);
    }

    DWORD attrs = GetFileAttributesW(longPath);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return 0; // Folder does not exist
    }
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int create_amogus_file(WCHAR *path) {
    WCHAR full_path[MAX_PATH];
    swprintf(full_path, MAX_PATH, L"%ls\\amogus.txt", path);

    HANDLE hFile = CreateFileW(
        full_path,               // use full path here
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        wprintf(L"Error creating file: %lu\n", GetLastError());
        return 1;
    }

    const char *data = "Find the imposta!";
    DWORD bytesWritten;
    WriteFile(hFile, data, (DWORD)strlen(data), &bytesWritten, NULL);

    CloseHandle(hFile);
    return 0;
}

int delete_amogus_file(WCHAR *folderPath) {
    WCHAR fullPath[MAX_PATH * 4]; // Extended buffer for \\?\ prefix
    swprintf(fullPath, MAX_PATH, L"%ls\\amogus.txt", folderPath);
    if (DeleteFileW(fullPath)) {
        wprintf(L"Deleted: %ls\n", fullPath);
        return 0; // Success
    } else {
        MessageBoxA(NULL, "Could not find Desktop path.", "Error", MB_ICONERROR);
        exit(1); // Failure
    }
}

void create_amogus_folders(int num, WCHAR *path) {
    for (int i = 0; i < num; i++) {
        WCHAR folder_path[MAX_PATH] = L"";
        swprintf(folder_path, MAX_PATH, L"\\\\?\\%ls\\AMOGUS_%d.", path, i + 1);
        if (folder_exists(folder_path))   continue;
        CreateDirectoryW(folder_path, NULL);
        create_amogus_file(folder_path);
        Sleep(delay);
    }
}

void delete_amogus_folders(WCHAR *path) {
    for (int i = 0; i < limit; i++) {
            WCHAR folder_path[MAX_PATH] = L"";
            swprintf(folder_path, MAX_PATH, L"\\\\?\\%ls\\AMOGUS_%d.", path, i + 1);
            if (!folder_exists(folder_path))  continue;
            delete_amogus_file(folder_path);
            RemoveDirectoryW(folder_path);
            Sleep(delay);
    }
}

int extra_mock(WCHAR *path) {
    WCHAR full_path[MAX_PATH] = L"";
    swprintf(full_path, MAX_PATH, L"%ls\\Delete-Desktop-Folders.txt", path);
    HANDLE hFile = CreateFileW(
        full_path, // extended path
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        wprintf(L"Error creating file: %lu\n", GetLastError());
        return 1;
    }
    char text[1000] = "Bro\nYou're totally fked\n";
    for (int i = 0; i < 200; i++){
        strncat(text, "HA\n", sizeof(text) - strlen(text) - 1);
    }
    const char *data = text;
    DWORD bytesWritten;
    WriteFile(hFile, data, (DWORD)strlen(data), &bytesWritten, NULL);

    CloseHandle(hFile);
    return 0;
}

// --- Input dialog with custom window proc ---
typedef struct {
    int result;
} DialogData;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG:
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    char buffer[32];
                    GetDlgItemText(hwndDlg, IDC_EDIT_NUMBER, buffer, sizeof(buffer));
                    int val = atoi(buffer);

                    if ((val >= 10 && val <= limit) || val == -143) {
                        EndDialog(hwndDlg, val);
                    } else {
                        char msg[200];
                        sprintf(msg, "Please enter a positive number between 10 and %d.", limit);
                        MessageBox(hwndDlg, msg, "Invalid Input", MB_ICONWARNING);
                        SetDlgItemText(hwndDlg, IDC_EDIT_NUMBER, "");
                    }
                    return TRUE;
                }
                case IDCANCEL:
                    EndDialog(hwndDlg, IDCANCEL);
                    return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    int lucky = DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, DialogProc);
    if (lucky == -1) {
        MessageBox(NULL, "Dialog failed!", "Error", MB_ICONERROR);
        return 1;
    }

    WCHAR desktop[MAX_PATH];
    if (SHGetFolderPathW(NULL, CSIDL_DESKTOP, NULL, 0, desktop) != S_OK) {
        MessageBoxA(NULL, "Could not find Desktop path.", "Error", MB_ICONERROR);
        return 1;
    }

    if (lucky == IDCANCEL) return 0;

    if (lucky == -143) {
        delete_amogus_folders(desktop);
        MessageBoxA(NULL, "Deleted cursed folders.\nShare with your friends and see them suffer like you did.\n", "Done", MB_OK | MB_ICONINFORMATION);
    } else if (lucky > 0) {
        char msg[500] = "";
        sprintf(msg, "Good luck deleting those %d folders on Desktop!\nYou're welcome.", lucky);
        MessageBoxA(NULL, msg, "Done", MB_OK | MB_ICONINFORMATION);
        create_amogus_folders(lucky, desktop);
        WCHAR current_dir[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, current_dir);
        extra_mock(current_dir);
    }
    return 0;
}

