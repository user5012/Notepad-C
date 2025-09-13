#pragma once  
#include <windows.h>  
#include "window.h"

typedef struct Window Window;

typedef struct Keys {  
HACCEL hAccel;
ACCEL* accels;
} Keys;  

Keys* initKeys();  

void addHaccelToWindow(Window* w);

void destroyKeys(Keys* k);
