#include "cecd.h"

static Handle cecduHandle;

Result cecdInit()
{
	return srvGetServiceHandle(&cecduHandle, "cecd:u");
}

Result cecdExit()
{
	return svcCloseHandle(cecduHandle);
}

Result CECDU_OpenRawFile(u32* out, u32 titleId, u32 path_type,u32 openFlags, u32 processId)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x1,3,2); // 0x100C2
	cmdbuf[1] = titleId;
	cmdbuf[2] = path_type;
	cmdbuf[3] = openFlags;
	cmdbuf[4] = processId;
	cmdbuf[5] = 0;

	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(cecduHandle))) return ret;

	if(out) *out = cmdbuf[2];

	return cmdbuf[1];
}

Result CECDU_ReadRawFile(u32* out, u32 bufferSize, void* buffer)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x2,1,2); // 0x20042
	cmdbuf[1] = 0; //buffer size (unused)
	cmdbuf[2] = IPC_Desc_Buffer(bufferSize, IPC_BUFFER_W);
	cmdbuf[3] = (u32)buffer;
	

	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(cecduHandle))) return ret;

	if(out) *out = cmdbuf[2];

	return cmdbuf[1];
}

Result CECDU_ReadMessage(u32* out, u32 titleId, bool is_out_box , u32* buffer)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x3,4,4); // 0x30104
	cmdbuf[1] = titleId;
	cmdbuf[2] = is_out_box;
	cmdbuf[3] = 8; //messageIdSize
	cmdbuf[4] = 1000; //bufferSize
	cmdbuf[5] = 0xA;
	cmdbuf[6] = 0;
	cmdbuf[7] = 0xC;
	cmdbuf[8] = (u32)buffer;

	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(cecduHandle))) return ret;

	if(out) *out = cmdbuf[2];

	return cmdbuf[1];
}

Result CECDU_RunCommand(CecCommand command)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0xB,1,0); // 0xB0040
	cmdbuf[1] = command;

	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(cecduHandle))) return ret;

	return cmdbuf[1];
}

Result CECDU_GetCecStateAbbreviated(u32* out)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0xE,0,0); // 0xE0000

	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(cecduHandle))) return ret;

	if(out) *out = cmdbuf[2];
	
	return cmdbuf[1];
}

Result CECDU_OpenAndRead(u32* out, u32 titleId, u32 path_type,u32 openFlags, u32 bufferSize, void* buffer)
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x12,4,4); // 0x120104
	cmdbuf[1] = 0;
	cmdbuf[2] = titleId;
	cmdbuf[3] = path_type;
	cmdbuf[4] = openFlags;
	cmdbuf[5] = 0x20; //Descriptor for Process ID
	cmdbuf[6] = 0;
	cmdbuf[7] = IPC_Desc_Buffer(bufferSize, IPC_BUFFER_W);
	cmdbuf[8] = (u32)buffer;
	

	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(cecduHandle))) return ret;

	if(out) *out = cmdbuf[4];

	return cmdbuf[1];
}