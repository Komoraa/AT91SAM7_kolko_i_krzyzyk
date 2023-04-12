//kolko i krzyzyk
#include <targets/AT91SAM7.h>
#include "lcd.h"

#define LCD_BACKLIGHT PIOB_SODR_P20 
#define AUDIO_OUT PIOB_SODR_P19
#define SW_1 PIOB_SODR_P24
#define SW_2 PIOB_SODR_P25

__attribute__ ((section(".fast")))
void delay(int n) //procedura opoznienia 
{
  volatile int i;

  for(i=3000*n;i>0;i--)
  {
    __asm__("nop");
  }
}
void rysuj_znak (char kolko_i_krzyzyk[3][3],int x, int y){ //funkcja rysuj�ca konkretetne miejsce w tablicy
  if(kolko_i_krzyzyk[x][y]=='x'){ 
    LCDPutChar('x',y*20+40,x*20+40,SMALL,WHITE,BLACK);
  }
  if(kolko_i_krzyzyk[x][y]=='o'){
    LCDPutChar(' ',y*20+40,x*20+40,SMALL,WHITE,BLACK); //usuwanie tla
    LCDSetCircle(y*20+40,x*20+40,5,WHITE);
  }
  if(kolko_i_krzyzyk[x][y]=='_'){
    LCDPutChar('_',y*20+40,x*20+40,SMALL,WHITE,BLACK);
  }
}

void rysuj_tablice(char kolko_i_krzyzyk[3][3]){ //funkcja rysuj�ca ca�� tablic�
LCDClearScreen();
for(int i=0;i<3;i++){
      for(int j=0;j<3;j++){
        rysuj_znak(kolko_i_krzyzyk,j,i);
        }
    }
}



int main()
{
  PMC_PCER=PMC_PCER_PIOB; //wlaczenie urzadzenia we/wy PIOB
  PIOB_OER=LCD_BACKLIGHT | AUDIO_OUT; //OUTPUT ENABLE
  PIOB_PER=LCD_BACKLIGHT | AUDIO_OUT; //PIO ENABLE 
  PMC_PCER=PMC_PCER_PIOA; // utuchomienie portu a

  InitLCD(); //inicjalizacja LCD
  LCDSettings(); //ustawienie LCD
  LCDClearScreen();
  
while(1){ //p�tla gry
  LCDClearScreen();
  char ster='o'; //ustawienie aktualnego gracza jako k�ko 
  int i=0;
  char kolko_i_krzyzyk[3][3] = {{'_','_','_'},{'_','_','_'},{'_','_','_'}}; //inicializacja pustej tablicy gry
  rysuj_tablice(kolko_i_krzyzyk); //narysowanie tablicy

  while(1){
    int x=0; //ustawienie pozycji wskaznika x na 0
    int y=0; //ustawienie pozycji wskaznika y na 0
    while(1){
     if( ((PIOA_PDSR & PIOA_SODR_P14 ) == 0) && x<2){  //if przesuwajacy wskaznik w prawo
       rysuj_znak(kolko_i_krzyzyk,x,y); //usuniecie wska�nika z poprzedniej pozycji
       x+=1;
       delay(1000);
      }

      if( ((PIOA_PDSR & PIOA_SODR_P9 ) == 0) && y>0){  //if przesuwajacy wskaznik w gore
        rysuj_znak(kolko_i_krzyzyk,x,y);
        y-=1;
        delay(1000);
      }

      if( ((PIOA_PDSR & PIOA_SODR_P8 ) == 0) && y<2){  //if przesuwajacy wskaznik w dol
        rysuj_znak(kolko_i_krzyzyk,x,y);
        y+=1;
        delay(1000);
      }

      if( ((PIOA_PDSR & PIOA_SODR_P7 ) == 0) && x>0){  //if przesuwajacy wskaznik w lewo
        rysuj_znak(kolko_i_krzyzyk,x,y);
        x-=1;
        delay(1000);
      }
    
      if( (PIOA_PDSR & PIOA_SODR_P15 ) == 0 && kolko_i_krzyzyk[x][y]=='_'){  // if sprawdzajacy czy guzik byl nacisniety czy nie
        kolko_i_krzyzyk[x][y]=ster;
        rysuj_znak(kolko_i_krzyzyk,x,y);
        delay(1000);
        i++;
        break;
      }
      LCDPutChar('#',y*20+40,x*20+40,SMALL,WHITE,BLACK);
    }

    if(kolko_i_krzyzyk[x][0]==ster && kolko_i_krzyzyk[x][1]==ster && kolko_i_krzyzyk[x][2]==ster){ //sprawdzdnie wygranej w kolumnie
      LCDPutStr("Wygrales",20,20,SMALL,WHITE,BLACK);
      delay(2000);
      break;
    }

    if(kolko_i_krzyzyk[0][y]==ster && kolko_i_krzyzyk[1][y]==ster && kolko_i_krzyzyk[2][y]==ster){ //sprawdzdnie wygranej w wierszu
      LCDPutStr("Wygrales",20,20,SMALL,WHITE,BLACK);
      delay(2000);
      break;
    }

    if(kolko_i_krzyzyk[0][0]==ster && kolko_i_krzyzyk[1][1]==ster && kolko_i_krzyzyk[2][2]==ster){ //sprawdzdnie wygranej na przekatnej
      LCDPutStr("Wygrales",20,20,SMALL,WHITE,BLACK);
      delay(2000);
      break;
    }

    if(kolko_i_krzyzyk[0][2]==ster && kolko_i_krzyzyk[1][1]==ster && kolko_i_krzyzyk[2][0]==ster){ //sprawdzdnie wygranej na przekatnej
      LCDPutStr("Wygrales",20,20,SMALL,WHITE,BLACK);
      delay(2000);
      break;
    }

    if(i==9){//petla sprawdzajaca remis
      LCDPutStr("Remis",20,20,SMALL,WHITE,BLACK);
      delay(2000);
      break;
    }
    
    if(ster=='o') //zmiana aktualnego gracza
      ster='x';
    else
      ster='o';
    
  }
  
  }
  return 0;
}
