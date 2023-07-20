#pragma once

#define DEBUG
#define LOG printf
#include "../../rx/rx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long QWORD;
typedef int BOOL;
typedef const char *PCSTR;
typedef const unsigned short *PCWSTR;
typedef void *PVOID;


QWORD rx_dump_module(rx_handle process, QWORD base);
void rx_free_module(QWORD dumped_module);
QWORD rx_scan_pattern(QWORD dumped_module, PCSTR pattern, PCSTR mask, QWORD length);
QWORD rx_read_i64(rx_handle process, QWORD address);
DWORD rx_read_i32(rx_handle process, QWORD address);
WORD rx_read_i16(rx_handle process, QWORD address);
BYTE rx_read_i8(rx_handle process, QWORD address);
float rx_read_float(rx_handle process, QWORD address);
BOOL rx_write_float(rx_handle process, QWORD address, float value);
BOOL rx_write_i32(rx_handle process, QWORD address, DWORD value);
int rx_read_int(rx_handle process, QWORD address);

QWORD ResolveRelativeAddressEx(
    rx_handle process,
    QWORD Instruction,
    DWORD OffsetOffset,
    DWORD InstructionSize);

QWORD rx_read_i64(rx_handle process, QWORD address)
{
	QWORD buffer = 0;
	rx_read_process(process, address, &buffer, sizeof(buffer));
	return buffer;
}

DWORD rx_read_i32(rx_handle process, QWORD address)
{
	DWORD buffer = 0;
	rx_read_process(process, address, &buffer, sizeof(buffer));
	return buffer;
}

WORD rx_read_i16(rx_handle process, QWORD address)
{
	WORD buffer = 0;
	rx_read_process(process, address, &buffer, sizeof(buffer));
	return buffer;
}

BYTE rx_read_i8(rx_handle process, QWORD address)
{
	BYTE buffer = 0;
	rx_read_process(process, address, &buffer, sizeof(buffer));
	return buffer;
}


float rx_read_float(rx_handle process, QWORD address)
{
	float buffer = 0;
	rx_read_process(process, address, &buffer, sizeof(buffer));
	return buffer;
}

BOOL rx_write_i32(rx_handle process, QWORD address, DWORD value)
{
	return rx_write_process(process, address, &value, sizeof(value)) == sizeof(value);
}

BOOL rx_write_float(rx_handle process, QWORD address, float value)
{
	return rx_write_process(process, address, &value, sizeof(value)) == sizeof(value);
}

int rx_read_int(rx_handle process, QWORD address)
{
	int buffer = 0;
	rx_read_process(process, address, &buffer, sizeof(buffer));
	return buffer;
}

QWORD ResolveRelativeAddressEx(
    rx_handle process,
    QWORD Instruction,
    DWORD OffsetOffset,
    DWORD InstructionSize)
{

	QWORD Instr = (QWORD)Instruction;
	DWORD RipOffset = rx_read_i32(process, Instr + OffsetOffset);
	QWORD ResolvedAddr = (QWORD)(Instr + InstructionSize + RipOffset);
	return ResolvedAddr;
}

QWORD rx_dump_module(rx_handle process, QWORD base)
{
	QWORD a0, a1, a2, a3 = 0;
	char *a4;

	a0 = base;
	if (a0 == 0)
		return 0;

	a1 = rx_read_i32(process, a0 + 0x03C) + a0;
	if (a1 == a0)
	{
		return 0;
	}

	a2 = rx_read_i32(process, a1 + 0x050);
	if (a2 < 8)
		return 0;

	a4 = (char *)malloc(a2 + 24);

	*(QWORD *)(a4) = base;
	*(QWORD *)(a4 + 8) = a2;
	*(QWORD *)(a4 + 16) = a3;

	a4 += 24;

	QWORD image_dos_header = base;
	QWORD image_nt_header = rx_read_i32(process, image_dos_header + 0x03C) + image_dos_header;

	DWORD headers_size = rx_read_i32(process, image_nt_header + 0x54);
	rx_read_process(process, image_dos_header, a4, headers_size);

	unsigned short machine = rx_read_i16(process, image_nt_header + 0x4);

	QWORD section_header = machine == 0x8664 ? image_nt_header + 0x0108 : image_nt_header + 0x00F8;

	for (WORD i = 0; i < rx_read_i16(process, image_nt_header + 0x06); i++)
	{

		QWORD section = section_header + (i * 40);
		QWORD local_virtual_address = base + rx_read_i32(process, section + 0x0c);
		DWORD local_virtual_size = rx_read_i32(process, section + 0x8);
		QWORD target_virtual_address = (QWORD)a4 + rx_read_i32(process, section + 0xc);
		rx_read_process(process, local_virtual_address, (PVOID)target_virtual_address, local_virtual_size);
	}

	return (QWORD)a4;
}

void rx_free_module(QWORD dumped_module)
{
	dumped_module -= 24;
	free((void *)dumped_module);
}

BOOL bDataCompare(const BYTE *pData, const BYTE *bMask, const char *szMask)
{

	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;

	return (*szMask) == 0;
}

QWORD FindPatternEx(QWORD dwAddress, QWORD dwLen, BYTE *bMask, char *szMask)
{

	if (dwLen <= 0)
		return 0;
	for (QWORD i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE *)(dwAddress + i), bMask, szMask))
			return (QWORD)(dwAddress + i);

	return 0;
}

QWORD rx_scan_pattern(QWORD dumped_module, PCSTR pattern, PCSTR mask, QWORD length)
{
	QWORD ret = 0;
	QWORD a0;

	if (dumped_module == 0)
		return 0;

	dumped_module -= 24;
	a0 = *(QWORD *)(dumped_module);
	dumped_module += 24;

	QWORD image_dos_header = dumped_module;
	QWORD image_nt_header = *(DWORD *)(image_dos_header + 0x03C) + image_dos_header;

	unsigned short machine = *(unsigned short *)(image_nt_header + 0x4);

	QWORD section_header = machine == 0x8664 ? image_nt_header + 0x0108 : image_nt_header + 0x00F8;

	for (WORD i = 0; i < *(unsigned short *)(image_nt_header + 0x06); i++)
	{

		QWORD section = section_header + (i * 40);
		QWORD section_address = image_dos_header + *(DWORD *)(section + 0x14);
		DWORD section_size = *(DWORD *)(section + 0x10);
		DWORD section_characteristics = *(DWORD *)(section + 0x24);

		if (section_characteristics & 0x00000020)
		{
			QWORD address = FindPatternEx(section_address, section_size - length, (BYTE *)pattern, (char *)mask);
			if (address)
			{
				ret = (address - dumped_module) + a0;
				break;
			}
		}
	}
	return ret;
}
