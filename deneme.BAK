#include "deneme.h"
#include "TOUCH.C"
#include "LCD.C"

#define eeprom_byte 16
#define bekleme 80
#define switch1	  pin_a0
#define switch2	  pin_a1
#define led_kirmizi pin_a2
#define led_yesil   pin_a3

BYTE Sil[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void eepromyaz(unsigned char adres,unsigned char buffer[8]){
         unsigned char i;
         for(i=8;i>0;i--){
              write_eeprom(adres++,buffer[i-1]);
         }
}

char eepromoku(unsigned char adres){
         unsigned char i;
         unsigned char bayt;
         lcd_putc("\f");
         printf(lcd_putc,"%X",adres);
         lcd_putc(":");
         for(i=3;i<7;i++){
         bayt=read_eeprom(8*adres+i);
         printf(lcd_putc,"%X",bayt);
         }
}

int1 eepromkarsilastir(unsigned char numara[8]){
unsigned char i,k,reg;
int sonuc;

reg=0;
sonuc=0;

for(i=0;i<eeprom_byte;i++){
if (read_eeprom(reg)==numara[7] & read_eeprom(reg+1)==numara[6] & read_eeprom(reg+2)==numara[5] & read_eeprom(reg+3)==numara[4] & read_eeprom(reg+4)==numara[3] & read_eeprom(reg+5)==numara[2] & read_eeprom(reg+6)==numara[1] & read_eeprom(reg+7)==numara[0] ) {
if (reg==0){
sonuc=2;
break;
}else{
sonuc=1;
break;
}
}
reg+=8;
}

return sonuc;
}


char bosyerayarla(void){
unsigned char i,reg,test;
int sonuc;

reg=0;
sonuc=8;

for(i=0;i<eeprom_byte;i++){

test = !read_eeprom(reg) | !read_eeprom(reg+1) | !read_eeprom(reg+2) | !read_eeprom(reg+3) | !read_eeprom(reg+4) | !read_eeprom(reg+5) | !read_eeprom(reg+6) | !read_eeprom(reg+7);
if (test==0) {
if (reg==0){
sonuc=9;
break;
}else{
sonuc=i;
break;
}
}
reg+=8;
}

return sonuc;
}

void yesil(void){
lcd_putc("\fTOM OK.");
output_bit(led_yesil,1);
delay_ms(1000);
output_bit(led_yesil,0);
reset_cpu();
}

void kirmizi(void){
lcd_putc("\fTOM Gecersiz!");
output_bit(led_kirmizi,1);
delay_ms(1000);
output_bit(led_kirmizi,0);
reset_cpu();
}

int secimbekle(void){
//buraya öyle bi kod yaz ki tuþu býraktýðýnda iþlem yapsýn.Basýlý olma özelliði olmasýn.
int i,secim,b1,b2;
secim=0;
for(i=0;i<bekleme;i++){
                  delay_ms(100);
				b1=input(switch1);
				b2=input(switch2);
            if (b1|b2){
            if(b1){
				for(;;){
				if(!input(switch1)){
				secim=1;
				break;
				}}}
            if(b2){
				for(;;){
				if(!input(switch2)){
				secim=2;
				break;
				}
				}
				}
            break;
                        }}

   return secim;
}

char butonoku(char buffer[8],char presenceCheck){
char i;
while(!touch_present()) ;
     delay_ms(200);
     if(touch_present()) {
        touch_write_byte(0x33);
        for(i=0;i<8;++i)
           buffer[i]=touch_read_byte();
        delay_ms(1000);

        presenceCheck = buffer[0] & buffer[1] & buffer[2] & buffer[3] & buffer[4] & buffer[5] & buffer[6] & buffer[7];
		  if(buffer[6]=0) presenceCheck=0xFF;
			return buffer;
			return presenceCheck;
}}

void main()
{
   unsigned CHAR buffer[8];
   int i,secim,secim2,k,l;
   unsigned CHAR presenceCheck,temp,masteryok,yer;

   port_b_pullups(TRUE);
	setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
	set_tris_b(0x08);
   lcd_init();

   lcd_putc("\fHAZIR.");
	masteryok=0;
	if(bosyerayarla()==0x09){
	lcd_putc("\fMaster Tom Kaydi\nTom'u dokundurun");
	masteryok=1;
	}

   while (TRUE) {


		butonoku(buffer,presenceCheck);

      if (presenceCheck != 0xFF)     // iButton dokundurulmuþsa
      {

			if(masteryok){
			eepromyaz(0,buffer);
			lcd_putc("\fDogrulama icin\ntekrar dokundur");
			butonoku(buffer,presenceCheck);
			for(i=0;i<bekleme;i++){
			delay_ms(100);
	      if (presenceCheck != 0xFF) {
			k = eepromkarsilastir(buffer);
			if(k==2){
			lcd_putc("\fKayit Onaylandi");
			masteryok=0;
			for(i=0;i<10;i++) delay_ms(100);
			reset_cpu();
			}else{
			lcd_putc("\fOnaylanmadi");
			for(i=0;i<8;i++) write_eeprom(i,0xFF);
			for(i=0;i<10;i++) delay_ms(100);
			reset_cpu();
			}
			}
			}
			reset_cpu();
			}



         if(eepromkarsilastir(buffer)==1|eepromkarsilastir(buffer)==2){
            if(eepromkarsilastir(buffer)==2){
            //Master Yetkili
            lcd_putc("\fMaster yetkili\n<-Islem   Kapi->");
            secim = secimbekle();
            switch(secim){
            case 0:
            //Seçim yapýlmamýþsa
            reset_cpu();
            break;

            //Seçim yapýlmýþsa
            case 1:
            //MASTER ISLEM'I SECTI
				islem:
            lcd_putc("\fIslem Seciniz\n<-Ekle     Sil->");
            secim2=secimbekle();
            switch(secim2){
            case 0:
            reset_cpu();
            break;
            case 1:
            //MASTER KAYIT'I SECTI
				yer = bosyerayarla();
				if(yer==0x08){						//Yer Yok
				lcd_putc("Bos Yer Yok!");
				delay_ms(1000);
				reset_cpu();
				}
				if(yer==0x09) reset_cpu();		// Master Yok nasý girmiþ?
				kayit:
				lcd_putc("\fYeni TOM'u\ndokundurun"); 			//Harici durumlar
            for(i=0;i<bekleme;i++){
            delay_ms(100);
				butonoku(buffer,presenceCheck);
				if(presenceCheck!=0xFF){
				if(eepromkarsilastir(buffer)!=0){
				lcd_putc("\fZaten Kayitli.");
				for(i=0;i<2;i++) delay_ms(1000);
				goto kayit;
				}else{
				eepromyaz(yer*0x08,buffer);
				lcd_putc("\fKaydedildi.");
				for(i=0;i<2;i++) delay_ms(1000);
				goto islem;
				}
				}

				}
            break;
            case 2:
            //MASTER SIL'I SECTI
            k=0;
            sil:
            eepromoku(k);
				lcd_putc("\n<-Sonraki  Sil->");
            for(i=0;i<bekleme;i++){
            delay_ms(100);
            if(input(switch1)){
				for(l=0;l<bekleme;l++){
				delay_ms(100);
				if(!input(switch1))
				goto arttir;
				}
				reset_cpu();
				arttir:
            if(k==eeprom_byte-1){
            k=0;}
            else{
            k+=1;}
            goto sil;
            }if(input(switch2)){
				for(l=0;l<bekleme;l++){
				delay_ms(100);
				if(!input(switch2)){
				//silme iþlemi yap
				printf(lcd_putc,"\f%X Emin misiniz?\n<-Evet   Hayir->",k);
				secim2 = secimbekle();
				switch(secim2){
				case 1:
				eepromyaz(k*0x08,Sil);
				lcd_putc("\fSilindi.");
				delay_ms(1000);
				if(k==0) reset_cpu();
				else goto islem;
				break;
				case 2:
				goto sil;
				break;
				}
				delay_ms(1000);
				}
				}}
				}
            reset_cpu();



            break;
            }
            break;
            case 2:
            //MASTER KAPIYI AC'I SECTI
            yesil();
            break;
            }}else
				{
				//KAPIYI AÇ
				yesil();
				}}
	         else{
   	      //KAYITLI DEGIL

        		kirmizi();
         	}


				}}}
