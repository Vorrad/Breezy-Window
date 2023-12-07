/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 

#include "LCD_GFX.h"
#include "ST7735.h"
#include "uart.h"	// for debugging

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if ((pixels>>j)&1==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
	// Will fill the background automatically
	
	LCD_setAddr(x0 - radius, y0 - radius, x0 + radius, y0 + radius);
	clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
	
	for (int y = y0 - radius; y <= y0 + radius; y++){
		for (int x = x0 - radius; x <= x0 + radius; x++){
			if ((x - x0)*(x - x0) + (y - y0)*(y - y0) <= radius*radius){
				SPI_ControllerTx_16bit_stream(color);
			}
			else
			{
				SPI_ControllerTx_16bit_stream(0x0000);
			}
		}
	}
	
	set(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
}


/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0, short y0, short x1, short y1, uint16_t color) {
	// assert y1 > y0 and x1 > x0
	
	short is_steep = 0;
	float k = (float)(y1 - y0)/(x1 - x0);
	
	if (abs(k) > 1) // steep
	{
		is_steep = 1;
		k = 1/k;
	}
		  
	switch(is_steep){
		case 0: {	// draw along x-axis
			for (short x = x0; x <= x1; x++){
				short y = k * (x -x0) + y0 + 0.5;	// 0.5 for rounding
				LCD_drawPixel(x, y, color);
			}
			break;
		}
		case 1: {	// draw along y-axis
			for (short y = y0; y <= y1; y++){
				short x = k * (y -y0) + x0 + 0.5;	// 0.5 for rounding
				LCD_drawPixel(x, y, color);
			}
			break;
		}
	}
}




/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
	LCD_setAddr(x0, y0, x1, y1);
	
	clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
	
	for (int y=y0; y <= y1; y++)
		for (int x=x0; x <= x1; x++)
			SPI_ControllerTx_16bit_stream(color);
	
	set(LCD_PORT, LCD_TFT_CS);	//set CS to high	
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
	LCD_setAddr(0, 0, LCD_WIDTH, LCD_HEIGHT);
	
	clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
	
	for (int i = 0; i <= LCD_WIDTH; i++){
		for (int j = 0; j <= LCD_HEIGHT; j++){
			SPI_ControllerTx_16bit_stream(color);
		}
	}
	
	set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	int i, j, num;
	num = strlen(str);
	
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for (int c=0; c < num; c++){	
			char character = str[c];
			uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
			
			for(i=0;i<5;i++){
				uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
				for(j=0;j<8;j++){
					if ((pixels>>j)&1==1){
						LCD_drawPixel(x+i + c*5,y+j,fg);
					}
					else {
						LCD_drawPixel(x+i + c*5,y+j,bg);
					}
				}
			}
		}
	}
}


void LCD_drawCharLarge(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor, uint8_t scale) {
	uint16_t row = character - 0x20; // Determine row of ASCII table starting at space
	int i, j, k, l;

	if ((LCD_WIDTH - x > 7 * scale) && (LCD_HEIGHT - y > 7 * scale)) {
		for (i = 0; i < 5; i++) {
			uint8_t pixels = ASCII[row][i]; // Go through the list of pixels
			for (j = 0; j < 8; j++) {
				for (k = 0; k < scale; k++) {       // Scale in width
					for (l = 0; l < scale; l++) {   // Scale in height
						if ((pixels >> j) & 1 == 1) {
							LCD_drawPixel(x + i * scale + k, y + j * scale + l, fColor);
							} else {
							LCD_drawPixel(x + i * scale + k, y + j * scale + l, bColor);
						}
					}
				}
			}
		}
	}
}

void LCD_drawStringLarge(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg, uint8_t scale) {
	int length = strlen(str);
	int charWidth = 5 * scale+1; // Width of each character after scaling

	for (int i = 0; i < length; i++) {
		LCD_drawCharLarge(x, y, str[i], fg, bg, scale);
		x = x + charWidth; // Move to the position for the next character
	}
}