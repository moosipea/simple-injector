#include "Injector.h"

DWORD Injector::GetProcessIdFromExecutable(const char* executableName)
{
    PROCESSENTRY32 entry;
    HANDLE snapshot;

    entry.dwSize = sizeof(PROCESSENTRY32);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry))
    {
        while (Process32Next(snapshot, &entry))
        {
            if (strcmp((const char*)entry.szExeFile, executableName) == 0)
                break;
        }
    }
    CloseHandle(snapshot);
    return entry.th32ProcessID;
}

DWORD Injector::GetProcessIdFromTitle(const char* windowTitle)
{
    DWORD id;
    GetWindowThreadProcessId(FindWindow(NULL, (LPCSTR)windowTitle), &id);
    return id;
}

bool Injector::Inject(DWORD pid, const char* dll)
{
    if (!l_FileExists(dll)) { l_Error("Failed to find DLL file"); }

    char dllPath[MAX_PATH];
    if (!GetFullPathName(dll, MAX_PATH, dllPath, nullptr)) { l_Error("Failed to get full path of DLL file"); }

    HANDLE h_proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!h_proc) { l_Error("Failed to open process"); }

    void* allocatedMemory = VirtualAllocEx(h_proc, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!allocatedMemory) { l_Error("Failed to allocate memory"); }

    if (!WriteProcessMemory(h_proc, allocatedMemory, dllPath, MAX_PATH, nullptr)) { l_Error("Failed to write process memory"); }

    HANDLE h_thread = CreateRemoteThread(h_proc, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocatedMemory, NULL, nullptr);
    if (!h_thread) { l_Error("Failed to create remote thread"); }

    CloseHandle(h_thread);
    CloseHandle(h_proc);
    VirtualFreeEx(h_proc, allocatedMemory, NULL, MEM_RELEASE);

    return true;
}

bool Injector::l_FileExists(const char* path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

void Injector::l_Error(const char* error)
{
#ifdef MESSAGEBOX_ERRORS
    MessageBox(NULL, error, "Error", MB_ICONERROR);
#else
    fprintf(stderr, "ERROR: %s\n", error);
#endif// MESSAGEBOX_ERRORS
    exit(EXIT_FAILURE);
}
