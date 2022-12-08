#pragma once
#ifndef firstTime
#define firstTime

#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#define CURL_STATICLIB
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"

#define CHUNK 10

#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")


struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t

WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char* readLine(FILE* read);
FILE* openFile(FILE* filePointer, char* readingOrWriting);
char* returnAPI(char* city);
char* serverRequest(char* city);
void printToFile(FILE* filePointerReading, FILE* filePointerWriting);


#endif // !1