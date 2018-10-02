/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios.
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting
 * with the serial monitor and sending a 'T'.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000001ELL, 0x000000001FLL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  //
  // Setup and configure rf radio
  //

  
  radio.begin();
  radio.setPayloadSize( 1 ) ;

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}


void loop(void)
{
  if (radio.available() )
  {
    Serial.println("hi") ;
    uint8_t input;

    String value = "" ;
    radio.read(&input, sizeof(uint8_t) );
    Serial.println(input) ;
    value = value + (input & 0b00001111) + "," + ( (input >> 4) & 0b00001111 ) +  "," ;
    delay(200) ;
    radio.read(&input, sizeof(uint8_t) );
    Serial.println(input) ;
    for (uint8_t mask = 1; mask < 128 ;)
    {
      if (mask == 1)
      {
        if ( (input & mask)== 1 )
        {
          value = value + "iamhere=true" + "," ;
        }
        else 
        {
          value = value + "iamhere=false" + "," ;
        }
      }
      if (mask == 2)
      {
        if ( (input & mask)== 2 )
        {
          value = value + "robot=true" + "," ;
        }
        else 
        {
          value = value + "robot=false" + "," ;
        }
      }
      else if (mask == 4)
      {
        if ( (input & mask)== 4 )
        {
          value = value + "north=true" + "," ;
        }
        else 
        {
          value = value + "north=false" + "," ;
        }
      }
      else if (mask == 8)
      {
        if ( (input & mask)== 8 )
        {
          value = value + "east=true" + "," ;
        }
        else 
        {
          value = value + "east=false" + "," ;
        }
      }
      else if (mask == 16)
      {
        if ( (input & mask)== 16 )
        {
          value = value + "south=true" + "," ;
        }
        else 
        {
          value = value + "south=false" + "," ;
        }
      }
      else if (mask == 32)
      {
        if ( (input & mask)== 32 )
        {
          value = value + "west=true" + "," ;
        }
        else 
        {
          value = value + "west=false" + "," ;
        }
      }
      else if (mask == 64)
      {
        if ( (input & mask)== 64 )
        {
          value = value + "treasure=true"  ;
        }
        else 
        {
          value = value + "treasure=false"  ;
        }
      }
      mask = mask << 1 ;
    }
    Serial.println(value) ;
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
