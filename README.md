# Arduino SDL_radar 

### Description
Project to school to get the best mark at the end of the semester. 
- Here is a link to get a model of radar:
 https://cults3d.com/en/3d-model/various/echo-sensor-radar-case
- It works fine up to 1.5m.


### Requires
- Linux with desktop environment
- Your system has to have a global access to the SDL2 libs


### SetUp
- (PC) After pluged in your arduino to the computer, the full access (`sudo chmod 777 /dev/*arduino*`)to the file of the arduino should be granted
to easy write/read operations.
- In order to compile the program you have to use g++ `g++ -o Radar main.cpp -lSDL2 -lSDL2_image`
- Run the program by `./Radar.out`


- (Arduino) 
The layout of the Ultrasonic Distance Sensor - HC-SR04 is `trigPin = 9`; `echoPin = 10`;
The layout of the servo sg90 (and all simillar to it) is `serwo.attach(2)`;


### Limits
I know that all comments are in polish but I will change them in the future :)
