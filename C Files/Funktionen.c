// Leuchtmuster zum "bereit halten" ausgeben und Tasterüberwachung einschalten
void vorsequenz()
{
  PORTD &= ~(1 << LEDrot);
  PORTD &= ~(1 << LEDblau);
  PORTD &= ~(1 << LEDgruen);
  anzeige.clear();
  lcdAusgabe("Bereit", 0, 0);
  lcdAusgabe("halten!", 0, 1);
  PORTD |= (1 << LEDrot) | (1 << LEDgruen);
  delay(500);
  PORTD |= (1 << LEDrot);
  PORTD |= (1 << LEDgruen);
  delay(1000);
  PORTD &= ~(1 << LEDrot);
  PORTD &= ~(1 << LEDblau);
  PORTD &= ~(1 << LEDgruen);
  anzeige.clear();
  lcdAusgabe("3", 0, 0);
  wd = 1;
  f = 0;
  TCNT1 = 65535; // setzen des Zählregisters um Overflow zu triggern. So geht es nach Ablauf der Zeit direkt los
  interOn();
}

void ampel(int tmp)
{
  // Durchlauf der Ampelphasen und des Reaktionstests
  switch (tmp)
  {
    // Ampel rot und Tasterüberwachung ein
    case 1:
      PORTD |= (1 << LEDrot);
      lcdAusgabe("2", 0, 0);
      break;

    // Ampel gelb
    case 2:
      PORTD |= (1 << LEDrot) | (1 << LEDgruen);
      lcdAusgabe("1", 0, 0);
      break;

    // Ampel grün
    case 3:
      PORTD &= ~(1 << LEDrot);
      PORTD |= (1 << LEDgruen);
      lcdAusgabe("LOS", 0, 0);
      break;

    // Ampel aus, Tasterüberwachung deaktivieren und Reaktionszeit ausgeben
    case 4:
      PORTD &= ~(1 << LEDgruen);
      interOff();
      anzeige.clear();
      anzeige.setCursor(0, 0);
      anzeige.print(zeitBerechnung(zeit)); // double kann nicht übergeben werden an lcdAusgabe(), es macht auch keinen Sinn dafür eine extra Funktion zu schreiben
      lcdAusgabe("ms");
      f = 0;
      cnt = 0;
      wd = 0;
      delay(2000);
      anzeige.clear();
      lcdAusgabe("Taster", 0, 0);
      lcdAusgabe("betaetigen", 0, 1);
      break;

    // Fehler erkannt, Fehlersequenz ausgeben und neustarten wenn keine 5 Fehlversuche erreicht sind
    case 6:
      interOff();
      fehlersequenz();
      f = 1;
      cnt = 0;
      wd = 1;
      fcnt++;
      if (fcnt == 5 )
      {
        f = 0;
        cnt = 0;
        wd = 0;
        fcnt = 0;
        anzeige.clear();
        lcdAusgabe("ENDE", 0, 0);
        delay(1000);
        anzeige.clear();
        lcdAusgabe("Taster", 0, 0);
        lcdAusgabe("betaetigen", 0, 1);
      }
      break;
  }
}

// Fehler wurde erkannt und wird hier angezeigt durch Ausgabe am LCD und weißem Blinken der LED
void fehlersequenz()
{
  lcdAusgabe("!!FEHLER!!", 0, 0);
  lcdAusgabe("Nr ", 0, 1);
  anzeige.print(fcnt + 1); // Hier das Selbe wie bei der Zeitberechnung
  lcdAusgabe("/5");
  PORTD &= ~(1 << LEDrot);
  PORTD &= ~(1 << LEDblau);
  PORTD &= ~(1 << LEDgruen);

  // LED toggeln
  for ( int m = 0; m < 6 ; m++)
  {
    PORTD ^= (1 << LEDrot);
    PORTD ^= (1 << LEDblau);
    PORTD ^= (1 << LEDgruen);
    delay(200);
  }
}

// Berechnung der Zeit
double zeitBerechnung(double val)
{
  return ((val - 3037) * 0.016);
}

// Hilfsfunktion der LCD-Ausgabe um Code zu sparen
void lcdAusgabe(String text, int spalte, int zeile)
{
  anzeige.setCursor(spalte, zeile);
  anzeige.print(text);
}

// Hilfsfunktion der LCD-Ausgabe um Code zu sparen
void lcdAusgabe(String text)
{
  anzeige.print(text);
}
