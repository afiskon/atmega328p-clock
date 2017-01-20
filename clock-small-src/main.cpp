#include <Arduino.h>

/* inc/dec buttons */
const int HINC_PIN = A5;
const int HDEC_PIN = A4;
const int MINC_PIN = A3;
const int MDEC_PIN = A2;

/* pin to 4026 IC's reset inputs */
const int RESET_PIN = 5;

const int DOT_PIN = 8;

/* pins to 4026 IC clock inputs */
const int HPIN_H = 10;
const int HPIN_L = 9;
const int MPIN_H = 7;
const int MPIN_L = 6;

/* current time */
int hour = 0;
int min = 0;
int sec = 0;
int msec = 0;

bool hinc_released = true;
bool hdec_released = true;
bool minc_released = true;
bool mdec_released = true;

unsigned long prevMillis;

void setup()
{
  pinMode(HDEC_PIN, INPUT);
  pinMode(HINC_PIN, INPUT);
  pinMode(MDEC_PIN, INPUT);
  pinMode(MINC_PIN, INPUT);

  pinMode(RESET_PIN, OUTPUT);
  pinMode(DOT_PIN, OUTPUT);

  pinMode(HPIN_H, OUTPUT);
  pinMode(HPIN_L, OUTPUT);
  pinMode(MPIN_H, OUTPUT);
  pinMode(MPIN_L, OUTPUT);

  prevMillis = millis();
}

void displayTime()
{
  digitalWrite(RESET_PIN, HIGH);
  digitalWrite(RESET_PIN, LOW);

  digitalWrite(DOT_PIN, (sec & 1) ? HIGH : LOW);

  for(int i = 0; i < min % 10; i++)
  {
    digitalWrite(MPIN_L, HIGH);
    digitalWrite(MPIN_L, LOW);
  }

  for(int i = 0; i < min / 10; i++)
  {
    digitalWrite(MPIN_H, HIGH);
    digitalWrite(MPIN_H, LOW);
  }

  for(int i = 0; i < hour % 10; i++)
  {
    digitalWrite(HPIN_L, HIGH);
    digitalWrite(HPIN_L, LOW);
  }

  for(int i = 0; i < hour / 10; i++)
  {
    digitalWrite(HPIN_H, HIGH);
    digitalWrite(HPIN_H, LOW);
  }
}

void loop()
{
  unsigned long currMillis;
  unsigned long delta;

  delay(50);

  currMillis = millis();
  delta = currMillis - prevMillis;
  prevMillis = currMillis;
  msec += delta;

  while(msec >= 1000)
  {
    msec -= 1000;
    sec++;
    if(sec == 60)
    {
      sec = 0;
      min++;
      if(min == 60)
      {
        min = 0;
        hour++;
        if(hour == 24)
          hour = 0;
      }
    }
  }

  displayTime();

  if(digitalRead(HDEC_PIN) == LOW)
  {
    if(hdec_released)
    {
      hdec_released = false;
      hour--;
      if(hour < 0)
        hour = 23;
    }
  } else
    hdec_released = true;

  if(digitalRead(HINC_PIN) == LOW)
  {
    if(hinc_released)
    {
      hinc_released = false;
      hour++;
      sec = msec = 0;
      if(hour > 23)
        hour = 0;
    }
  } else
    hinc_released = true;

  if(digitalRead(MDEC_PIN) == LOW)
  {
    if(mdec_released)
    {
      mdec_released = false;
      min--;
      sec = msec = 0;
      if(min < 0)
        min = 59;
    }
  } else
    mdec_released = true;

  if(digitalRead(MINC_PIN) == LOW)
  {
    if(minc_released)
    {
      minc_released = false;
      min++;
      sec = msec = 0;
      if(min > 59)
        min = 0;
    }
  } else
    minc_released = true;
}
