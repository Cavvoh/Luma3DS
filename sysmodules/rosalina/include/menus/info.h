#pragma once

#include <3ds/types.h>
#include "menu.h"

extern Menu infoMenu;

void InfoMenu_ShowSystemInfo();

bool infoMenuShouldShowDebugInfo(void);
void InfoMenu_ShowDebugInfo(void);