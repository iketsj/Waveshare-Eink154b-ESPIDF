#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "epdif.h"
#include "epd1in54b.h"
#include "imagedata.h"

void app_main()
{
    if(Epd_Init() != 0){
        printf("Error initializing E-paper display\n");
        return;
    }
    Epd_DisplayFrame(IMAGE_BLACK, IMAGE_RED);
}

