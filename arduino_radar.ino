#include <Servo.h>

Servo serwo;
unsigned long mili_S;
unsigned long mili_K;

unsigned char znak[6]{'0','0','0','0','0','0'};

// defines pins numbers
const short int trigPin = 9;
const short int echoPin = 10;

// defines variables
long duration;
short int distance;


int kat = 0;

bool strona = true; //true w prawo false w lewo

String napis;


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
    delayMicroseconds(2);
     
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    // Prints the distance on the Serial Monitor
  
    napis = static_cast<String>(distance);
    znak[0] = napis[0];
    znak[1] = napis[1];
    znak[2] = napis[2];
    
     if(strona == true)
     {
       kat++;
     }

     if(strona == false)
     {
       kat--;
     }
     
     if(kat >= 185)
     {
        strona = false;
     }

     if(kat <= 0)
     {
        strona = true;
     }
    napis = static_cast<String>(kat);
    
    znak[3] = napis[0];
    znak[4] = napis[1];
    znak[5] = napis[2];

    mili_K = millis();
    mili_K = mili_K - mili_S;
    
    Serial.write(znak,6);
    
    while(mili_K <= 30)
    {
      mili_K = millis();
      mili_K = mili_K - mili_S;
    }
  
    serwo.write(kat);
}
