#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

int redLed = 11;
int greenLed = 12;

int leftLed = 6;
int midLed = 7;
int rightLed = 8;
int mainLed = 9;

// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void setup() {
  // set up serial port
  Serial.begin( 9600 );
  putstring_nl( "WaveHC with " );
  //Serial.print( NUMBUTTONS, DEC );
  //putstring_nl( "buttons" );
  
  putstring( "Free RAM: " );       // This can help with debugging, running out of RAM is bad
  Serial.println( freeRam( ) );      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode( 2, OUTPUT );
  pinMode( 3, OUTPUT );
  pinMode( 4, OUTPUT );
  pinMode( 5, OUTPUT );

  pinMode( redLed, OUTPUT );
  pinMode( greenLed, OUTPUT );
  pinMode( leftLed, OUTPUT );
  pinMode( midLed, OUTPUT );
  pinMode( rightLed, OUTPUT );
  pinMode( mainLed, OUTPUT );
  
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");
}

void flashSeq1( )
{
  digitalWrite( leftLed, HIGH );
  delay( 50 );
  digitalWrite( leftLed, LOW );
  
  delay( 50 );
  
  digitalWrite( leftLed, HIGH );
  delay( 100 );
  digitalWrite( leftLed, LOW );
  
  delay( 50 );

  digitalWrite( leftLed, HIGH );
  digitalWrite( midLed, HIGH );
  delay( 200 );
  digitalWrite( leftLed, LOW );
  digitalWrite( midLed, LOW );

  delay( 20 );

  digitalWrite( mainLed, HIGH );
  delay( 80 );
  digitalWrite( mainLed, LOW );

  delay( 20 );

  digitalWrite( mainLed, HIGH );
  digitalWrite( rightLed, HIGH );
  delay( 150 );
  digitalWrite( mainLed, LOW );
  digitalWrite( rightLed, LOW );

  delay( 30 );

  digitalWrite( mainLed, HIGH );
  delay( 50 );
  digitalWrite( mainLed, LOW );
  
  delay( 50 );

  digitalWrite( rightLed, HIGH );
  delay( 30 );
  digitalWrite( rightLed, LOW );
}

void flashSeq2( )
{
  digitalWrite( midLed, HIGH );
  delay( 50 );
  digitalWrite( midLed, LOW );
  delay( 30 );
  digitalWrite( midLed, HIGH );
  delay( 30 );
  digitalWrite( midLed, LOW );
  delay( 50 );
  digitalWrite( midLed, HIGH );
  delay( 30 );
  digitalWrite( midLed, LOW );
  delay( 30 );
  
  digitalWrite( midLed, HIGH );
  delay( 10 );
  digitalWrite( rightLed, HIGH );
  delay( 20 );
  digitalWrite( rightLed, LOW );
  delay( 30 );
  digitalWrite( midLed, LOW );
  
  delay( 100 );
  
  digitalWrite( rightLed, HIGH );
  delay( 20 );
  digitalWrite( rightLed, LOW );
  delay( 30 );
  digitalWrite( rightLed, HIGH );
  delay( 80 );
  digitalWrite( rightLed, LOW );
  delay( 20 );
  digitalWrite( rightLed, HIGH );
  delay( 30 );
  digitalWrite( midLed, HIGH );
  delay( 50 );
  digitalWrite( midLed, LOW );
  delay( 20 );
  digitalWrite( rightLed, LOW );
  delay( 20 );
  digitalWrite( rightLed, HIGH );
  delay( 120 );
  digitalWrite( rightLed, LOW );
  delay( 20 );
  digitalWrite( mainLed, HIGH );
  delay( 20 );
  digitalWrite( mainLed, LOW );
  delay( 50 );
  digitalWrite( mainLed, HIGH );
  delay( 20 );
  digitalWrite( mainLed, LOW );
  delay( 30 );
  digitalWrite( mainLed, HIGH );
  delay( 80 );
  digitalWrite( mainLed, LOW );
  delay( 30 );
  digitalWrite( mainLed, HIGH );
  delay( 100 );
  digitalWrite( leftLed, HIGH );
  digitalWrite( mainLed, LOW );
  delay( 30 );
  digitalWrite( leftLed, LOW );
  digitalWrite( mainLed, HIGH );
  delay( 200 );
  digitalWrite( mainLed, LOW );
  digitalWrite( leftLed, HIGH );
  delay( 50 );
  digitalWrite( leftLed, LOW );
}

void flashSeq3( )
{
  digitalWrite( midLed, HIGH );
  delay( 50 );
  digitalWrite( midLed, LOW );

  delay( 20 );
  
  digitalWrite( midLed, HIGH );
  delay( 20 );
  digitalWrite( midLed, LOW );

  delay( 100 );
  
  digitalWrite( midLed, HIGH );
  digitalWrite( leftLed, HIGH );
  delay( 50 );
  digitalWrite( midLed, LOW );
  digitalWrite( leftLed, LOW );

  delay( 50 );

  digitalWrite( rightLed, HIGH );
  delay( 30 );
  digitalWrite( rightLed, LOW );

  delay( 10 );

  digitalWrite( rightLed, HIGH );
  digitalWrite( leftLed, HIGH );
  delay( 50 );
  digitalWrite( rightLed, LOW );
  digitalWrite( leftLed, LOW );

  delay( 10 );

  digitalWrite( midLed, HIGH );
  delay( 20 );
  digitalWrite( midLed, LOW );
  
  delay( 10 );
  
  digitalWrite( midLed, HIGH );
  delay( 50 );
  digitalWrite( midLed, LOW );
}

void randSeq( )
{
  int led1, led2;
  int dly, flashLen, areaProb;

  int cnt = random( 1, 30 );
  for ( int i = 0; i < cnt; i ++ )
  {
    dly = random( 0, 150 );
    flashLen = random( 10, 150 );
    
    areaProb = random( 0, 100 );
    if ( areaProb < 70 ) // 70% chance
    {
      switch ( random( 4 ) )
      {
        case 0:
          digitalWrite( leftLed, HIGH );
        break;
        case 1:
          digitalWrite( midLed, HIGH );
        break;
        case 2:
          digitalWrite( rightLed, HIGH );
        break;
        case 3:
          digitalWrite( mainLed, HIGH );
        break;
      }
    }
    else if ( areaProb < 90 ) // 20% chance
    {
      switch ( random( 6 ) )
      {
        case 0:
          digitalWrite( leftLed, HIGH );
          digitalWrite( midLed, HIGH );
        break;
        case 1:
          digitalWrite( leftLed, HIGH );
          digitalWrite( rightLed, HIGH );
        break;
        case 2:
          digitalWrite( leftLed, HIGH );
          digitalWrite( mainLed, HIGH );
        break;
        case 3:
          digitalWrite( midLed, HIGH );
          digitalWrite( rightLed, HIGH );
        break;
        case 4:
          digitalWrite( midLed, HIGH );
          digitalWrite( mainLed, HIGH );
        break;
        case 5:
          digitalWrite( rightLed, HIGH );
          digitalWrite( mainLed, HIGH );
        break;
      }
    }
    else // 10% chance
    {
      switch ( random( 4 ) )
      {
        case 0:
          led1 = leftLed;
          digitalWrite( leftLed, HIGH );
        break;
        case 1:
          led1 = midLed;
          digitalWrite( midLed, HIGH );
        break;
        case 2:
          led1 = rightLed;
          digitalWrite( rightLed, HIGH );
        break;
        case 3:
          led1 = mainLed;
          digitalWrite( mainLed, HIGH );
        break;
      }

      delay( random( 10, 100 ) );

      switch ( random( 4 ) )
      {
        case 0:
          if ( led1 != leftLed )
            led2 = leftLed;
          else
            led2 = rightLed;
          digitalWrite( led2, HIGH );
        break;
        case 1:
          if ( led1 != midLed )
            led2 = midLed;
          else
            led2 = mainLed;
          digitalWrite( led2, HIGH );
        break;
        case 2:
          if ( led1 != rightLed )
            led2 = rightLed;
          else
            led2 = leftLed;
          digitalWrite( led2, HIGH );
        break;
        case 3:
          if ( led1 != mainLed )
            led2 = mainLed;
          else
            led2 = midLed;
          digitalWrite( led2, HIGH );
        break;
      }

      delay( random( 10, 150 ) );

      digitalWrite( led2, LOW );
    }

    delay( dly );
    digitalWrite( leftLed, LOW );
    digitalWrite( midLed, LOW );
    digitalWrite( rightLed, LOW );
    digitalWrite( mainLed, LOW );
    
    if ( cnt < cnt - 1 )
      delay( dly );
  }
}


void loop() {
  //digitalWrite( redLed, HIGH );
  //delay( 500 );
  //digitalWrite( greenLed, HIGH );
  //delay( 500 );

  if ( !wave.isplaying )
    playfile( "THUNDER.WAV" );

  switch ( random( 4 ) )
  {
    case 0:
      flashSeq1( );
    break;
    case 1:
      flashSeq2( );
    break;
    case 2:
      flashSeq3( );
    break;
    case 3:
      randSeq( );
    break;
  }

  //digitalWrite( redLed, LOW );
  //delay( 500 );
  //digitalWrite( greenLed, LOW );
  //delay( 500 );
  
  //digitalWrite( light1, LOW );
  //delay( 500 );
  //digitalWrite( light2, LOW );
  //delay( 500 );
  //digitalWrite( light3, LOW );
  //delay( 500 );
  //digitalWrite( light4, LOW );
  delay( 1000 * random( 0, 10 ) );
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}
