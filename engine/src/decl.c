#include "decl.h"
#include "helium.h"
#include <string.h>

#ifdef IS_WINDOWS
double __TimeDouble() {
    double unixTime = 0.0;

    FILETIME ft;
    ULARGE_INTEGER li;

    GetSystemTimeAsFileTime(&ft);

    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    unixTime = li.QuadPart / 1.0e7 - 11644473600.0;
    return unixTime;
}

char *GetExecutablePath() {
    char temp[1024];
    GetModuleFileName(NULL, temp, sizeof(temp));
    return strdup(temp);
}
#else
double __TimeDouble() {
    double unixTime = 0.0;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    unixTime = tv.tv_sec + tv.tv_usec / 1.0e6;

    return unixTime;
}

char *GetExecutablePath() {
    char temp[1024];
    ssize_t len = readlink("/proc/self/exe", temp, sizeof(temp) - 1);
    if (len != -1) {
        temp[len] = '\0';
    }
    return strdup(temp);
}
#endif

float Time() {
    return (float)(__TimeDouble() - Engine.startTime);
}

char *ResolvePath(char *path) {
    if (path == NULL || strlen(path) == 0) {
        return NULL;
    }

    char *resolved = (char *)malloc(strlen(path) + 1);
    if (resolved == NULL) {
        Crash("Memory allocation failed for resolved path.\n");
        return NULL;
    }
    resolved[0] = '\0';

    int is_absolute = (path[0] == '/' || path[0] == '\\');

    const char *token;
    const char *delimiters = "\\/";
    char *temp = strdup(path);

    token = strtok(temp, delimiters);

    while (token != NULL) {
        if (strcmp(token, "..") == 0) {
            char *last_delim = strrchr(resolved, '/');
            if (last_delim != NULL) {
                *last_delim = '\0';
            } else {
                last_delim = strrchr(resolved, '\\');
                if (last_delim != NULL) {
                    *last_delim = '\0';
                }
            }
        } else if (strcmp(token, ".") != 0) {
            if (strlen(resolved) > 0) {
                char lastResolved = resolved[strlen(resolved) - 1];
                if (lastResolved != '/' && lastResolved != '\\') {
                    strcat(resolved, "/");
                }
            }
            strcat(resolved, token);
        }

        token = strtok(NULL, delimiters);
    }

    free(temp);

    if (is_absolute) {
        memmove(resolved + 1, resolved, strlen(resolved) + 1);
        resolved[0] = '/';
    }

    return resolved;
}