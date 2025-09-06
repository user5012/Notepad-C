# Notepad C

**Notepad C** is a lightweight text editor written entirely in **C** using the **Win32 API**. It provides a simple Windows-style GUI for basic text editing and is a great project for learning native Windows GUI programming.

## Features

- Create, open, edit, and save text files.
- Basic editing operations: cut, copy, paste, undo, select all.
- Menu bar with standard options (File, Edit, Help).
- Pure Win32 API implementation – no external libraries required.
- Lightweight and fast.

## Screenshots

*(Add screenshots here if available)*

## Getting Started

### Prerequisites

- Windows OS (Win32 compatible)
- C compiler (e.g., **MSVC**, **MinGW**, **TDM-GCC**)
- Basic understanding of compiling C programs on Windows

### Compilation

Using **MinGW**:

```bash
gcc -o NotepadC.exe main.c -mwindows
```

Using **MSVC** (Developer Command Prompt):
```bash
cl /DUNICODE /D_UNICODE main.c user32.lib gdi32.lib
```

### Running

After compilation, run:
```bash
NotepadC.exe
```
You should see a simple Notepad-style window with text editing functionality.

## Usage

* File → New: Create a new document

* File → Open: Open an existing text file

* File → Save / Save As: Save the current document

* Edit → Cut / Copy / Paste / Undo / Select All: Standard text editing operations

## License

This project is open-source and free to use under the [MIT License](https://github.com/user5012/Notepad-C/blob/master/LICENSE.txt).
