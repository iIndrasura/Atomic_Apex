#pragma once

#define DEBUG
#define LOG printf
#include "../../rx/rx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <string> 
#include <cstdlib>
#include <stdexcept>


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

QWORD rx_read_i64(rx_handle process, QWORD address);

DWORD rx_read_i32(rx_handle process, QWORD address);

WORD rx_read_i16(rx_handle process, QWORD address);

BYTE rx_read_i8(rx_handle process, QWORD address);

std::string rx_read_string(rx_handle process, QWORD address);

float rx_read_float(rx_handle process, QWORD address);

BOOL rx_write_i32(rx_handle process, QWORD address, DWORD value);

BOOL rx_write_float(rx_handle process, QWORD address, float value);

int rx_read_int(rx_handle process, QWORD address);

QWORD ResolveRelativeAddressEx(rx_handle process, QWORD Instruction, DWORD OffsetOffset, DWORD InstructionSize);

QWORD rx_dump_module(rx_handle process, QWORD base);

void rx_free_module(QWORD dumped_module);

void rx_write_array(rx_handle process, QWORD address, char* buffer, int size);

BOOL bDataCompare(const BYTE *pData, const BYTE *bMask, const char *szMask);

QWORD FindPatternEx(QWORD dwAddress, QWORD dwLen, BYTE *bMask, char *szMask);

QWORD rx_scan_pattern(QWORD dumped_module, PCSTR pattern, PCSTR mask, QWORD length);
