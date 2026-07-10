#include <3ds.h>
#include "menus/tools.h"
#include "menus/home_button_sim.h"
#include "memory.h"
#include "draw.h"
#include "fmt.h"
#include "utils.h"
#include "ifile.h"


Menu toolsMenu = {
    "Tools menu",
    {
        { "Set the number of Play Coins", METHOD, .method = &ToolsMenu_EditPlayCoins },
        { "HOME button simulation options...", MENU, .menu = &homeButtonSimMenu },
        {},
    }
};

static Result ToolsMenu_SetPlayCoins(u16 amount)
{
    FS_Archive archive; //extdata archive
    Handle file; //gamecoin file handle
    Result res; //result variable
    FS_Path pathData;
    //its on nand so mediatype nand and extdata id is 0xf000000b, for some reason the low comes before high here,
    //high is always 00048000 for nand extdata https://www.3dbrew.org/wiki/Extdata
    u32 extdataPath[3] = {MEDIATYPE_NAND, 0xf000000b, 0x00048000};  //type low high
    pathData.type = PATH_BINARY; //binary path because titleid
    pathData.size = 12; //3*sizeof(u32)
    pathData.data = (const void*)extdataPath; //data
    //shared extdata archive https://www.3dbrew.org/wiki/Extdata#NAND_Shared_Extdata has the f000000b archive
    res = FSUSER_OpenArchive(&archive, ARCHIVE_SHARED_EXTDATA, pathData);
    if (R_FAILED(res)) { //return if error
        return res;
    }
    // open /gamecoin.dat in extdata archive
    // https://www.3dbrew.org/wiki/Extdata#Shared_Extdata_0xf000000b_gamecoin.dat

    res = FSUSER_OpenFile(&file, archive, fsMakePath(PATH_ASCII, "/gamecoin.dat"), FS_OPEN_WRITE, 0); //open for writing, no attributes necessary
    if (R_FAILED(res)) { //return if error
        FSUSER_CloseArchive(archive); //dont care about error, just close archive since it opened without error
        return res;
    }
    // from 3dbrew
    // offset: 0x4 size: 0x2 desc: Number of Play Coins, (note: size 0x2 so its a u16 value)
    //I think we dont care about the amount of bytes written, so NULL, as buffer we use the provided u16 argument, size is sizeof(u16) which should be 0x2, as u8 (one byte) * 2 is u16
    res = FSFILE_Write(file, NULL, 0x4, &amount, sizeof(u16), 0); 
    if (R_FAILED(res)) {
        FSFILE_Close(file); //dont care about error, just close file since it opened without error
        FSUSER_CloseArchive(archive); //dont care about error, just close archive since it opened without error
        return res;
    }

    res = FSFILE_Close(file);
    if (R_FAILED(res)) { //return if error
        FSUSER_CloseArchive(archive); //dont care about error, just close archive since it opened without error
        return res;
    }
    res = FSUSER_CloseArchive(archive);
    return res;
}

void ToolsMenu_EditPlayCoins(void)
{
    u16 playCoins = 300;
    Result res = 0;
    u32 pressed = 0;

    void updateDisplay(bool showResult)
    {
        Draw_Lock();
        Draw_ClearFramebuffer();
        Draw_DrawMenuFrame("Tools menu");
        Draw_DrawFormattedString(20, 40, COLOR_WHITE, "Set Play Coins: %d", playCoins);
        
        Draw_DrawString(20, 60, COLOR_WHITE, "DPAD Up/Down: +-1\nDPAD Right/Left: +-10\nA: Apply");
        if (showResult) {
            if (R_SUCCEEDED(res))
                Draw_DrawString(20, 100, COLOR_GREEN, "Play Coins successfully set.");
            else
                Draw_DrawFormattedString(20, 100, COLOR_RED, "Error: 0x%08lx", res);
        }

        Draw_DrawString(20, 120, COLOR_GRAY, "Press B to go back.");
        Draw_DrawString(20, 140, COLOR_TITLE, "Note:");
        
        Draw_DrawString(20, 150, COLOR_WHITE, "Changes may not be applied immediately.\nIt will be applied once the app reloads the\nplay coins count.");
        Draw_FlushFramebuffer();
        Draw_Unlock();
    }

    updateDisplay(false);

    do {
        pressed = waitInputWithTimeout(50);

        if (pressed & KEY_A) {
            res = ToolsMenu_SetPlayCoins(playCoins);
            updateDisplay(true);
        } else if (pressed & KEY_B) {
            return;
        } else {
            bool updated = false;
            if (pressed & KEY_DUP) {
                if (playCoins < 300)
                    playCoins++;
                else
                    playCoins = 0;
                updated = true;
            } else if (pressed & KEY_DDOWN) {
                if (playCoins > 0) {
                    playCoins--;
                } else {
                    playCoins = 300;
                }
                updated = true;
            } else if (pressed & KEY_DRIGHT) {
                if (playCoins + 10 > 300)
                    playCoins = 300;
                else
                    playCoins += 10;
                updated = true;
            } else if (pressed & KEY_DLEFT) {
                if (playCoins < 10)
                    playCoins = 0;
                else
                    playCoins -= 10;
                updated = true;
            }

            if (updated) {
                Draw_Lock();
                Draw_DrawString(20, 40, COLOR_WHITE, "Set Play Coins:         ");
                Draw_DrawFormattedString(20, 40, COLOR_WHITE, "Set Play Coins: %d", playCoins);
                Draw_DrawString(20, 100, COLOR_WHITE, "                            ");
                Draw_FlushFramebuffer();
                Draw_Unlock();
            }
        }
    } while (!menuShouldExit);
}