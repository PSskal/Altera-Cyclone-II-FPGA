
#define LEFT 0
#define RIGHT 1

#include "sys/alt_stdio.h"
#include "system.h"
#include "altera_avalon_lcd_16207_regs.h"
#include "altera_avalon_pio_regs.h"
#include "unistd.h"
#include "string.h"

#define LCD_WR_COMMAND_REG 0
#define LCD_RD_STATUS_REG 1
#define LCD_WR_DATA_REG 2
#define LCD_RD_DATA_REG 3
#define LCD_0_BASE 0x5200

unsigned char cuenta =0;
unsigned char centena =0;
unsigned char decena =0;
unsigned char unidad =0;
unsigned char addhora =8;

unsigned char botton = 0;
unsigned char botton2 = 0;
unsigned char botton3 = 0;

void convierte(char unsigned numero){
	centena=(numero % 1000)/100;
	decena = (numero%100)/10;
	unidad = numero%10;
}
void LCD_init(void);
void ESCRIBE_MENSAJE(const char *cadena);
void LCD_ENVIACHAR(unsigned char caracter);
void LCD_CLEAR(void);
void LCD_LINE1(void);
void LCD_LINE2(void);
void LCD_CUSTOM_CHAR(const unsigned char *vector, unsigned char pos);
void LCD_CHARVAR_SEND(unsigned char numero, unsigned char show_digit);
void LCD_POS_CURSOR(unsigned char fila, unsigned char columna);
void LCD_CURSOR_MOVE(unsigned char direction);

void cuenta_reloj(void);
volatile unsigned int minutos=0,segundos=0,horas=0;
int main()
{
  alt_putstr("Hello from Nios II!\n");
  alt_putstr("vamono pa' Chincha!\n");
  alt_putstr("\t 0_o!\n");

  	LCD_init(); // llamando funcion
  	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x02);
    usleep(2000);

    ESCRIBE_MENSAJE("Control 8 horas");

    while(1){
    	if(IORD_ALTERA_AVALON_PIO_DATA(0x3000)==1){

    		botton = IORD_ALTERA_AVALON_PIO_DATA(0x5000) & 0x01;
    		botton2 =IORD_ALTERA_AVALON_PIO_DATA(0x5000) & 0x02;
          	botton2 = botton2 >> 1;
          	botton3 =IORD_ALTERA_AVALON_PIO_DATA(0x5000) & 0x04;
          	//botton3 = botton2 >> 3;
          	volatile unsigned char nuevahora, nv_minuto;
          	if(botton==1){
          		horas=horas+1;
          		if(horas>23){
          			horas=horas-24;
          		}
    		    	}
          	else if(botton2==1){
          		minutos=minutos+1;
          		if(minutos>59){
          			minutos=minutos-60;
          		}
          	}
    		else if(botton3==4){
    			LCD_CLEAR();
    			LCD_LINE1();
    			ESCRIBE_MENSAJE("I:");
    			LCD_CHARVAR_SEND(horas, 2);
    			ESCRIBE_MENSAJE(":");
    			LCD_CHARVAR_SEND(minutos,2);
    			ESCRIBE_MENSAJE(" F:");
    			nuevahora=horas+addhora;
    			if(nuevahora>23){
    				nuevahora=nuevahora-24;
    			}
    			nv_minuto=minutos;
    			LCD_CHARVAR_SEND(nuevahora, 2);
    			ESCRIBE_MENSAJE(":");
    			LCD_CHARVAR_SEND(nv_minuto,2);
    	    	}

    		IOWR_ALTERA_AVALON_PIO_DATA(0x4000, 1);
        	cuenta_reloj();
        	LCD_LINE2();
        	ESCRIBE_MENSAJE("HORA: ");
        	LCD_CHARVAR_SEND(horas, 2);
        	ESCRIBE_MENSAJE(":");
        	LCD_CHARVAR_SEND(minutos,2);
        	ESCRIBE_MENSAJE(":");
        	LCD_CHARVAR_SEND(segundos,2);

        	while((horas==nuevahora && nv_minuto==minutos)&&IORD_ALTERA_AVALON_PIO_DATA(0x3000)==0){
        	   IOWR_ALTERA_AVALON_PIO_DATA(0x4000, 2); /// encender alarma
        	  }
    	}
    	else{
    		IOWR_ALTERA_AVALON_PIO_DATA(0x4000, 0x00);
    	}

    }

     return 0;

}

void cuenta_reloj(void){
	//volatile unsigned int clk_1hz=IORD_ALTERA_AVALON_PIO_DATA(0x3000);
	usleep(500000);
	//if(IORD_ALTERA_AVALON_PIO_DATA(0x3000) == 1){
		if(segundos==59){
			segundos=0;
			if(minutos==59){
				minutos=0;
				if(horas==23){
					horas=0;
				}
				else if(botton==1){
					horas=horas+1;
				}
				else{
					horas=horas+1;
				}
			}
			else if(botton2==0){
				minutos=minutos+1;
			}
			else{
				minutos=minutos+1;
			}
		}
		else{
			segundos=segundos+1;
		}
	//}
}


void LCD_init(void){
	usleep(15000);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x38);
	usleep(4100);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x38);
	usleep(100);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x38);
	usleep(5000);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x38);
	usleep(100);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x08);
	usleep(100);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x0C);
	usleep(100);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x06);
	usleep(100);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x02);
	usleep(2000);
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x01);
	usleep(2000);
}

void ESCRIBE_MENSAJE(const char *cadena){
	unsigned char tam;
	tam = strlen(cadena);
	unsigned char i=0;
	for(i=0; i<tam; i++){
		IOWR(LCD_0_BASE, LCD_WR_DATA_REG, cadena[i]);
		usleep(100);
	}
}
 void LCD_ENVIACHAR(unsigned char caracter){
	 IOWR(LCD_0_BASE, LCD_WR_DATA_REG, caracter);
	 usleep(100);

 }

 void LCD_CLEAR(void){
	 IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x01);
	 usleep(2000);
 }

void LCD_LINE1(void){
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x02);
	usleep(2000);
}

void LCD_LINE2(void){
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0xC0);
	usleep(2000);
}

void LCD_CUSTOM_CHAR(const unsigned char *vector, unsigned char pos){
	unsigned char i;
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x40+8*pos);
	for(i=0;i<8;i++){
		LCD_ENVIACHAR(vector[i]);
	}
	IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x80);
}

void LCD_CHARVAR_SEND(unsigned char numero, unsigned char show_digit){
	switch(show_digit){
	case 1:
		LCD_ENVIACHAR((numero%10)+0x30);
		break;
	case 2:
		LCD_ENVIACHAR(((numero%100)/10)+0x30);
		LCD_ENVIACHAR((numero%10)+0x30);
		break;
	case 3:
		LCD_ENVIACHAR(((numero%1000)/100)+0x30);
		LCD_ENVIACHAR(((numero%100)/10)+0x30);
		LCD_ENVIACHAR((numero%10)+0x30);
		break;

	}
}

void LCD_POS_CURSOR(unsigned char fila, unsigned char columna){
	if(fila==1){
		IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x80+columna);
	}
	else if(fila == 2){
		IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0xC0+columna);
	}
}

void LCD_CURSOR_MOVE(unsigned char direction){

	if(direction == LEFT){
		IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x10);
	}
	if(direction == RIGHT){
		IOWR(LCD_0_BASE, LCD_WR_COMMAND_REG, 0x14);
	}
}
