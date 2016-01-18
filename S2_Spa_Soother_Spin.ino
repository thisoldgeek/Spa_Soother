/* 
 * Copyright (C) 2013 Gilad Dayagi.  All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

 /*
  * An example for the Arduino particle system library
  * Creates a spinning galaxy effect
  */

//'r' for rainbowduino, 'c' for colorduino
#define BOARD 'r'

#if BOARD == 'c'
#include <Colorduino.h>
#else
#include <Rainbowduino.h>
#endif

#include "ParticleSys.h"
#include "Particle_Bounce.h"
#include "Emitter_Spin.h"
#include "PartMatrix.h"

const byte numParticles = 60;

Particle_Bounce particles[numParticles];
Emitter_Spin emitter(112, 112, 5, 7);
ParticleSys pSys(numParticles, particles, &emitter);
PartMatrix pMatrix;

/**
 * Render the particles into a low-resolution matrix
 */
void drawMatrix(){
    pMatrix.fade();
    pMatrix.render(particles, numParticles);
    //update the actual LED matrix
    for (byte y=0;y<PS_PIXELS_Y;y++) {
        for(byte x=0;x<PS_PIXELS_X;x++) {
#if BOARD == 'c'
            Colorduino.SetPixel(x, y, pMatrix.matrix[x][y].r, pMatrix.matrix[x][y].g, pMatrix.matrix[x][y].b);
#else
            Rb.setPixelXY(x, y, pMatrix.matrix[x][y].r, pMatrix.matrix[x][y].g, pMatrix.matrix[x][y].b);
#endif
        }
    }
}

String msg;   // Will receive "Shutdown" when RPi times out
int switch_off = 3;  // RainbowDuino digital pin turning switch OFF
int time_off = 30000;
long elpased;  // amount of time since Shutdown message from RPi Zero
int start_countdown = 0;

void setup()
{ Serial.begin(9600);
  Serial.println("Started...");
  pinMode(switch_off, OUTPUT);
  pinMode(A2, OUTPUT);   // A2 = RaindowDuino digital pin controlling LED in front/power momentary switch
  digitalWrite(switch_off, LOW);  // OFF until RPi Zero sends Shutdown message
  digitalWrite(A2, HIGH); // ON while Arduino is powered up
    
#if BOARD == 'c'    
  Colorduino.Init(); // initialize the board
  
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  byte whiteBalVal[3] = {36,63,7}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
#else
    Rb.init();
#endif

  randomSeed(analogRead(0));
  
  pMatrix.reset();
  PartMatrix::isOverflow = true;
  
  emitter.oscilate = true;
}

void loop()
{   //check for shutdown from Pi
    while(Serial.available()) 
        {msg = Serial.readString();  // read the incoming data as string
         Serial.println(msg);
          if (msg == "Shutdown")
            { elpased = millis();
              start_countdown = 1;
            }
        } 
     // if the Pi is shutting down, wait for it to stop
     // then turn the power switch off     
     if (start_countdown){
         if ( millis() - elpased > time_off)
           {         
           digitalWrite(switch_off, HIGH);
            }   
     }
     
    // Particle System Display    
    pSys.update();
    drawMatrix();
#if BOARD == 'c'    
    Colorduino.FlipPage();
#endif 
    delay(30);
}

