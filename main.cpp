//#include <SFML/Network.hpp>
#include <ostream>
#include <iostream>

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/file.h>
#include <string>
#include <sys/io.h>
#include <cmath>
#include <string>

//dla sdla
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//SDL
SDL_Window      * Okno          = nullptr;
SDL_Texture     * Tekstura      = nullptr;
SDL_Texture     * Punkt         = nullptr;
SDL_Renderer    * Generator     = nullptr;

SDL_Rect          Pozycja_pkt{0,0,10,10};

const int SZEROKOSC = 900;
const int WYSOKOSC = 500;

//podstawowa konf SDLa i zmiennych globalnych
bool SDL_Konf();
bool SDL_Czyszczenie();
//koniec SDL

const double PI = (22/7);

int serial_port;
struct termios tty;

void konfiguracja();
void akcja();
double Radiany();

unsigned char read_buf [6]{};
unsigned char write_flag[6]{'0','0','0','0','0','0'};
int num_bytes_read;

//dystansowe sprawy
std::string dystans_s = "000";
std::string kat_s = "000";

int dystans_i = 0;
int kat_i = 0;

int pos_x = 0;
int pos_y = 0;
float sin_r = 0.0000;
float cos_r = 0.0000;




using namespace std;

int main()
{
    konfiguracja();

    //sprawdzenie czy udalo sie zrobic SDL-a
    if(SDL_Konf() == true)
    {
        cout << "Hejka" << endl;

        //aby się nam łądnie arduino przygotowało do pracki

        usleep(5000000);
        //petla dla akcji
        while(true)
        {
            akcja();
        }

    }

    close(serial_port);
    SDL_Czyszczenie();
    return 0;
}


bool SDL_Konf()
{
    if(SDL_Init(SDL_INIT_VIDEO) >= 0)
    {
        if(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG)
        {
            //stworzenie okna oraz generatora
            Okno = SDL_CreateWindow("RADAR",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SZEROKOSC,WYSOKOSC,SDL_WINDOW_SHOWN);
            Generator = SDL_CreateRenderer(Okno,-1,SDL_RENDERER_ACCELERATED);

            if(Okno == nullptr)
            {
                cout << "Okno nie zaladowane" << endl;
            }

            if(Generator == nullptr)
            {
                cout << "Generator nie zaladowany" << endl;
            }

            //zapelnienie tekstury oraz dodanie ją do okna
            SDL_Surface * Warstwa = IMG_Load("grey.png");

            if(Warstwa == nullptr)
            {
                cout << "Warstwa chwilowa nie zaladowana" << endl;
            }

                Tekstura = SDL_CreateTextureFromSurface(Generator,Warstwa);

            if(Tekstura == nullptr)
            {
                cout << "Tekstura nie zaladowana" << endl;
            }
            //zrobienie punktu
            Warstwa = IMG_Load("punkt.png");

            if(Warstwa == nullptr)
            {
                cout << "Warstwa chwilowa nie zaladowana" << endl;
            }

            Punkt = SDL_CreateTextureFromSurface(Generator,Warstwa);

            //pozbycie sie warstwy z pamieci
            SDL_FreeSurface(Warstwa);
            Warstwa = nullptr;

            //zaladowanie do
            SDL_RenderCopy(Generator,Tekstura,NULL,NULL);
            SDL_RenderPresent(Generator);

            return true;
        }
        else
        {
            cout << "SDL IMG nie zostal zainicjalizowany" << endl;
            return false;
        }
    }
    else
    {
        cout << "SDL nie zostal zainicjalizowany" << endl;
        return false;
    }
}

bool SDL_Czyszczenie()
{
    SDL_DestroyRenderer(Generator);
    SDL_DestroyWindow(Okno);
    Generator = nullptr;
    Okno = nullptr;

    SDL_DestroyTexture(Tekstura);
    Tekstura = nullptr;

    SDL_DestroyTexture(Punkt);
    Punkt = nullptr;

    IMG_Quit();
    SDL_Quit();

    return true;
}

void konfiguracja()
{
        serial_port = open("/dev/ttyACM0", O_RDWR, O_NONBLOCK); //niebedzie przerwana zadna opcja w tym pliku (zapis,odczyt itd przez sheulder)

        if(tcgetattr(serial_port, &tty) != 0)
        {
            cout << "Hejka cos sie popsulo i nie bylo mnie slychac..." << endl;
        }

        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
        tty.c_cflag |= CS8; // 8 bits per byte (most common)
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO; // Disable echo
        tty.c_lflag &= ~ECHOE; // Disable erasure
        tty.c_lflag &= ~ECHONL; // Disable new-line echo
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        tty.c_cc[VTIME] = 15;  //Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        tty.c_cc[VMIN] = 15;

        // Set in/out baud rate to be 9600
        cfsetispeed(&tty, B9600);

        // Save tty settings, also checking for error
        if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
        {
            cout << "Hejka Hejka znow cos nie dziala" << endl;
        }
        else
        {
            cout << "Połączono z Arduino maj friend ;)" << endl;
        }

}

void akcja()
{
        //odebranie z arduina wiadomosci
        write(serial_port,write_flag,6);
        num_bytes_read = read(serial_port, read_buf, sizeof(read_buf));


        /*
        for(int i=0;i<6;i++)
        {
            cout << read_buf[i];
        }
        cout << endl;
        */
        if(num_bytes_read == 6)
        {
            for(int i=3;i<6;i++)
            {
                dystans_s[i-3] = read_buf[i];
            }

            for(int i=0;i<3;i++)
            {
                kat_s[i] = read_buf[i];
            }


            //zmiana stringa na inty
            dystans_i = atoi(dystans_s.c_str());
            kat_i = atoi(kat_s.c_str());

            //zrobienie katow
            pos_x = (cos( Radiany() ) * dystans_i) * 6;
            pos_y = (sin( Radiany() ) * dystans_i) * 6;

            //resety itd
            if(kat_i == 0)
            {
                SDL_RenderClear(Generator);
                SDL_RenderCopy(Generator,Tekstura,NULL,NULL);
            }


            //ustawienie wartosci dla SDL_Rect
            Pozycja_pkt.y = 500 - pos_y;
            Pozycja_pkt.x = pos_x + 450;

             cout << "Dystans  = " << dystans_i << "\t kat_serwa = " << kat_i << "\t pos_x = " << Pozycja_pkt.x << "\t pos_y = " << Pozycja_pkt.y  << endl;

            SDL_RenderCopy(Generator,Punkt,NULL,&Pozycja_pkt);
            SDL_RenderPresent(Generator);
        }

        //cout << "Dystans  = " << read_buf[0] <<  read_buf[1] <<  read_buf[2]<< "\t kat_serwa = " << read_buf[3]<< read_buf[4] << read_buf[5]<< endl;
}

double Radiany()
{
    return (((kat_i) * PI) / 180);
}
