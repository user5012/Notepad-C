#include "keys.h"  

Keys* initKeys()  
{  
   Keys* k = (Keys*)malloc(sizeof(Keys));  
   if (!k) return NULL;  
   int numAccels = 3;
   k->accels = (ACCEL*)malloc(numAccels * sizeof(ACCEL));
   if (!k->accels) {
	   free(k);
	   return NULL;
   }
   // Define your accelerators here
   k->accels[0] = (ACCEL){ FVIRTKEY | FCONTROL, 'O', 0 };  // Ctrl + O for Open
   k->accels[1] = (ACCEL){ FVIRTKEY | FCONTROL, 'S', 1 };  // Ctrl + S for Save
   k->accels[2] = (ACCEL){ FVIRTKEY | FCONTROL | FALT, 'S', 2 }; // Ctrl + Alt + S for Save As
   
   

   //ACCEL accels[] = {
	  // { FVIRTKEY | FCONTROL, 'O', 0},  // Ctrl + O for Open
	  // { FVIRTKEY | FCONTROL, 'S', 1},  // Ctrl + S for Save
	  // { FVIRTKEY | FCONTROL | FALT, 'S', 2} // Ctrl + Alt + S for Save As
   //};

   k->hAccel = CreateAcceleratorTable(k->accels, numAccels);

   return k;  
}

void addHaccelToWindow(Window* w)
{
	if (!w) return;
	Keys* k = initKeys();
	printf("Keys initialized\n");
	w->keys = k;
}

void destroyKeys(Keys* k)
{
	if (!k) return;
	if (k->hAccel) DestroyAcceleratorTable(k->hAccel);
	if (k->accels) free(k->accels);
	free(k);
}
