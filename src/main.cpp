#include "ArduinoGFX/Arduino_GFX_Library.h"
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include <FFat.h>
#include <SPIFFS.h>
#include <SD.h>
#include <SD_MMC.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)

#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, DF_GFX_RST, 0 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include "hw_config.h"

extern "C"
{
#include <noufrendo/nofrendo.h>
}

int16_t bg_color;
extern Arduino_TFT *gfx;
extern void display_begin();
void setup_noufrendo();

//SPIClass spiSD(HSPI);

#include <esp_task_wdt.h>

void setup(void)
{
//    gfx->begin();
//    gfx->fillScreen(BLACK);
//
//#ifdef GFX_BL
//    pinMode(GFX_BL, OUTPUT);
//    digitalWrite(GFX_BL, HIGH);
//#endif
//
//    gfx->setCursor(10, 10);
//    gfx->setTextColor(RED);
//    gfx->println("Hello World!");
//
//    delay(5000); // 5 seconds

    setup_noufrendo();
}

void setup_noufrendo() {

    // disable Core 0 WDT
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
    esp_task_wdt_delete(idle_0);

    // start display
    display_begin();

    // filesystem defined in hw_config.h
    FILESYSTEM_BEGIN

//    SPI.end();
//    spiSD.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
//    if (!SD.begin(SD_CS, spiSD))
//    {
//        Serial.println("Card Mount Failed");
//    }
//    else
//    {
//        Serial.println("Card Mount Successful");
//    }
//    File root = SD.open("/");

    // find first rom file (*.nes)
    File root = filesystem.open("/");
    char *argv[1];
    if (!root)
    {
        Serial.println("Filesystem mount failed! Please check hw_config.h settings.");
        gfx->println("Filesystem mount failed! Please check hw_config.h settings.");
    }
    else
    {
        bool foundRom = false;

        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory())
            {
                // skip
            }
            else
            {
                char *filename = (char *)file.name();
                int8_t len = strlen(filename);
                if (strstr(strlwr(filename + (len - 4)), ".nes"))
                {
                    foundRom = true;
                    char fullFilename[256];
                    sprintf(fullFilename, "%s%s", FSROOT, filename);
                    Serial.println(fullFilename);
                    argv[0] = fullFilename;
                    break;
                }
            }

            file = root.openNextFile();
        }

        if (!foundRom)
        {
            Serial.println("Failed to find rom file, please copy rom file to data folder and upload with \"ESP32 Sketch Data Upload\"");
            argv[0] = "/";
        }

        Serial.println("NoFrendo start!\n");
        nofrendo_main(1, argv);
        Serial.println("NoFrendo end!\n");
    }
}

void loop()
{
//    gfx->setCursor(random(gfx->width()), random(gfx->height()));
//    gfx->setTextColor(random(0xffff), random(0xffff));
//    gfx->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
//    gfx->println("Hello World!");
//
//    delay(1000); // 1 second
}
