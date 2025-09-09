#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

File* file(HWND hwnd)
{
    File* f = (File*)malloc(sizeof(File));
    if (!f) return NULL;

    f->parentHwnd = hwnd;
    f->fileName = NULL; //init fileName
    f->fileContent = NULL; // init
    f->fptr = NULL;

    return f;
}

void destroyFile(File* f)
{
    if (!f) return;
    if (f->fileContent) free(f->fileContent);
    if (f->fileName) free(f->fileName);

    if (f->fptr) fclose(f->fptr);


    free(f);
}

WCHAR* getFileFromDialog(File* f)
{
    OPENFILENAME ofn;
    wchar_t szFile[3780]; //buffer for filePath

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = f->parentHwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = _countof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        size_t len = wcslen(ofn.lpstrFile);
        WCHAR* result = malloc((len + 1) * sizeof(WCHAR));
        if (!result) return NULL;
        wcscpy_s(result, len + 1, ofn.lpstrFile);
        f->fileName = result;

        wprintf(L"Selected File Name: %ls\n", f->fileName);
        return f->fileName;
    }
    else
    {
        printf("No file selected\n");
        return NULL;
    }
}

WCHAR* getFileContent(File* f)
{

    f->fptr = createFilePtr(f, L"rb");
    
    unsigned char bom[3] = { 0 };
    fread(bom, 1, 3, f->fptr);
    rewind(f->fptr);

    Encoding enc = ENC_ANSI; // default fallback
    if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)
        enc = ENC_UTF8;
    else if (bom[0] == 0xFF && bom[1] == 0xFE)
        enc = ENC_UTF16LE;
    else if (bom[0] == 0xFE && bom[1] == 0xFF)
        enc = ENC_UTF16BE;

    size_t wlen;
    f->fileContent = readTextFileToWCHAR(f->fptr, enc, &wlen);
    printf("File Contents: \n");
    printUtf16(f->fileContent, f);

    return f->fileContent;
}

FILE* createFilePtr(File* f, const wchar_t* mode)
{
    if (!f->fileName) return NULL;

    FILE* fileptr = _wfopen(f->fileName, mode);
    if (!fileptr) {
        perror("Error opening file");
        return NULL;
    }
    return fileptr;
}

WCHAR* readTextFileToWCHAR(FILE* f, Encoding enc, size_t* outLength)
{
    if (!f) return NULL;

    // Get file size
    fseek(f, 0, SEEK_END);
    long fSize = ftell(f);
    rewind(f);

    if (fSize <= 0) return NULL;

    // Read file into buffer
    char* buffer = malloc(fSize + 1);
    if (!buffer) return NULL;

    size_t bytesRead = fread(buffer, 1, fSize, f);
    buffer[bytesRead] = '\0';

    WCHAR* wbuffer = NULL;
    size_t wlen = 0;

    if (enc == ENC_UTF8 || enc == ENC_ANSI)
    {
        UINT codepage = (enc == ENC_UTF8) ? CP_UTF8 : CP_ACP;
        char* start = buffer;
        int byteCount = (int)bytesRead;

        // Skip UTF-8 BOM if present
        if (enc == ENC_UTF8 && bytesRead >= 3 &&
            (unsigned char)buffer[0] == 0xEF &&
            (unsigned char)buffer[1] == 0xBB &&
            (unsigned char)buffer[2] == 0xBF)
        {
            start += 3;
            byteCount -= 3;
        }

        wlen = MultiByteToWideChar(codepage, 0, start, byteCount, NULL, 0);
        if (wlen > 0)
        {
            wbuffer = malloc((wlen + 1) * sizeof(WCHAR));
            if (!wbuffer) { free(buffer); return NULL; }
            MultiByteToWideChar(codepage, 0, start, byteCount, wbuffer, (int)wlen);
            wbuffer[wlen] = L'\0';
        }
    }
    else if (enc == ENC_UTF16LE)
    {
        size_t startOffset = 0;
        if (bytesRead >= 2 && (unsigned char)buffer[0] == 0xFF && (unsigned char)buffer[1] == 0xFE)
            startOffset = 2;

        size_t charCount = (bytesRead - startOffset) / sizeof(WCHAR);
        if (charCount > 0)
        {
            wbuffer = malloc((charCount + 1) * sizeof(WCHAR));
            if (!wbuffer) { free(buffer); return NULL; }
            memcpy(wbuffer, buffer + startOffset, charCount * sizeof(WCHAR));
            wbuffer[charCount] = L'\0';
            wlen = charCount;
        }
    }
    else if (enc == ENC_UTF16BE)
    {
        size_t startOffset = 0;
        if (bytesRead >= 2 && (unsigned char)buffer[0] == 0xFE && (unsigned char)buffer[1] == 0xFF)
            startOffset = 2;

        size_t charCount = (bytesRead - startOffset) / sizeof(WCHAR);
        if (charCount > 0)
        {
            wbuffer = malloc((charCount + 1) * sizeof(WCHAR));
            if (!wbuffer) { free(buffer); return NULL; }

            for (size_t i = 0; i < charCount; i++)
            {
                wbuffer[i] = ((unsigned char)buffer[startOffset + 2 * i] << 8) |
                    (unsigned char)buffer[startOffset + 2 * i + 1];
            }
            wbuffer[charCount] = L'\0';
            wlen = charCount;
        }
    }

    free(buffer);

    if (outLength) *outLength = wlen;
    return wbuffer;
}


void printUtf16(WCHAR* text, File* f)
{
    SetConsoleOutputCP(CP_UTF8); // switch console to UTF-8
    int wlen = WideCharToMultiByte(CP_UTF8, 0, f->fileContent, -1, NULL, 0, NULL, NULL);
    char* utf8 = malloc(wlen);
    WideCharToMultiByte(CP_UTF8, 0, f->fileContent, -1, utf8, wlen, NULL, NULL);
    
    printf("%s\n", utf8);

    free(utf8);
}

void writeWCHARToFile(FILE* f, WCHAR* text, Encoding enc)
{
    
}

