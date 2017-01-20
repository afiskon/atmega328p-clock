#include <Arduino.h>

/* inc/dec buttons */
const int HDEC_PIN = A0;
const int HINC_PIN = A1;
const int MDEC_PIN = A2;
const int MINC_PIN = A3;
const int SDEC_PIN = A4;
const int SINC_PIN = A5;

/* pin to 4026 IC's reset inputs */
const int RESET_PIN = 8;

/* pins to 4026 IC clock inputs */
const int HPIN_H = 7;
const int HPIN_L = 6;
const int MPIN_H = 5;
const int MPIN_L = 4;
const int SPIN_H = 3;
const int SPIN_L = 2;

/* current time */
int hour = 0;
int min = 0;
int sec = 0;
int msec = 0;

bool hinc_released = true;
bool hdec_released = true;
bool minc_released = true;
bool mdec_released = true;
bool sinc_released = true;
bool sdec_released = true;

unsigned long prevMillis;

void setup()
{
  pinMode(HDEC_PIN, INPUT);
  pinMode(HINC_PIN, INPUT);
  pinMode(MDEC_PIN, INPUT);
  pinMode(MINC_PIN, INPUT);
  pinMode(SDEC_PIN, INPUT);
  pinMode(SINC_PIN, INPUT);

  pinMode(RESET_PIN, OUTPUT);

  pinMode(HPIN_H, OUTPUT);
  pinMode(HPIN_L, OUTPUT);
  pinMode(MPIN_H, OUTPUT);
  pinMode(MPIN_L, OUTPUT);
  pinMode(SPIN_H, OUTPUT);
  pinMode(SPIN_L, OUTPUT);

  prevMillis = millis();
}

void displayTime()
{
  digitalWrite(RESET_PIN, HIGH);
  digitalWrite(RESET_PIN, LOW);

  for(int i = 0; i < sec % 10; i++)
  {
    digitalWrite(SPIN_L, HIGH);
    digitalWrite(SPIN_L, LOW);
  }

  for(int i = 0; i < sec / 10; i++)
  {
    digitalWrite(SPIN_H, HIGH);
    digitalWrite(SPIN_H, LOW);
  }

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

  if(digitalRead(HDEC_PIN) == HIGH)
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

  if(digitalRead(HINC_PIN) == HIGH)
  {
    if(hinc_released)
    {
      hinc_released = false;
      hour++;
      if(hour > 23)
        hour = 0;
    }
  } else
    hinc_released = true;

  if(digitalRead(MDEC_PIN) == HIGH)
  {
    if(mdec_released)
    {
      mdec_released = false;
      min--;
      if(min < 0)
        min = 59;
    }
  } else
    mdec_released = true;

  if(digitalRead(MINC_PIN) == HIGH)
  {
    if(minc_released)
    {
      minc_released = false;
      min++;
      if(min > 59)
        min = 0;
    }
  } else
    minc_released = true;

  if(digitalRead(SDEC_PIN) == HIGH)
  {
    if(sdec_released)
    {
      sdec_released = false;
      sec--;
      if(sec < 0)
        sec = 59;
    }
  } else
    sdec_released = true;

  if(digitalRead(SINC_PIN) == HIGH)
  {
    if(sinc_released)
    {
      sinc_released = false;
      sec++;
      if(sec > 59)
        sec = 0;
    }
  } else
    sinc_released = true;

}
