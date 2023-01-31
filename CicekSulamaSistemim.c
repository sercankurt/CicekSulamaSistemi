// Otomatik Cicek Sulama Sistemi 
// Made by Sercan KURT - TA3KRT
// 
// pinler :  A2 ayar, A3++, A4--,
//           AN1 Toprak nem sensörü //--> ADC toprak nem eþik deðeri: 720
//           B0 su bitik sensörü,
//           B2 ds18b20 sýcaklýk sensörü,
//           B1 role su motoru,
//           B4 SDA 
//           B5 SCL
//           B6 nem led gösterge
#include <16F88.h>
#device ADC = 10
#fuses INTRC_IO, NOWDT, NOMCLR
#use delay(internal=8M)
#use i2c (Master, I2C1, sda=PIN_B4, scl=PIN_B5, STREAM = DS3231_STREAM, force_sw)
#include "i2c_Flex_lcd.c"
#include "ds3231.c"
#define ONE_WIRE_PIN PIN_B2// sensörün DQ bacaðýnýn baðlanacaðý pin
#include "ds18b20.c"
#use fast_io(a)
#use fast_io(b)
unsigned int ayar=0, s_saat=0, s_dakika=0, s_saniye_st=0, s_saniye_fn=0; //saat ayarý için
unsigned int16 nem=0;
float temp=0;
RTC_Time *mytime;

void rtc_print(){
  lcd_gotoxy(14, 1);
  switch(mytime->dow)
  {
    case SUNDAY   :  printf(lcd_putc, "SUN");  break;
    case MONDAY   :  printf(lcd_putc, "MON");  break;
    case TUESDAY  :  printf(lcd_putc, "TUE");  break;
    case WEDNESDAY:  printf(lcd_putc, "WED");  break;
    case THURSDAY :  printf(lcd_putc, "THU");  break;
    case FRIDAY   :  printf(lcd_putc, "FRI");  break;
    default       :  printf(lcd_putc, "SAT");
  }
  lcd_gotoxy(1,2);printf(lcd_putc,"%02d:%02d:%02d", mytime->hours, mytime->minutes, mytime->seconds);
  lcd_gotoxy(1,1);printf(lcd_putc,"%02d/%02d/20%02d", mytime->day, mytime->month, mytime->year);
}

void saatayari(){
   if (input(pin_a2)==1) {ayar++; delay_ms(10); while(input(pin_a2)); }
   if (ayar==7) { lcd_gotoxy(10,2); lcd_putc("set: Sn"); }
   if (ayar==6) { lcd_gotoxy(10,2); lcd_putc("set: Dk"); }
   if (ayar==5) { lcd_gotoxy(10,2); lcd_putc("set: Sa"); }
   if (ayar==4) { lcd_gotoxy(10,2); lcd_putc("set: Hf"); }
   if (ayar==3) { lcd_gotoxy(10,2); lcd_putc("set: Yl"); } 
   if (ayar==2) { lcd_gotoxy(10,2); lcd_putc("set: Ay"); }
   if (ayar==1) { lcd_gotoxy(10,2); lcd_putc("set: Gn"); }
   
   if (ayar==7 && input(pin_a3)==1) { mytime->seconds++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==7 && input(pin_a4)==1) { mytime->seconds--; RTC_Set(mytime); while(input(pin_a4)); }
   if (ayar==6 && input(pin_a3)==1) { mytime->minutes++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==6 && input(pin_a4)==1) { mytime->minutes--; RTC_Set(mytime); while(input(pin_a4)); }  
   if (ayar==5 && input(pin_a3)==1) { mytime->hours++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==5 && input(pin_a4)==1) { mytime->hours--; RTC_Set(mytime); while(input(pin_a4)); }
   if (ayar==4 && input(pin_a3)==1) { mytime->dow++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==4 && input(pin_a4)==1) { mytime->dow--; RTC_Set(mytime); while(input(pin_a4)); }
   if (ayar==3 && input(pin_a3)==1) { mytime->year++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==3 && input(pin_a4)==1) { mytime->year--; RTC_Set(mytime); while(input(pin_a4)); }
   if (ayar==2 && input(pin_a3)==1) { mytime->month++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==2 && input(pin_a4)==1) { mytime->month--; RTC_Set(mytime); while(input(pin_a4)); }
   if (ayar==1 && input(pin_a3)==1) { mytime->day++; RTC_Set(mytime); while(input(pin_a3)); }
   if (ayar==1 && input(pin_a4)==1) { mytime->day--; RTC_Set(mytime); while(input(pin_a4)); }
}

void sulamaayari(){
  if (ayar==8) { lcd_gotoxy(10,2); printf(lcd_putc,">%02d:%02d ", read_eeprom(0), read_eeprom(1) ); }
  if (ayar==9) { lcd_gotoxy(10,2); printf(lcd_putc," %02d:%02d<", read_eeprom(0), read_eeprom(1) ); }
  if (ayar==10){ lcd_gotoxy(10,2); printf(lcd_putc,">%02d/%02d ", read_eeprom(2), read_eeprom(3) ); }
  if (ayar==11){ lcd_gotoxy(10,2); printf(lcd_putc," %02d/%02d<", read_eeprom(2), read_eeprom(3) ); }
  if (ayar==12){ lcd_gotoxy(10,2); printf(lcd_putc,"   %lu   ", nem); }
  if (ayar==13){ayar=0;}
  if (ayar==0) {temp=ds1820_read(); lcd_gotoxy(11,2); printf(lcd_putc," %02.1f\xDFC ",temp);}
    
  if (ayar==8 && input(pin_a3)==1) { s_saat++; write_eeprom(0,s_saat); while(input(pin_a3)); }
  if (ayar==8 && input(pin_a4)==1) { s_saat--; write_eeprom(0,s_saat); while(input(pin_a4)); }
  if (ayar==9 && input(pin_a3)==1) { s_dakika++; write_eeprom(1,s_dakika); while(input(pin_a3)); }
  if (ayar==9 && input(pin_a4)==1) { s_dakika--; write_eeprom(1,s_dakika); while(input(pin_a4)); }
  if (ayar==10 && input(pin_a3)==1){ s_saniye_st++; write_eeprom(2,s_saniye_st); while(input(pin_a3)); }
  if (ayar==10 && input(pin_a4)==1){ s_saniye_st--; write_eeprom(2,s_saniye_st); while(input(pin_a4)); }
  if (ayar==11 && input(pin_a3)==1){ s_saniye_fn++; write_eeprom(3,s_saniye_fn); while(input(pin_a3)); }
  if (ayar==11 && input(pin_a4)==1){ s_saniye_fn--; write_eeprom(3,s_saniye_fn); while(input(pin_a4)); }
} 

void main(){
setup_adc(ADC_CLOCK_INTERNAL);
delay_us(10);
setup_adc_ports(sAN1);
delay_us(10);
set_adc_channel(1);
delay_us(10);
set_tris_b(0x01);
output_b(0x00);
delay_us(10);
lcd_init(0x4e,16,2);
lcd_putc("\f");
lcd_gotoxy(1,1); lcd_putc("Otomatik Sulama ");
lcd_gotoxy(1,2); lcd_putc(" Sistemi v1.0   "); 
delay_ms(500);
lcd_putc("\f");
lcd_gotoxy(1,1); lcd_putc(" Made by TA1AFS ");
lcd_gotoxy(1,2); lcd_putc("  SERCAN KURT   "); 
delay_ms(500);
lcd_clear(); 
mytime = RTC_Get(); 
IntSqw_Set(OUT_1Hz); 
s_saat=read_eeprom(0);  //sulama ayarý için kaydedilmiþ eeprom verileri okunuyor eþleþtiriliyor.
s_dakika=read_eeprom(1);
s_saniye_st=read_eeprom(2);
s_saniye_fn=read_eeprom(3);
  
while(TRUE){
mytime = RTC_Get();
rtc_print();
saatayari();
sulamaayari();
nem=read_adc();

   if   (mytime->hours   == s_saat && 
         mytime->minutes == s_dakika && 
         mytime->seconds >=  s_saniye_st && 
         mytime->seconds <= s_saniye_fn &&  
         nem <= 700 &&
         (input(pin_b0)==1)  /* Yazýn satýrý devre dýþý býrak &&
       // (mytime->dow == MONDAY || mytime->dow == WEDNESDAY || mytime->dow == FRIDAY || mytime->dow == SUNDAY) // Sulayacaðý günler  Yazýn satýrý devre dýþý býrak */
         )   
        { delay_ms(10); output_high(pin_b1);
         lcd_gotoxy(9,2); lcd_putc(" sulama "); delay_ms(500); lcd_gotoxy(9,2); lcd_putc("        "); delay_ms(500);
         lcd_gotoxy(9,2); lcd_putc(" aktif  "); delay_ms(500); lcd_gotoxy(9,2); lcd_putc("        "); delay_ms(500);}
   else {output_low(pin_b1);}
   
   if   (input(pin_b0)==0) 
        {lcd_gotoxy(9,2); lcd_putc("su bitti"); delay_ms(500); lcd_gotoxy(9,2); lcd_putc("        "); delay_ms(500);}

   if   (nem<=720 && (mytime->seconds==0 || mytime->seconds==30) )
        {output_high(pin_b6);delay_ms(10);output_toggle(pin_b6);}
   else {output_low(pin_b6);}
   
   if   (mytime->hours >= 00 && mytime->hours <= 07) {lcd_backlight_led(OFF);}
   else {lcd_backlight_led(ON);}
   
   if(s_saniye_st==59) s_saniye_st=-1;  //Sýnýrlandýrmalar
   if(s_saniye_fn==59) s_saniye_fn=-1;
   if(s_dakika==59)    s_dakika=-1;
   if(s_saat==23)      s_saat=-1;
   } 
}

// pinler :  A2 ayar, A3++, A4--,
//           AN1 Toprak nem sensörü,
//           B0 su bitik sensörü,
//           B2 ds18b20 sýcaklýk sensörü,
//           B1 role su motoru,
//           B4 SDA 
//           B5 SCL
//           B6 nem led gösterge



