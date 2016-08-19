#include "UI.h"


/*
Rec : Drücken bringt Links/Rechts zur Anzeige und zum Einstellen des ausgewählten Timers. Erneutes Drücken speichert den geänderten Wert unter dem ausgewählten Timer.
Play/Pause : Startet den Timer. Erneutes drücken pausiert den Timer. Erneutes drücken macht weiter. Zum Stoppen muss Timer 0 bis 2 gedrückt werden.
Timer 1 : Wählt den Timer aus.
Timer 2 : Wählt den Timer aus.
Links / Weniger : Wenn Rec aktiv ist, dann wird der Timer reduziert. Hier gibt es auch einen Dauerhold.
Rechts / Mehr : Wenn Rec aktiv ist, dann wird der Timer erhöht. Hier gibt es auch einen Dauerhold.

UI enthält keinen Zustandsautomaten!!
Es gibt Buttons mit Größe, Form/Darstellung und einer Funktion, die bei Touch ausgeführt wird.
*/

#include <Adafruit_GFX.h>    // Core graphics library

#if defined(_GFXFONT_H_)           //are we using the new library?
//#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#define ADJ_BASELINE 11            //new fonts setCursor to bottom of letter
#else
#define ADJ_BASELINE 0             //legacy setCursor to top of letter
#endif

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.


// most mcufriend shields use these pins and Portrait mode:
uint8_t YP = A3;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 9;   // can be a digital pin
uint8_t XP = 8;   // can be a digital pin
uint8_t SwapXY = 0;

uint16_t TS_LEFT = 906;
uint16_t TS_RT  = 149;
uint16_t TS_TOP = 913;
uint16_t TS_BOT = 142;

//TS_LEFT = 920; TS_RT = 139; TS_TOP = 944; TS_BOT = 150;

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 325);
TSPoint tp;

#define MINPRESSURE 20
#define MAXPRESSURE 1000

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}

int16_t BOXSIZE;
int16_t PENRADIUS = 3;
uint16_t identifier, oldcolor, currentcolor = BLUE;
uint8_t Orientation = 1;    //Landscape


UI::UI() {
}

void UI::init(int *timers) {
    tft.begin(0x9341);

    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);


#if defined(_GFXFONT_H_)
    tft.setFont(&FreeSans18pt7b);
#endif

    tft.fillScreen(BLACK);

    int BOXSIZE = tft.width() / 6;

    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);



    tft.setTextSize(1 );
    tft.setCursor(50,50);
    tft.print(F("Hello!"));

  uint16_t tmp;

    switch (Orientation) {      // adjust for different aspects
        case 0:   break;        //no change,  calibrated for PORTRAIT
        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
    }
    
}

int UI::update() {
      uint16_t xpos, ypos;  //screen coordinates
    tp = ts.getPoint();   //tp.x, tp.y are ADC values

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!

    
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        // is controller wired for Landscape ? or are we oriented in Landscape?
        if (SwapXY != (Orientation & 1)) SWAP(tp.x, tp.y);
        // scale from 0->1023 to tft.width  i.e. left = 0, rt = width
        // most mcufriend have touch (with icons) that extends below the TFT
        // screens without icons need to reserve a space for "erase"
        // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
        xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
        ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());


          if (xpos > _buttons[0].pos_x && xpos < (_buttons[0].pos_x + _buttons[0].width) && ypos > _buttons[0].pos_y && ypos < (_buttons[0].pos_y + _buttons[0].height)) {
            _buttons[0].callback();
          }
          
        // are we in drawing area ?
        else if (((ypos - PENRADIUS) > BOXSIZE) && ((ypos + PENRADIUS) < tft.height())) {
            tft.fillCircle(xpos, ypos, PENRADIUS, currentcolor);
        }
        else {

        }

    }

    return 0;
}

void UI::refresh(int newVlaue) {
  
}

void UI::showSplashScreen(String name, String version) {
  tft.print(name);
  tft.print(version);
  delay(2000);
 tft.fillScreen(BLACK);
}

/*
void timer1Btn() {
  DEBUG_PRINTLN("pressed 1");
}

  ButtonType timer1;
  timer1.pos_x = 120;
  timer1.pos_y = 10;
  timer1.width = 160;
  timer1.height = 40;
  timer1.callback = timer1Btn;

  ui.addButton(timer1);
  */
void UI::addButton(ButtonType newBtn) {

_buttons[0] = newBtn;

      tft.fillRect(newBtn.pos_x, newBtn.pos_y, newBtn.width, newBtn.height, GREEN);

          tft.fillTriangle(
            100    , 100, // peak
            160, 160, // bottom left
            200, 160, // bottom right
            YELLOW);
}

