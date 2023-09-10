#pragma once

#include "handling.h"

//typedef std::chrono::high_resolution_clock Clock;

//#define clear() printf("\033[H\033[J")

int GetApexProcessId(void)
{
	int pid = 0;
	rx_handle snapshot = rx_create_snapshot(RX_SNAP_TYPE_PROCESS, 0);

	RX_PROCESS_ENTRY entry;

	while (rx_next_process(snapshot, &entry))
	{
		if (!strcmp(entry.name, "wine64-preloader"))
		{
			rx_handle snapshot_2 = rx_create_snapshot(RX_SNAP_TYPE_LIBRARY, entry.pid);

			RX_LIBRARY_ENTRY library_entry;

			while (rx_next_library(snapshot_2, &library_entry))
			{
				if (!strcmp(library_entry.name, "easyanticheat_x64.dll"))
				{
					pid = entry.pid;
					break;
				}
			}
			rx_close_handle(snapshot_2);
			//
			// process found
			//
			if (pid != 0)
			{
				break;
			}
		}
	}
	rx_close_handle(snapshot);

	return pid;
}

QWORD GetApexBaseAddress(int pid)
{
	rx_handle snapshot = rx_create_snapshot(RX_SNAP_TYPE_LIBRARY, pid);

	RX_LIBRARY_ENTRY entry;
	DWORD counter = 0;
	QWORD base = 0;

	while (rx_next_library(snapshot, &entry))
	{
		const char *sub = strstr(entry.name, "memfd:wine-mapping");

		if ((entry.end - entry.start) == 0x1000 && sub)
		{
			if (counter == 0)
				base = entry.start;
		}

		if (sub)
		{
			counter++;
		}

		else
		{
			counter = 0;
			base = 0;
		}

		if (counter >= 200)
		{
			break;
		}
	}

	rx_close_handle(snapshot);

	return base;
}

QWORD get_interface_function(rx_handle game_process, QWORD ptr, DWORD index)
{
	return rx_read_i64(game_process, rx_read_i64(game_process, ptr) + index * 8);
}

int dump_table(rx_handle game_process, QWORD table, const char *name)
{
	for (DWORD i = 0; i < rx_read_i32(game_process, table + 0x10); i++)
	{

		QWORD recv_prop = rx_read_i64(game_process, table + 0x8);
		if (!recv_prop)
		{
			continue;
		}

		recv_prop = rx_read_i64(game_process, recv_prop + 0x8 * i);
		char recv_prop_name[260];
		{
			QWORD name_ptr = rx_read_i64(game_process, recv_prop + 0x28);
			rx_read_process(game_process, name_ptr, recv_prop_name, 260);
		}

		if (!strcmp(recv_prop_name, name))
		{
			return rx_read_i32(game_process, recv_prop + 0x4);
		}
	}

	return 0;
}

// Input System Dotton Down
BOOL IsButtonDown(rx_handle game_process, QWORD IInputSystem, int KeyCode)
{
	KeyCode = KeyCode + 1;
	DWORD a0 = rx_read_i32(game_process, IInputSystem + ((KeyCode >> 5) * 4) + 0xb0);
	return (a0 >> (KeyCode & 31)) & 1;
}
