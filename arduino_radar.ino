#include <Servo.h>

//zmienne serwa
Servo serwo;
bool strona = true; //true w prawo false w lewo
short int kat = 0;

//zmiene czasowe
unsigned long mili_S;
unsigned long mili_K;

//piny sensora
const short int trigPin = 9;
const short int echoPin = 10;

//zmienne dotyczace pomiarow
long czas_trwania;
short int dystans;

//zmiene dotyczace transmisji
int dlugosc_stringu[2]{};
char znak[6]{'0','0','0','0','0','0'};
String napis[2];
int roznica = 0;

void setup() 
{
  serwo.attach(2);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}


void loop() 
{
  mili_S = millis();

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(15);
     
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
    
  // Reads the echoPin, returns the sound wave travel time in microseconds
  czas_trwania = pulseIn(echoPin, HIGH);
    
  // Calculating the distance
  dystans = czas_trwania*0.034/2;

  mili_K = millis();
  mili_K = mili_K - mili_S;

  while(mili_K <= 30)
  {
    mili_K = millis();
    mili_K = mili_K - mili_S;
  }
  

  //sterowanie serwem 
  if(kat < 0)
  {
    strona = true;
    kat = 0;
  }
  else if( kat > 180)
  {
    strona = false;
    kat = 180;
  }

  if(strona)
  {
    kat++;
  }
  else
  {
    kat--;
  }

  //zerowanie ramki
  for(short i=0;i<6;i++)
  {
    znak[i] = '0';
  }

  //przygotowanie ramki do wysłania do kompa
  napis[1] = String(dystans);
  dlugosc_stringu[1] = napis[1].length();

  napis[0] = String(kat); 
  dlugosc_stringu[0] = napis[0].length();

  roznica = 1;
  for(int i=2;i >= 3-dlugosc_stringu[0];i--)
  {
    znak[i] = napis[0][dlugosc_stringu[0]-roznica];

    roznica++;
  }

  roznica = 1;
  for(int i=5;i >= 6-dlugosc_stringu[1]; i--)
  {
    znak[i] = napis[1][dlugosc_stringu[1]-roznica];

    roznica++;
  }

  //wysłanie ramki gdy dostanie takie info że trzeba
  if(Serial.available())
  {
    Serial.write(znak,6);
  }
}