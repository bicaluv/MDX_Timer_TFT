#ifndef StateMachine_h
#define StateMachine_h


#define NUM_STATES        7

#define INITIAL_STATE     -1
#define TIMER0_STATE      0
#define TIMER1_STATE      1
#define TIMER2_STATE      2
#define PLAY_PAUSE_STATE  3
#define REC_STATE         4
#define UP_STATE          5
#define DOWN_STATE        6

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


typedef int (*transition)(int);
typedef void (*updateContext)(int);

class State {
public:
  State();
  
  void init(transition tf, updateContext uf);
  int handle(int event);
  
private:
  transition _transitionFunction;
  updateContext _updateFunction;
};


class StateMachine {
public:
  StateMachine();
  
  void init(int initialState);
  void addState(int id, transition tf, updateContext uf);
  int update(int newValue);
  
private:
  State _states[NUM_STATES];
  int _currentSate;
};

#endif
