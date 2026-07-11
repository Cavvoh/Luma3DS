#include <3ds.h>
#include <3ds/os.h>
#include "menu.h"
#include "menus.h"
#include "draw.h"
#include "menus/info.h"
#include "plugin.h"
#include "pmdbgext.h"
#include "ifile.h"
#include "memory.h"
#include "fmt.h"
#include "process_patches.h"
#include "luma_config.h"


Menu infoMenu = {
    "Info menu",
    {
        { "System info", METHOD, .method = &InfoMenu_ShowSystemInfo },
        { "Debug info", METHOD, .method = &InfoMenu_ShowDebugInfo, .visibility = &infoMenuShouldShowDebugInfo },
        {},
    }
};

bool infoMenuShouldShowDebugInfo(void)
{
    // Don't show on release builds

    s64 out;
    svcGetSystemInfo(&out, 0x10000, 0x200);
    return out == 0;
}

void InfoMenu_ShowSystemInfo(void)
{
    u32 kver = osGetKernelVersion();

    do
    {
        Draw_Lock();
        Draw_DrawMenuFrame("System info");

        u32 posY = 40;

        if (areScreenTypesInitialized)
        {
            posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "Top screen type:    %s\n", topScreenType);
            posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "Bottom screen type: %s\n\n", bottomScreenType);
        }

        posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "Kernel version:     %lu.%lu-%lu\n\n", GET_VERSION_MAJOR(kver), GET_VERSION_MINOR(kver), GET_VERSION_REVISION(kver));
        if (mcuFwVersion != 0 && mcuInfoTableRead)
        {
            posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "MCU FW version:     %lu.%lu\n", GET_VERSION_MAJOR(mcuFwVersion), GET_VERSION_MINOR(mcuFwVersion));
            posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "PMIC vendor:        %hhu\n", mcuInfoTable[1]);
            posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "Battery vendor:     %hhu\n", mcuInfoTable[2]);
            posY = Draw_DrawString(20, posY, COLOR_WHITE, "\n");
        }

        u64 titleId = 0;
        Get_TitleID(&titleId);
        if (titleId != 0)
            posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "Title ID:           %016llX\n", titleId);
        else
            posY = Draw_DrawString(20, posY, COLOR_WHITE, "Title ID:           Not Found\n");

        Draw_FlushFramebuffer();
        Draw_Unlock();
    }
    while(!(waitInput() & KEY_B) && !menuShouldExit);
}

void InfoMenu_ShowDebugInfo(void)
{
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    char memoryMap[512];
    formatMemoryMapOfProcess(memoryMap, 511, CUR_PROCESS_HANDLE);

    s64 kextAddrSize;
    svcGetSystemInfo(&kextAddrSize, 0x10000, 0x300);
    u32 kextPa = (u32)((u64)kextAddrSize >> 32);
    u32 kextSize = (u32)kextAddrSize;

    FS_SdMmcSpeedInfo speedInfo;

    do
    {
        Draw_Lock();
        Draw_DrawMenuFrame("Debug info");

        u32 posY = 40;

        posY = Draw_DrawString(20, posY, COLOR_WHITE, memoryMap);
        posY = Draw_DrawFormattedString(20, posY, COLOR_WHITE, "Kernel ext PA: %08lx - %08lx\n\n", kextPa, kextPa + kextSize);
        if (R_SUCCEEDED(FSUSER_GetSdmcSpeedInfo(&speedInfo)))
        {
            u32 clkDiv = 1 << (1 + (speedInfo.sdClkCtrl & 0xFF));
            posY = Draw_DrawFormattedString(
                20, posY, COLOR_WHITE, "SDMC speed: HS=%d %lukHz\n",
                (int)speedInfo.highSpeedModeEnabled, SYSCLOCK_SDMMC / (1000 * clkDiv)
            );
        }
        if (R_SUCCEEDED(FSUSER_GetNandSpeedInfo(&speedInfo)))
        {
            u32 clkDiv = 1 << (1 + (speedInfo.sdClkCtrl & 0xFF));
            posY = Draw_DrawFormattedString(
                20, posY, COLOR_WHITE, "NAND speed: HS=%d %lukHz\n",
                (int)speedInfo.highSpeedModeEnabled, SYSCLOCK_SDMMC / (1000 * clkDiv)
            );
        }
        {
            posY = Draw_DrawFormattedString(
                20, posY, COLOR_WHITE, "APPMEMTYPE: %lu\n",
                OS_KernelConfig->app_memtype
            );
        }
        Draw_FlushFramebuffer();
        Draw_Unlock();
    }
    while(!(waitInput() & KEY_B) && !menuShouldExit);
}