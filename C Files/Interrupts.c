// Funktion zum Ausschalten des externen Interruppt 1
void interOff()
{
  EIMSK &= ~(1 << INT1);
}

// Funktion zum Einschalten des externen Interruppt 1
void interOn()
{
  EIMSK |= (1 << INT1);
}

// Interrupt Service Routine externer Interrupt 1
ISR(INT1_vect)
{
  interOff();
  delay(50);
  if  (cnt == 3)
  {
    zeit = TCNT1;
  }
  if (cnt < 3)
  {
    TCNT1 = 65535;
    cnt = 5;
  }
}

// Interrupt Service Routine Timer 1
ISR(TIMER1_OVF_vect)
{
  TCNT1 = voreinst;

  // Wenn die Tasterüberwachung aktiv ist, dann Programmzähler inkrementieren
  if (wd == 1)
  {
    cnt++;
  }
}
