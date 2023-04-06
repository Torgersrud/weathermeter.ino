
#include <SDS011.h>//Importerer bibloteker sånn at språket kan forstå og interface med bmp og adafruit
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"


SDS011 mySDS;
Adafruit_BMP280 bmp;

float input_temp,input_hoyde, input_luft_kvalitet;
float snitt_temp, snitt_luft_kvalitet1, snitt_luft_kvalitet2, snitt_hoyde;
int i, error;
float temp_val;
float bmp_trykk, bmp_hoyde, bmp_temp;
float p10, p25;
float mikro_kvalitet, makro_kvalitet;


void setup() {
  // put your setup code here, to run once:
  bmp.begin();
  Serial.begin(9600);
  Serial.print("BMP temperatur, ");
  Serial.print("Luft kvalitet 1, ");
  Serial.print("Luft kvalitet 2, ");
  Serial.println("Trykk(bar)");
  mySDS.begin(2,3);
  i = 0;
  bmp_temp = 0;
  bmp_trykk = 0;
  error = 0;
  mikro_kvalitet = 0;
  makro_kvalitet = 0;
}

void loop() {
  i += 1;
  delay(5000);  // Forsikrer meg at vifta får blåst vekk alt som ikke trengs
  
  float p10, p25;
  bmp_temp += bmp.readTemperature(); //leser av temperatur fra bmp
  bmp_trykk += bmp.readPressure(); // ^
  bmp_trykk += bmp_trykk/100000; // bmp leser av i pascal, så jeg justerer tilbake til BAR

  error = mySDS.read(&p25, &p10); // SDS luftkvalitetsmålingene som måles, om "Read" er 0 eller "False" i boolean
                                  // Så vil error ikke leses, dette er for å unngå dobbelt målinger.  "&" viser adressen til en variabel, den viser hva verdien "pointer" til
                                  // Derfor så må man legge til "string(p10);" for å kunne printe verdiene, for å gi de en type
  if (error != false){
    mikro_kvalitet += p25;
    makro_kvalitet += p10;
  }
  
  
  Serial.print(snitt_resultat(i, bmp_temp),2);
  Serial.print(", ");
  Serial.print(String(snitt_resultat(i, makro_kvalitet)));
  Serial.print(", ");
  Serial.print(String(readBinaryString(snitt_resultat(i, mikro_kvalitet))));
  Serial.print(", ");
  Serial.println(snitt_resultat(i,bmp_trykk),2);

  Serial.print(i);
  
  delay(1000);

}
int snitt_resultat(int antall, float maaling){
  return maaling/antall;
}
int readBinaryString(char *s) {
  int result = 0;
  while(*s) {
    result <<= 1;
    if(*s++ == '1') result |= 1;
  }
  return result;
}
