#include <main.h>
#include <stdio.h>
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS)

#INT_RTCC
void  RTCC_isr(void) 
{

}

char *debut_data = "#";
char *data[4];
int flag_suivant = 0;  
int num_donner = 0;

#INT_RDA
void  RDA_isr(void) // reception et decoupe des donnees
{
   data[num_donner] = getc();
	if (strcmp(debut_data, data[0])){
		if (flag_suivant == 0){  
			num_donner ++;
			if (num_donner >= 4){    
				num_donner = 0;
				flag_suivant = 1;
			 }
		}
	}
}

void test_diodes(int val_max, int val_min, int true_val, int ech_aff){
   if((true_val > val_max) || (true_val < val_min)){
      
      printf("ALARM_IN %d",true_val);
      output_toggle(PIN_E1);
   }
   else{
      printf("ALARM_OUT %d",true_val);
      output_high(PIN_E0);
   }
}

int afficheurs(int true_val, int ech_aff){


   
   int val_point = 0b00000000; // valeur de base -# on affiche pas de point
   int val_affich = 0b00010000; // valeur de base, sera d'office changee'
   
   //echange a chaque affichage -X alternance des afficheurs
   if (ech_aff == 0){//aficheur 1 -> plus grand
      ech_aff = 1;
      val_affich = 0b00010000; //active le premier afficheur
      
      if (true_val >= 100){
         val_point = 0b00100000; // binaire pour allumer le point 
         true_val = true_val/100; //division entiere pour seulement avoir le premier chiffre
         
      }
      else{
         true_val = true_val/10;
      }
      
   }
   else{//afficheur 2 -# plus petit
      ech_aff = 0;
      val_affich = 0b01000000; //active le deuxieme afficheur
      
      if (true_val >= 100){
         true_val = (true_val/10)%10; //on prends le deuxieme chiffre des trois
      }
      else{
         true_val = true_val%10;
      }
   }
   
   delay_ms(150); //enlever IRL ?
   output_b(true_val | val_affich | val_point);
   //output_b(0b00011000);
   // OU logique des differents nombres binaires. Pas sur que ca fonctionne, val_affich et val_point -> int
   // alors que val -> tableau de int
   return ech_aff;
}

void main()
{
   setup_timer_0(RTCC_INTERNAL);      //409 us overflow


   enable_interrupts(INT_RTCC);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   setup_low_volt_detect(FALSE);
   int ech_aff = 0;
   long duration;
   int16 val_mesuree;
   int val_max = 120;
   int val_min = 0;
   
   while(TRUE)
   {
	  
      if(flag_suivant){ // tranforme les donnee en int et reconstruit le nombre 
	 int16 num1 = data[1] - 48;
         int16 num2 = data[2] - 48;
         int16 num3 = data[3] - 48;
         val_max = (int16)((num1*100)+(num2*10)+num3);
         flag_suivant = 0;
      }
      
      output_high(PIN_C0);
      delay_us(300); 
      output_low(PIN_C0);
      
      while(!input(PIN_C1)){}
      set_timer0(0);

      
      while(input(PIN_C1)){}
      duration = (long)get_timer0(); 
      val_mesuree = (int16)(duration / 145); // calcule de la distance
      
      if (val_mesuree >= 1000){ 
         output_b(0);
         output_high(PIN_B5); 

      }
      else{
         ech_aff = afficheurs(val_mesuree, ech_aff);
      }
      test_diodes(val_max, val_min, val_mesuree, ech_aff);
 
   }

}
