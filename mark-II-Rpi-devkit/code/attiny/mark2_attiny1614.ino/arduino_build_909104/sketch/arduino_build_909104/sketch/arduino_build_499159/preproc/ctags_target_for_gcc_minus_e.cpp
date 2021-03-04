# 1 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"

# 3 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino" 2

# 5 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino" 2





// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// Parameter 4 = array to store pixel data in




// Since this is for the static version of the library, we need to supply the pixel array
// This saves space by eliminating use of malloc() and free(), and makes the RAM used for
// the frame buffer show up when the sketch is compiled.

byte pixels[12 * 3];
//byte bufferI2C[NUMPIXELS * 3 + 100];
byte bufferI2C[256];
byte pattern[1][12];

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values. Finally, for the 4th argument we pass the array we
// defined above.

tinyNeoPixel strip = tinyNeoPixel(12, 1, ((1 << 6) | (1 << 4) | (0 << 2) | (2)), pixels);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
//volatile byte state = 1;
# 51 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
volatile byte state = 0;
long lastButtonPress = 0;
volatile bool nextState = false;
volatile bool readBuffer = false;
void setup() {

  Wire.begin(4); // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  pinMode(1,0x1);
  pinMode(0 /*PA4*/ , 0x1);
  pinMode(10 /*PA3*/ , 0x0);
  pinMode(9 /*PA2*/, 0x0);
  pinMode(2 /*PA6*/, 0x1);
  digitalWrite(2 /*PA6*/, 1);
  pinMode(3 /*PA7*/, 0x0);

  //delay(5);
  pinMode(8 /*PA1*/, 0x0);
  //pinMode(TPU_RST_L,   OUTPUT);
  //if(digitalRead(TPU_PGOOD)){
    //digitalWrite(TPU_RST_L, HIGH);  
  //}
  strip.setBrightness(100);
  strip.show(); // Initialize all pixels to 'off'

  setFan(0); // Start Fan as off
  Serial.begin(57600);
}

void loop() {
  switch(state){
    case 0:
      colorWipe(strip.Color(34, 167, 240), 70 ); // Blue
      colorWipe(strip.Color(0, 0, 0), 50); // 
      //setState( 1 );
      break;
    case 1:
      //strip.show();
      showi2cPixels();
      //colorWipe(strip.Color(200, 0, 0), 70 ); // Blue
      //colorWipe(strip.Color(0, 0, 0), 50); // 

      break;
    case 2:
      // Some example procedures showing how to display to the pixels:
      colorWipe(strip.Color(255, 0, 0), 50); // Red
      colorWipe(strip.Color(0, 0, 0), 50); // Red
      break;
    case 3:
      colorWipe(strip.Color(0, 255, 0), 50); // Green
      colorWipe(strip.Color(0, 0, 0), 50); // Red
      break;
    case 4:
      colorWipe(strip.Color(0, 0, 0), 0); // black
      //setState( 1 );
      break;

    case 5:
       theaterChase(strip.Color(127, 127, 127), 200); // White
       break;
    case 6:
      rainbow(5);
      break;
    case 7:
//      rainbowCycle(20);
      break;
    case 8:
 //     theaterChaseRainbow(50);
      break;
/*  

    case 9:

      crossfade();

      break;

 */
# 126 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
    case 10:
      show_pattern();
      state = 255;
      break;
    // 100 - 199 are commands that read stuff from the buffer
    case 100: // set pallete N
      set_pattern();
      break;
    case 101: // fill pallete N with single RGB
      fill_pattern();
      break;
/*    

    case 103: // cross fade between pallete N and M

      setup_crossfade();

      state = 9;

      break;

*/
# 143 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
    case 104:
      show_pattern();
      state = 255;
      break;
    case 202:
    case 255:
      break;
    default:
      //colorWipe(strip.Color(0, 0, 0), 0); // black
      strip.show();
      break;
  } // end switch stmt

  nextState = false;

  setI2CreadOnly();

}

long lastReadTime = 0;

/**

 * Read in statuses from inputs and load them into I2CBuffer

 */
# 167 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
void setI2CreadOnly(){
   if (millis() - lastReadTime > 100 ){
     // bufferI2C[80] = pulseIn(FAN_TACH, HIGH, 100) ;
      bufferI2C[0x51] = digitalRead(9 /*PA2*/);
      bufferI2C[0x52] = digitalRead(2 /*PA6*/);
      bufferI2C[0x53] = digitalRead(3 /*PA7*/);
      bufferI2C[0x54] = digitalRead(8 /*PA1*/);
      bufferI2C[0x60] = 0xFF;
      bufferI2C[0x61] = millis() % 255;
      lastReadTime = millis();
   }

}


/**

 * Speed is 0 to 255

 */
# 185 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
void setFan(byte fanSpeed){

  // FAN_PWM at zero turns on Fan
  // invert input so 255 is full on, zero is off
  analogWrite(0 /*PA4*/, ((255 - fanSpeed)<(0)?(0):((255 - fanSpeed)>(255)?(255):(255 - fanSpeed))));

}
inline void setState(byte s1){
  state = s1;
  if(state > 10){
      state = 10;
   }
   //nextState = true;
   readBuffer = false;
}
int getState(){
  return state;
}
/**

 * Is the boot sequence running

 */
# 206 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
bool isBootSequenceRunning(){
  if(getState() == 0){
    return true;
  }
  return false;
}

void cycleStates(){
  if(millis() - lastButtonPress > 100 ){
    state++;
    if(state > 10){
      setState(0);
    }
    lastButtonPress = millis();
  }
  nextState = true;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels() && !nextState; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void showi2cPixels() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(bufferI2C[i * 3], bufferI2C[i * 3+1], bufferI2C[i * 3+2]) ); // set color from i2c
    delay(10);
    strip.show();
  }
  strip.show();
}

void show_pattern() {
  byte pattern_num = bufferI2C[1];
  for(byte i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(pattern[pattern_num][i*3], pattern[pattern_num][i*3+1], pattern[pattern_num][i*3+2] )); // set color from pattern
    delay(10);
  }
  strip.show();
}

/*

byte fade_pattern_0;

byte fade_pattern_1;

byte fade_step;

byte fade_speed;

byte fade_pause;



void setup_crossfade() {

   fade_pattern_0 = bufferI2C[1];

   fade_pattern_1 = bufferI2C[2];

   fade_step      = bufferI2C[3];

   fade_speed     = bufferI2C[4];

   fade_pause     = bufferI2C[5];

}



void crossfade() {

  byte temp;

  uint16_t r,g,b;

  while (!nextState) {

    for(byte fade = 0; fade < 255; fade += fade_step) {

      for(byte i = 0; i < strip.numPixels(); i++) {      

        r = (((pattern[fade_pattern_0][i*3 + 0] * fade)) / 256)

          + ((pattern[fade_pattern_1][i*3 + 0] * (255 - fade)) / 256);

        g = (((pattern[fade_pattern_0][i*3 + 1] * fade)) / 256)

          + ((pattern[fade_pattern_1][i*3 + 1] * (255 - fade)) / 256);

        b = (((pattern[fade_pattern_0][i*3 + 2] * fade)) >> 8)

          + ((pattern[fade_pattern_1][i*3 + 2] * (255 - fade)) / 256);

        strip.setPixelColor(i, strip.Color(r,g,b));   

      }

      strip.show();

      temp = fade_pattern_0;

      fade_pattern_0 = fade_pattern_1;

      fade_pattern_1 = temp; 

      delay(fade_speed);

    }

    delay(fade_pause);

  }

}

*/
# 293 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 && !nextState; j++) {
    for(i=0; i<strip.numPixels() && !nextState; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
/*

// Slightly different, this makes the rainbow equally distributed throughout

void rainbowCycle(uint8_t wait) {

  uint16_t i, j;



  for(j=0; j<256*5 && !nextState; j++) { // 5 cycles of all colors on wheel

    for(i=0; i< strip.numPixels() && !nextState; i++) {

      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));

    }

    strip.show();

    delay(wait);

  }

}

*/
# 318 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10 && !nextState; j++) { //do 10 cycles of chasing
    for (int q=0; q < 3 && !nextState; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels() && !nextState; i=i+3) {
        strip.setPixelColor(i+q, 0); //turn every third pixel off
      }
    }
  }
}

/*

//Theatre-style crawling lights with rainbow effect

void theaterChaseRainbow(uint8_t wait) {

  for (int j=0; j < 256 && !nextState; j++) {     // cycle all 256 colors in the wheel

    for (int q=0; q < 3 && !nextState; q++) {

      for (int i=0; i < strip.numPixels() && !nextState; i=i+3) {

        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on

      }

      strip.show();



      delay(wait);



      for (int i=0; i < strip.numPixels() && !nextState; i=i+3) {

        strip.setPixelColor(i+q, 0);        //turn every third pixel off

      }

    }

  }

}

*/
# 355 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void set_pattern() {
  byte pattern_num = bufferI2C[0];
  byte addrStart = bufferI2C[1];
  pattern[pattern_num][addrStart * 3] = bufferI2C[2];
  pattern[pattern_num][addrStart * 3] = bufferI2C[3];
  pattern[pattern_num][addrStart * 3] = bufferI2C[4];
}

void fill_pattern() {
  byte pattern_num = bufferI2C[0];
  for (byte i=0; i < 12; i++) {
    pattern[pattern_num][1 + i * 3] = bufferI2C[1];
    pattern[pattern_num][2 + i * 3] = bufferI2C[2];
    pattern[pattern_num][3 + i * 3] = bufferI2C[3];
  }
}



byte lastNumItems = 0;
byte addrStart = 0;
volatile byte reg = 0;
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
/**

 * 

 * sudo ./vfctrl_usb  SET_I2C_WITH_REG 74 5 13   5  100 101 102 103 104 105 106 107 108 109 110 116  112  113  0  0  0  0  0  0  0  0  0  0  0  0  0 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 0  0  0  0  0  0  0  0  0  0  

 * SET_I2C_WITH_REG 74 5 13   5 

 *               74 Address

 *                5 start byte

 *               13 length

 *                5 state to set 

 *          101...  12 bytes for colors

 *                  

 **/
# 407 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
void receiveEvent(int howMany) {
  readBuffer = true;
  byte i = 0;

  lastNumItems = howMany;

  /**

   * if how many, then requesting a read, else read pixel values

   */
# 416 "C:\\Dropbox\\Crashspace\\Mycroft\\programming\\mark2_attiny1614.ino\\mark2_attiny1614.ino.ino"
  if(howMany == 1){
    reg = Wire.read();
    Serial.print(reg);
  }
  else{
    addrStart = Wire.read() ; // first byte is pixel address or command

    if(addrStart < 12) {
      while (1 <= Wire.available()) { // loop through all but the last
        byte c = Wire.read(); // receive byte as a character
        bufferI2C[addrStart * 3 + i] = c;
        i++;
      }
      setState(1);
    }
    else {
      while (1 <= Wire.available()) { // loop through all but the last
        byte c = Wire.read(); // receive byte as a character
        bufferI2C[addrStart ] = c;
      }
      //setState(addrStart - NUMPIXELS);
    }

    if (addrStart == 101){
      byte fanSpeed = 0;
      fanSpeed = bufferI2C[101];
      setFan(fanSpeed);
    }

    nextState = true;
  }
}


//byte startingNum = 0;
void requestEvent() {
   //byte count = 0;
   if(reg < 256 && reg >= 0 ){
     Wire.write(bufferI2C[reg]);
     //Wire.write(reg);
   }
   //Wire.write(0x44);
   //Serial.println(bufferI2C[reg]);
}