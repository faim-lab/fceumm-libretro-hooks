/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Family Study Box by Fukutake Shoten
 */

#include "mapinc.h"

static uint8 SWRAM[3072];
static uint8 *WRAM = NULL;
static uint8 regs[4];

static SFORMAT StateRegs[] =
{
	{ regs, 4, "DREG" },
	{ SWRAM, 3072, "SWRM" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, regs[0] >> 6);
	setprg16(0x8000, regs[1]);
	setprg16(0xc000, 0);
}

static void M186Write(uint32 A, uint8 V) {
	if (A & 0x4203) regs[A & 3] = V;
	Sync();
}

static uint8 M186Read(uint32 A) {
	switch (A) {
	case 0x4202: return 0x40; break;
	case 0x4200:
	case 0x4201:
	case 0x4203:
		     return 0x00; break;
	}
	return 0xFF;
}

static uint8 ASWRAM(uint32 A) { return(SWRAM[A - 0x4400]); }
static void BSWRAM(uint32 A, uint8 V) { SWRAM[A - 0x4400] = V; }

static void M186Power(void) {
	setchr8(0);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, CartBW);
	SetReadHandler(0x4200, 0x43FF, M186Read);
	SetWriteHandler(0x4200, 0x43FF, M186Write);
	SetReadHandler(0x4400, 0x4FFF, ASWRAM);
	SetWriteHandler(0x4400, 0x4FFF, BSWRAM);
	FCEU_CheatAddRAM(32, 0x6000, WRAM);
	regs[0] = regs[1] = regs[2] = regs[3];
	Sync();
}

static void M186Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void M186Restore(int version) {
	Sync();
}

void Mapper186_Init(CartInfo *info) {
	info->Power = M186Power;
	info->Close = M186Close;
	GameStateRestore = M186Restore;
	WRAM = (uint8*)FCEU_gmalloc(32768);
	SetupCartPRGMapping(0x10, WRAM, 32768, 1);
	AddExState(WRAM, 32768, 0, "WRAM");
	AddExState(StateRegs, ~0, 0, 0);
}
