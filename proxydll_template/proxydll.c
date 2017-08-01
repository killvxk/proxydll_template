#include <Windows.h>
#include <tchar.h>

#include "proxydll.h"

static HMODULE hmod = NULL;

static FARPROC export_procs[EXPORT_COUNT] = { 0 };

static LPCSTR export_names[] = { EXPORT_NAMES };

BOOL proxy_dll_init(BOOL b_resolve_all_procs) {
    if (!hmod) {
        TCHAR path[MAX_PATH];
        GetSystemDirectory(path, _countof(path));
        _tcscat_s(path, _countof(path), _T(DLL_FNAME));
        hmod = LoadLibrary(path);
    }
    if (b_resolve_all_procs) {
        for (SIZE_T i = 0; i < EXPORT_COUNT; i++)
            resolve_export_proc(i);
    }
    return (hmod != NULL);
}

FARPROC resolve_export_proc(SIZE_T index) {
    if (index > EXPORT_COUNT)
        return NULL;

    if (hmod && export_procs[index])
        return export_procs[index];

    if (proxy_dll_init(FALSE))
        return export_procs[index] = GetProcAddress(hmod, export_names[index]);
    
    return NULL;
}

BOOL proxy_dll_free(void) {
    if (!hmod)
        return FALSE;

    BOOL result = FreeLibrary(hmod);
    if (result)
        hmod = NULL;

    return result;
}
