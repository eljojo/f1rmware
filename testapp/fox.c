/*
 * Copyright 2015 team rad1o
 *
 */

#include <unistd.h>

#include <rad1olib/setup.h>
#include <r0ketlib/display.h>
#include <r0ketlib/print.h>
#include <r0ketlib/itoa.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/menu.h>
#include <r0ketlib/select.h>
#include <r0ketlib/image.h>
#include <fatfs/ff.h>
#include <r0ketlib/fs_util.h>

// # from LED
#include <rad1olib/pins.h>
#include <libopencm3/lpc43xx/adc.h>

void show_menu() {
	lcdClear();
	lcdPrintln("Super Image");
	lcdPrintln("up:  view file");
	lcdPrintln("down: choose second image");
	lcdPrintln("right: start the loop!");
	lcdDisplay();
}

//# MENU img
void fox_menu() {
	char filename[FLEN];
	char filename2[FLEN];
	FATFS FatFs;
	FRESULT res;
	int so=58;

	show_menu();

	getInputWaitRelease();

	SETUPadc(LED4);
	while(1){
		switch(getInputWaitRepeat()){
			case BTN_UP:
				getInputWaitRelease();
				if(selectFile(filename,"LCD")<0){
					lcdPrintln("Select ERROR");
					lcdDisplay();
					getInputWait();
					return;
				};

				show_menu();
				// lcdShowImageFile(filename);
				break;
			case BTN_DOWN:
				getInputWaitRelease();
				if(selectFile(filename2,"LCD")<0){
					lcdPrintln("Select ERROR");
					lcdDisplay();
					getInputWait();
					return;
				};
				show_menu();
				break;
			case BTN_LEFT:
				break;
			case BTN_RIGHT:
				while(getInput() != BTN_LEFT) {
					if(adc_get_single(ADC0,ADC_CR_CH6)*2*330/1023 > 210) {
						lcdShowImageFile(filename);
						// lcdPrintln("HIGH");
						// lcdDisplay();
					} else {
						lcdShowImageFile(filename2);
						// lcdPrintln("LOW");
						// lcdDisplay();
					}
				}
				show_menu();
				break;
			case BTN_ENTER:
				lcd_select();
				lcdWrite(TYPE_CMD,0x3a); lcdWrite(TYPE_DATA,2);
				lcd_deselect();
				return;
		};
	};
};
