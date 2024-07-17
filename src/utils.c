#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

void tsclicker_utils_formatbooleanstring(char* string, bool value, char* trueopt, char* falseopt) {
    if (value) {
        strcat(string, trueopt);
    } else {
        strcat(string, falseopt);
    }
}

void tsclicker_utils_getlocalfilepath(char* filepath, const char* filename) {
    char currentfilepath[MAX_PATH];
    HMODULE hm = NULL;

    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) &tsclicker_utils_getlocalfilepath, &hm) == 0) {
        return;
    }
    int currentpathlength = GetModuleFileName(hm, currentfilepath, sizeof(currentfilepath));
    if (currentpathlength == 0) {
        return;
    }

    char currentpath[MAX_PATH] = "";
    strncpy(currentpath, currentfilepath, currentpathlength-13);

    strcpy(filepath, currentpath);
    strcat(filepath, filename);
}