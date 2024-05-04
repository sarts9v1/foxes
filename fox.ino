//=================================================================
// Modified from N10MJ Nano Fox ver 1.0
// Added dual speed operations
//=================================================================
//   define the morse code for the alphabet and numbers
//=================================================================
char* letters[] = {
  ".-",     // A
  "-...",   // B
  "-.-.",   // C
  "-..",    // D
  ".",      // E
  "..-.",   // F
  "--.",    // G
  "....",   // H
  "..",     // I
  ".---",   // J
  "-.-",    // K
  ".-..",   // L
  "--",     // M
  "-.",     // N
  "---",    // O
  ".--.",   // P
  "--.-",   // Q
  ".-.",    // R
  "...",    // S
  "-",      // T
  "..-",    // U
  "...-",   // V
  ".--",    // W
  "-..-",   // X
  "-.--",   // Y
  "--.."    // Z
};

char* numbers[] = {
  "-----",   // 0
  ".----",   // 1
  "..---",   // 2
  "...--",   // 3
  "....-",   // 4
  ".....",   // 5
  "-....",   // 6
  "--...",   // 7
  "---..",   // 8
  "----."    // 9   --- end of 1st segment of borrowed code
};

int PTTpin = 2;         // Radio PTT Pin
int TonePin = 3;        // tone output pin

int TXled = 13;         // LED Pin
int frequency = 1000;   // frequency of tone

#define DOTDELAY 70
#define FASTDOTDELAY 30
#define CHARDELAY 400
#define FASTCHARDELAY 200
#define WORDDELAY 1100
#define FASTWORDDELAY 400

int dotDelay = 70;      // duration of the dot in morse code, this is also the time between the dots and dashes 70 is default
int charDelay = 400;    // duration of the wait between letters for Farsnworth method 500 is default
int wordDelay = 1100;   // duration of the wait between words for Farsnworth method 1100 is default

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output:
  pinMode(2, OUTPUT);     // PTT Output
  pinMode(3, OUTPUT);     // Tone Output Pin
  pinMode(13, OUTPUT);    // output pin for PTT LED
  delay(2000);            // initial delay after powering on
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(PTTpin, HIGH);     // turn the PTTpin on (HIGH is the voltage level)
  digitalWrite(TXled, HIGH);      // turn the PTTpin on (HIGH is the voltage level)
  delay(1000);                    // dead key for 1 sec before CWID

  SendText("DE ISM", 1);        //Ident
  delay(1000);                    // dead key for 1 sec before fox beacon

  SendText("FOX 3 3 3 3 3", 0);          // Fox message that will be sent
  delay(1000);                    // wait for a second

  tone(TonePin, frequency);
  delay(5000);                    // send 5 sec tone with tone

  noTone(TonePin);
  delay(5000);                    // 5 seconds transmit w/o tone

  digitalWrite(PTTpin, LOW);      // turn the PTTpin off by making the voltage LOW
  digitalWrite(TXled, LOW);       // turn the PTTpin off by making the voltage LOW
  delay(10000);                   // delay before next TX cycle 1000 = 1 sec
}
//=================================================================
//
// Input: Character Array of Dots and Dashes to be sent
//
// Description:
//      This function takes as input an array or "." and "-" and
//      calls dotOrDash for each item in the array.
//
//      At the end of the sequence, there is a delay of 3 times 
//      the dot duration.
//
//=================================================================

void morseCodeSequence(const char* sequence, char speed)
{
   int i = 0;

   // Loop for each element in the array
   while (sequence[i] != 0)  
   {
      dotOrDash(sequence[i], speed);     // Send out the dot or dash
       i++;                        // Increment to the next element in the array
   }
   if(speed == 0){
   delay(CHARDELAY);            // gap between letters
   }else{
    delay(FASTCHARDELAY);
   }
}
//=================================================================
// Function: SendText
// Input: Character Array of text in English
// Description:
//      This function takes text as input and sends Morse code for each letter.
//      There then is a pause after each letter.
//=================================================================

void SendText(const char* MorseCodeLetters, char speed)
{
   int i = 0;
   char ch;

   // Loop for each element in the array
   while (MorseCodeLetters[i] != 0)  
   {
    ch = MorseCodeLetters[i];
    // Is it lowercase letter?
    if (ch >= 'a' && ch <= 'z')
    {
      morseCodeSequence(letters[ch - 'a'], speed);
    }
    else if (ch >= 'A' && ch <= 'Z')    // Uppercase Letter
    {
      morseCodeSequence(letters[ch - 'A'], speed);
    }
    else if (ch >= '0' && ch <= '9')    // Number
    {
      morseCodeSequence(numbers[ch - '0'], speed);
    }
    else if (ch == ' ')                // Space (wait for 4 times dotDelay
    {
      if(speed == 0){
       delay(WORDDELAY);      // gap between words
      }else{
        delay(FASTWORDDELAY);
      }
    }
    else {
    }
    i++;         // Increment to the next element in the array
   }
      if(speed == 0){
        delay(CHARDELAY);            // gap between letters
      }else{
        delay(FASTCHARDELAY);
      }
}

//=================================================================
// Function: dorOrDash
// Input: Character that should be either a dot or a dash
// Description:
//      This function first turns on the output then looks to see 
//      if the character is a "." and if so delays the dotDelay.
//      If the character is not a "." then the routine assumes it
//      is a "-" and keep the output high for 3 times the length of
//      dotDelay.  This could be improved by making sure the 
//      character is a "-" but for most cases it would not matter.
//      After the delay time the pin is taken low turning off the 
//      tone.
//      Then it delays for one dotDelay time so the dots and dashes
//      do not run together.
//=================================================================

void dotOrDash(const char dotOrDash, char speed)
{
  tone(TonePin, frequency);
  if (dotOrDash == '.')
  {
    if(speed == 0){
    delay(DOTDELAY);
    }else{
      delay(FASTDOTDELAY);           
    }
  }
  else // must be a -
  {
    if(speed == 0){
      delay(DOTDELAY * 3);
    }else{
      delay(FASTDOTDELAY *3);           
    }
  }
  noTone(TonePin);
  if(speed == 0){
    delay(DOTDELAY); // gap between flashes
  }else{
    delay(FASTDOTDELAY);
  }
}