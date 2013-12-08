#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "pifacecad.h"

/*
    Sample code for PiFace PifaceCAD (Control and Display)
    By Gregory Fenton
    http://labby.co.uk
    
    Language: C
    To generate the required libraries, follow the instructions at
    http://piface.github.io/libpifacecad/
    
    Compilation:
        gcc -o pifacecad_review pifacecad_review.c -Isrc/ -L. -lpifacecad -L../libmcp23s17/ -lmcp23s17
    Usage:
        ./pifacecad_review
        Press a button on the PiFaceCAD to exit
*/

int main(void)
{
   uint8_t pifacelogo[] = {
      //piface logo top characters
      0b00001, 0b00111, 0b01111, 0b11111, 0b11111, 0b11110, 0b11110, 0b11111,
      0b11100, 0b11110, 0b11111, 0b11111, 0b11111, 0b01111, 0b01110, 0b11110,
      //piface logo bottom characters
      0b11111, 0b11111, 0b01111, 0b01111, 0b00111, 0b00011, 0b00000, 0b00000,
      0b01100, 0b01000, 0b10100, 0b10100, 0b11010, 0b11111, 0b00000, 0b00000 };
   uint8_t santa[] = {
      //left 3 characters of Santa
      0b00000, 0b00001, 0b00011, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111,
      0b01111, 0b01111, 0b10000, 0b01111, 0b01000, 0b01010, 0b10101, 0b10010,
      0b10001, 0b10000, 0b10111, 0b10100, 0b10011, 0b01011, 0b01001, 0b00111,
      //right 3 characters of Santa
      0b00010, 0b10101, 0b11010, 0b10000, 0b10000, 0b11000, 0b11100, 0b11100,
      0b11110, 0b11110, 0b00001, 0b11110, 0b00010, 0b01010, 0b10101, 0b01001,
      0b10001, 0b00001, 0b11101, 0b00101, 0b11001, 0b11010, 0b10010, 0b11100 };
   
   uint8_t checkerboard[8] = { 
      // checkerboard pattern
      0b10101, 0b01010, 0b10101, 0b01010, 0b10101, 0b01010, 0b10101, 0b01010 };
   
   int i, j, address, dir;
   int delay = 250 * 1000;
   char buffer[256], sub[14] = "";
   uint8_t switches;
   
   snprintf(buffer, sizeof(buffer), "              %s              ",
	    "Press any PiFace Control and Display button to exit");
   pifacecad_open();
   pifacecad_lcd_backlight_on();
   pifacecad_lcd_cursor_off();
   pifacecad_lcd_blink_off();
   pifacecad_lcd_store_custom_bitmap(0, pifacelogo);
   pifacecad_lcd_store_custom_bitmap(1, pifacelogo + 8);
   pifacecad_lcd_store_custom_bitmap(2, pifacelogo + 16);
   pifacecad_lcd_store_custom_bitmap(3, pifacelogo + 24);
   pifacecad_lcd_store_custom_bitmap(4, checkerboard);
   pifacecad_lcd_write_custom_bitmap(0);
   pifacecad_lcd_write_custom_bitmap(1);
   pifacecad_lcd_write("  PiFace CAD\n");
   pifacecad_lcd_write_custom_bitmap(2);
   pifacecad_lcd_write_custom_bitmap(3);
   address = pifacecad_lcd_get_cursor_address();
   pifacecad_lcd_write("   Element14   ");
   sleep(1);
   pifacecad_lcd_set_cursor_address(address);
   pifacecad_lcd_write("   Review by   ");
   sleep(1);
   pifacecad_lcd_set_cursor_address(address);
   pifacecad_lcd_write(" GregoryFenton ");
   sleep(1);
   pifacecad_lcd_set_cursor_address(address);
   pifacecad_lcd_write("  labby.co.uk  ");
   pifacecad_lcd_store_custom_bitmap(0, santa);
   pifacecad_lcd_store_custom_bitmap(1, santa + 24);
   pifacecad_lcd_store_custom_bitmap(2, santa + 8);
   pifacecad_lcd_store_custom_bitmap(3, santa + 32);
   sleep(3);
   i = j = dir = 1;
   //pifacecad_lcd_write_custom_bitmap(4);
   while(1)
   {
      pifacecad_lcd_store_custom_bitmap(0, santa + j);
      pifacecad_lcd_store_custom_bitmap(1, santa + j + 24);
      pifacecad_lcd_store_custom_bitmap(2, santa + j + 8);
      pifacecad_lcd_store_custom_bitmap(3, santa + j + 32);
      pifacecad_lcd_set_cursor_address(address);
      snprintf(sub, 15, "%10s", buffer + i);
      sub[0] = 4;
      sub[13] = 4;
      pifacecad_lcd_write(sub);
      usleep(delay);
      i++;
      j += dir;
      if(j == 8)
	dir = -1;
      if(j == 0)
	dir = 1;
      if(i > strlen(buffer) - 13)
	i = 0;
      switches = pifacecad_read_switches();
      if(switches != 0xff)
	break;
   }
   pifacecad_lcd_clear();
   pifacecad_lcd_write("Goodbye\nlabby.co.uk");
   pifacecad_close();

   return 0;
}
