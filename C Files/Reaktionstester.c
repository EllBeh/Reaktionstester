#define Taster 3
#define LEDrot 5
#define LEDblau 6
#define LEDgruen 7
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C anzeige(0x27, 16, 2);

const unsigned int voreinst = 3037;
int cnt = 0;    // Programmzähler (count)
int wd = 0;     // Interruptüberwachung aktiv (watchdog)
int f = 0;      // Fehlerschleife aktiv
int fcnt = 0;   // Fehlerzähler (failcount)
double zeit = 0.0;    //Reaktionszeit


void setup()
{
  anzeige.init();
  anzeige.backlight(); // leider defekt daher unbrauchbar
  anzeige.clear();
  lcdAusgabe("Taster", 0, 0);
  lcdAusgabe("betaetigen", 0, 1);

  // Konfiguration der I/O's
  DDRD  = (0 << Taster) ;
  PORTD = 0b00001000 ; // interner Pullup-Widerstand aktivieren
  DDRD =  (1 << LEDrot) | (1 << LEDblau) | (1 << LEDgruen);

  // Konfiguration external Interrupt 1
  SREG |= (1 << 7);
  EICRA |= (1 << ISC11) | (0 << ISC10);
  EIMSK &= ~(1 << INT1);

  // Konfiguration Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);  // Vorteiler 256
  TIMSK1 |= (1 << TOIE1) | (0 << OCIE1B) | (0 << OCIE1A) ; // OVF1-Interrupt zulassen
  TCNT1 = voreinst;

}

void loop()
{
  // Wenn Taster betätigt und Interruptüberwachung aus, dann Vorsequenz ausführen
  if ( (PIND & (1 << Taster) ) == 0 && wd == 0)
  {
    vorsequenz();
  }
  // Wenn Interruptüberwachung an ist dann, Ampel ausführen
  if ( wd == 1)
  {
    // Wenn Fehler erkannt wurde dann, Vorsequenz ausführen
    if (f == 1)
    {
      vorsequenz();
    }
    ampel(cnt);
  }
}
