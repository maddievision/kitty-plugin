/* Copyright 2026 Maddie Lim
 *
 * Kitty Advance Plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Kitty Advance Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with  Kitty Advance Plugin.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <tonc.h>
#include <tonc_bios.h>
#include "demobank.h"
#include "kitty.h"

KittyState kitty;

int main() {
  irq_init(NULL);
  irq_add(II_VBLANK, NULL);

  DemoBankInit();
  KittyInit(&kitty, &sndbank, &drumbank, 32, 11, 4, 10, 1);
  KittySetVSync(&kitty, 1);

  while(1) {
    VBlankIntrWait();
    KittyVSync(&kitty);
    KittyMain(&kitty);
  }
  return 0;
}
