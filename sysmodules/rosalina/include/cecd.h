#pragma once

#include <3ds.h>

#define FLAGREAD 2;
#define FLAGWRITE 4;

typedef enum
{
	CEC_COMMAND_NONE = 0,
	CEC_COMMAND_START = 1,
	CEC_COMMAND_RESET_START = 2,
	CEC_COMMAND_READYSCAN = 3,
	CEC_COMMAND_READYSCANWAIT = 4,
	CEC_COMMAND_RESET_FILTER = 14,
} CecCommand;

Result cecdInit(void);
Result cecdExit(void);

Result CECDU_OpenRawFile(u32* out, u32 titleId, u32 path_type,u32 openFlags, u32 processId);
Result CECDU_ReadRawFile(u32* out, u32 bufferSize, void* buffer);
Result CECDU_ReadMessage(u32* out, u32 titleId, bool is_out_box , u32* buffer);
Result CECDU_RunCommand(CecCommand command);
Result CECDU_GetCecStateAbbreviated(u32* out);
Result CECDU_OpenAndRead(u32* out, u32 titleId, u32 path_type,u32 openFlags, u32 bufferSize, void* buffer);