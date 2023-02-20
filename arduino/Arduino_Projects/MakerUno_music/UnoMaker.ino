
int piezoPin = 8;
int button = 2;
int lengths = 26;
char jbnotes[] = "eeeeeeegcde fffffeeeeddedg";
int jbbeats[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};
int length = 15;
char ttlsnotes[] = "ccggaag ffeeddc ggffeed ggffeed ccggaag ffeeddc ";
int ttlsbeats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2) 
  {
    digitalWrite(piezoPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(piezoPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) 
{
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  for (int i = 0; i < 8; i++) 
  {
    if (names[i] == note) 
    {
      playTone(tones[i], duration);
    }
  }
}

void setup() 
{
  pinMode(piezoPin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() 
{
  int sensorVal = digitalRead(button);
  if (sensorVal == HIGH)
  {
    for (int i = 0; i < length; i++) 
    {
      if (ttlsnotes[i] == ' ') 
      {
        delay(ttlsbeats[i] * tempo);
      } 
      else 
        playNote(ttlsnotes[i], ttlsbeats[i] * tempo);
      delay(tempo / 2); 
    }
  }
  else
  {
    for (int i = 0; i < lengths; i++) 
    {
      if (jbnotes[i] == ' ') 
      {
        delay(jbbeats[i] * tempo);
      } 
      else 
        playNote(jbnotes[i], jbbeats[i] * tempo);
      delay(tempo / 2); 
    }
  }
}