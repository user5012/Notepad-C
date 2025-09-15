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
    f->hFile = NULL;

    return f;
}

void setFileName(File* f, WCHAR* name)
{
    if (f->fileName) free(f->fileName);
    size_t len = wcslen(name);
    f->fileName = malloc((len + 1) * sizeof(WCHAR));
    if (!f->fileName) return;
    wcscpy_s(f->fileName, len + 1, name);
}

void destroyFile(File* f)
{
    if (!f) return;
    /*if (f->fileContent) free(f->fileContent);*/
    /*if (f->fileName) free(f->fileName);*/

    /*if (f->hFile) CloseHandle(f->hFile);*/


    free(f);
}

WCHAR* getFileFromDialog(File* f, BOOL isOpenFileDialog)
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

    
    if (isOpenFileDialog) {
        ofn.lpstrTitle = L"Open File";
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
	else {
		ofn.lpstrTitle = L"Save File As";
		if (GetSaveFileNameW(&ofn)) {
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
    
}

WCHAR* getFileContent(File* f)
{
    //Open File
	f->hFile = CreateFileW(f->fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f->hFile == INVALID_HANDLE_VALUE) {
		perror("Error opening file");
		return NULL;
	}

    // get file size
	DWORD fileSize = GetFileSize(f->hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        perror("Error getting file size or file is empty");
        CloseHandle(f->hFile);
        return NULL;
    }
	
	BYTE* raw = (BYTE*)malloc(fileSize + 2);
    if (!raw) {
        perror("Memory allocation failed");
        CloseHandle(f->hFile);
        return NULL;
	}

	DWORD bytesRead;
    if (!ReadFile(f->hFile, raw, fileSize, &bytesRead, NULL)) {
		wprintf(L"Error reading file: %d\n", GetLastError());
		free(raw);
		CloseHandle(f->hFile);
		return NULL;
    }

    CloseHandle(f->hFile); //clear handler

	WCHAR* wide = NULL;
    int len = 0;
	// BOM check for UTF-16 LE
    if (bytesRead >= 2 && raw[0] == 0xFF && raw[1] == 0xFE) {
		// assume UTF-16 LE
		wide = (WCHAR*)(raw + 2); // skip BOM
		len = (bytesRead - 2) / 2; // each WCHAR is 2 bytes
		f->fileContent = _wcsdup(wide);
		free(raw); // free raw buffer as we have duplicated the content
    }
    else {
		// assume UTF-8, convert to WCHAR
		len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)raw, bytesRead, NULL, 0);
		wide = (WCHAR*)malloc((len + 1) * sizeof(WCHAR));
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)raw, bytesRead, wide, len);
		wide[len] = L'\0'; // null-terminate
		free(raw); // free raw buffer as we have converted the content
		f->fileContent = wide;
	}

    
	wprintf(L"File content read (%d bytes):\n%ls\n", bytesRead, f->fileContent);
    
	return f->fileContent;
}
void writeWCHARToFile(File* f, WCHAR* text)
{

	if (!f || !text) return;

	f->hFile = CreateFileW(f->fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD bytesWritten;
	BOOL success = WriteFile(f->hFile, text, (DWORD)(wcslen(text) * sizeof(WCHAR)), &bytesWritten, NULL);

    if (!f->hFile || f->hFile == INVALID_HANDLE_VALUE) {
        wprintf(L"Error creating or opening file: %d\n", GetLastError());
        return;
	}


	if (!success) {
		wprintf(L"Error writing to file: %d\n", GetLastError());
	}
	else {
		wprintf(L"Wrote %d bytes to file successfully.\n", bytesWritten);
	}

	CloseHandle(f->hFile); // close handle after writing

	// Clean up in destroyFile(f)
    
}

void saveFile(Window* w) {
	if (!w) return;
    if (w->isSaved) {
        printf("File is already saved, no changes to save.\n");
        return;
	}
    WCHAR* currentText = getTxtBoxText(w->txt_boxes[0]->txtBox);
    wprintf(L"Text to save: %ls\n", currentText);
    if (currentText) {
        printf("Current Text: %ls\n", currentText);
        if (!w->openedFileName || w->OpenedFilePtr) {
            // save as logic
            saveFileAs(w);
			free(currentText);
			return;
        }
        writeWCHARToFile(w->OpenedFilePtr, currentText);
        free(w->OpenedFilePtr->fileContent);
        w->OpenedFilePtr->fileContent = _wcsdup(currentText);

        w->isSaved = TRUE;
		updateTitleIfChanges(w); // update title to remove asterisk
        free(currentText);
    }
}

void saveFileAs(Window* w) {
	if (!w) return;
    File* f = file(w->hwnd);
    WCHAR* fileName = getFileFromDialog(f, FALSE);
    WCHAR* currentText = getTxtBoxText(w->txt_boxes[0]->txtBox);
    if (fileName) {
        w->openedFileName = fileName;
        updateTitle(w, w->openedFileName);
        writeWCHARToFile(f, currentText);
		w->OpenedFilePtr = f;
		w->OpenedFilePtr->fileContent = _wcsdup(currentText); // update file content in struct
        w->isSaved = TRUE;
        updateTitleIfChanges(w); // update title to remove asterisk
		free(currentText);
    }
}

void openFile(Window* w) {
    if (!w->fileToOpen) return;
	File* f = file(w->hwnd);
	f->fileName = w->fileToOpen;
    if (!f->fileName) {
        perror("No file selected");
        destroyFile(f);
        return;
	}
    w->openedFileName = f->fileName;
	getFileContent(f);
	w->OpenedFilePtr = f;
	updateTxtBoxText(w->txt_boxes[0]->txtBox, f->fileContent);
	w->isSaved = TRUE;
}