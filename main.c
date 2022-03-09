#include <main.h>


void test_diodes(int val_max, int val_min, int true_val, int ech_aff){
   if((true_val > val_max) || (true_val < val_min)){
      // Dans un cas d'erreur
      if(ech_aff == 0){
         output_e(0b010);
      }
      else{
         output_e(0b000);
      }
      // comme ech_aff oscille entre 0 et 1 en foction de l'afficheur actif,
      // une fois sur deux la led est allumée
   }
   else{
      //Dans le cas ou la valeur mesurée est entre les deux valeurs extrêmes
      // led verte allumée
      output_e(0b001);
   }
}


int afficheurs(int true_val, int ech_aff){
// Fonctiongérant les afficheurs 7 segments

   
   int val_point = 0b00000000; // valeur de base -# on affiche pas de point
   int val_affich = 0b00010000; // valeur de base, sera d'office changée'
   
   //échange à chaque affichage -X alternance des afficheurs
   if (ech_aff == 0){//aficheur 1 -> plus grand
      ech_aff = 1;
      val_affich = 0b00010000; //active le premier afficheur
      
      if (true_val >= 100){
         val_point = 0b00100000; // binaire pour allumer le point 
         true_val = true_val/100; //division entière pour seulement avoir le premier chiffre
         
      }
      else{
         true_val = true_val/10;
      }
      
   }
   else{//afficheur 2 -# plus petit
      ech_aff = 0;
      val_affich = 0b01000000; //active le deuxième afficheur
      
      if (true_val >= 100){
         true_val = (true_val/10)%10; //on prends le deuxième chiffre des trois
      }
      else{
         true_val = true_val%10;
      }
   }
   
   delay_ms(150); //enlever IRL ?
   output_b(true_val | val_affich | val_point);
   //output_b(0b00011000);
   // OU logique des différents nombres binaires. Pas sur que ça fonctionne, val_affich et val_point -> int
   // alors que val -> tableau de int
   return ech_aff;
}



void main()
{
   setup_low_volt_detect(FALSE);
   
   set_tris_b(01111111);
   set_tris_e(000);
   int ech_aff = 0;

   while(TRUE)
   {
      int val_mesuree = 130;
      int val_max = 120;
      int val_min = 60;
      test_diodes(val_max, val_min, val_mesuree, ech_aff);
      
      ech_aff = afficheurs(val_mesuree, ech_aff);
      

      //TODO: User Code
   }
   
}


   
   



