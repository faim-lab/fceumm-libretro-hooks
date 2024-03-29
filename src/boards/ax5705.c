/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 * Super Bros. Pocker Mali (VRC4 mapper)
 */

#include "mapinc.h"

static uint8 IRQCount;	/*, IRQPre; */
static uint8 IRQa;
static uint8 prg_reg[2];
static uint8 chr_reg[8];
static uint8 mirr;

static SFORMAT StateRegs[] =
{
	{ &IRQCount, 1, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ prg_reg, 2, "PRG" },
	{ chr_reg, 8, "CHR" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	int i;
	setprg8(0x8000, prg_reg[0]);
	setprg8(0xA000, prg_reg[1]);
	setprg8(0xC000, ~1);
	setprg8(0xE000, ~0);
	for (i = 0; i < 8; i++)
		setchr1(i << 10, chr_reg[i]);
	setmirror(mirr ^ 1);
}

static void UNLAX5705Write(uint32 A, uint8 V) {
	switch (A & 0xF00F) {
	case 0x8000: prg_reg[0] = ((V & 2) << 2) | ((V & 8) >> 2) | (V & 5); break;	/* EPROM dump have mixed PRG and CHR banks, data lines to mapper seems to be mixed */
	case 0x8008: mirr = V & 1; break;
	case 0xA000: prg_reg[1] = ((V & 2) << 2) | ((V & 8) >> 2) | (V & 5); break;
	case 0xA008: chr_reg[0] = (chr_reg[0] & 0xF0) | (V & 0x0F); break;
	case 0xA009: chr_reg[0] = (chr_reg[0] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xA00A: chr_reg[1] = (chr_reg[1] & 0xF0) | (V & 0x0F); break;
	case 0xA00B: chr_reg[1] = (chr_reg[1] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xC000: chr_reg[2] = (chr_reg[2] & 0xF0) | (V & 0x0F); break;
	case 0xC001: chr_reg[2] = (chr_reg[2] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xC002: chr_reg[3] = (chr_reg[3] & 0xF0) | (V & 0x0F); break;
	case 0xC003: chr_reg[3] = (chr_reg[3] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xC008: chr_reg[4] = (chr_reg[4] & 0xF0) | (V & 0x0F); break;
	case 0xC009: chr_reg[4] = (chr_reg[4] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xC00A: chr_reg[5] = (chr_reg[5] & 0xF0) | (V & 0x0F); break;
	case 0xC00B: chr_reg[5] = (chr_reg[5] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xE000: chr_reg[6] = (chr_reg[6] & 0xF0) | (V & 0x0F); break;
	case 0xE001: chr_reg[6] = (chr_reg[6] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	case 0xE002: chr_reg[7] = (chr_reg[7] & 0xF0) | (V & 0x0F); break;
	case 0xE003: chr_reg[7] = (chr_reg[7] & 0x0F) | ((((V & 4) >> 1) | ((V & 2) << 1) | (V & 0x09)) << 4); break;
	}
	Sync();
}

static void UNLAX5705Power(void) {
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, UNLAX5705Write);
}

static void StateRestore(int version) {
	Sync();
}

void UNLAX5705_Init(CartInfo *info) {
	info->Power = UNLAX5705Power;
/*	GameHBIRQHook=UNLAX5705IRQ; */
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
