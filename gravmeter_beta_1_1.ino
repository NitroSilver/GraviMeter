/**************************************************
* Class: SevenSegment
* Author: Jarrett Kral
* Use: Control Seven Segment Displays
**************************************************/
class SevenSegment
{
  public:
    SevenSegment();
    SevenSegment(byte segPins[]);
    void display(byte number);
    void clearSegment();
    void displaySegment(byte number);
    void displayBorder(byte border);

  private:
    byte data[10];
    byte segmentPins[7];
};

/**************************************************
* SevenSegment: Constructor
**************************************************/
SevenSegment::SevenSegment()
{
}

/**************************************************
* SevenSegment: Constructor
*  segPins: 7 pins to controll the segment
**************************************************/
SevenSegment::SevenSegment(byte segPins[])
{
  byte tempData[10] =
  {
    B00000011,
    B10011111,
    B00100101,
    B00001101,
    B10011001,
    B01001001,
    B01000001,
    B00011111,
    B00000001,
    B00011001
  };

  for (byte row=0; row<10; row++)
    data[row]=tempData[row];

  for (byte set=0;set<7;set++)
  {            
    segmentPins[set]=segPins[set];
    pinMode(segmentPins[set],OUTPUT);
    digitalWrite(segmentPins[set],HIGH);
  }
}

/**************************************************
* SevenSegment: Display
*  Use: Displays a number
*  number: The number to display
**************************************************/
void SevenSegment::display(byte number)
{  
  for (byte col=0;col<7;col++)
    digitalWrite(segmentPins[col],bitRead(data[number],7-col));
}

/**************************************************
* Seven Segment displaySegment
*  Use: Displays a single segment
*  number: The segment number to display
**************************************************/
void SevenSegment::displaySegment(byte number)
{
  for (byte col=0;col<7;col++)
    digitalWrite(segmentPins[col],(number - 1 == col) ? 0 : 1);
}

/**************************************************
* Seven Segment displayBorder
*  Use: Displays a border on the segment
*  number: The segment boarder numberr to display
**************************************************/
void SevenSegment::displayBorder(byte border)
{
  if (border == 1)
  {
    for (byte col=0;col<7;col++)
      digitalWrite(segmentPins[col],(col == 0 or col == 1 or col == 2 or col == 3) ? 0 : 1);
  }
  else if (border == 2)
  {
    for (byte col=0;col<7;col++)
      digitalWrite(segmentPins[col],(col == 0 or col == 3) ? 0 : 1);    
  }
  else
  {
    for (byte col=0;col<7;col++)
      digitalWrite(segmentPins[col],( col == 0 or col == 3 or col == 4 or col == 5) ? 0 : 1);
  }
}

/**************************************************
* Seven Segment clearSegment
*  Use: Turns off all segments
**************************************************/
void SevenSegment::clearSegment()
{
  for (byte col=0;col<7;col++)
    digitalWrite(segmentPins[col], 1);
}

/**************************************************
* Class: DualThreeDigitSevenSegmentMux
* Author: Jarrett Kral
* Use: Control Two Seven Segment Displays
**************************************************/
class DualThreeDigitSevenSegmentMux
{
  public:
    DualThreeDigitSevenSegmentMux(); 
    DualThreeDigitSevenSegmentMux(byte segPins[], byte digiPowerPins[]); 
    void display();
    void displayWaiting(int &frame);
    void displayReady(int &frame);
    void displayZero(int &frame);
    void update(int one, int two);
    void updateSingle(byte displaySelection, int value);
    
  private:
    SevenSegment segment;
    byte digitPowerPins[6];
    int oneNumber;
    int twoNumber;
    int currentDigit;
    int oneFrame;
    int twoFrame;
};

/**************************************************
* DualThreeDigitSevenSegmentMux: Constructor
**************************************************/
DualThreeDigitSevenSegmentMux::DualThreeDigitSevenSegmentMux(){
}

/**************************************************
* DualThreeDigitSevenSegmentMux: Constructor
*  segPins: 7 pins to control the segments
*  digiPowerPins: 3 pins to control the power lines
**************************************************/
DualThreeDigitSevenSegmentMux::DualThreeDigitSevenSegmentMux(byte segPins[], byte digiPowerPins[])
{
  currentDigit = 1;
  oneNumber = 0;
  twoNumber = 0;
  oneFrame = 0;
  twoFrame = 0;
  segment = SevenSegment(segPins);
  
  for (byte set=0;set<6;set++)
  {
    digitPowerPins[set]=digiPowerPins[set];
    pinMode(digitPowerPins[set],OUTPUT);
    digitalWrite(digitPowerPins[set],LOW);
  }
}

/**************************************************
* DualThreeDigitSevenSegmentMux: update
*  Use: Updates both timer values
*  one: Value for timer one
*  two: Value for timer two
**************************************************/
void DualThreeDigitSevenSegmentMux::update(int one, int two)
{
  oneNumber = one;
  twoNumber = two;
}

/**************************************************
* DualThreeDigitSevenSegmentMux: updateSingle
*  Use: Updates a single timer
*  displaySelection: Choice between 1 or 2 identifying the timer
*  value: The value to update the timer
**************************************************/
void DualThreeDigitSevenSegmentMux::updateSingle(byte displaySelection, int value)
{
  if (displaySelection == 1)
    oneNumber = value;
  else if (displaySelection == 2)
    twoNumber = value;
}

/**************************************************
* DualThreeDigitSevenSegmentMux: display
*  Use: Sends the timer information to the display
**************************************************/
void DualThreeDigitSevenSegmentMux::display()
{
  if (currentDigit > 6)
    currentDigit = 1;

  if (currentDigit == 1)
  {
    digitalWrite(digitPowerPins[5], LOW);
    if (oneNumber == -1)
      displayWaiting(oneFrame);
    else if (oneNumber == -2)
      displayReady(oneFrame);
    else if (oneNumber == -3)
      displayZero(oneFrame);
    else
      segment.display(oneNumber % 10);
    digitalWrite(digitPowerPins[0], HIGH);
  }
  else if (currentDigit == 3)
  {
    digitalWrite(digitPowerPins[3], LOW);
    if (oneNumber == -1)
      displayWaiting(oneFrame);
    else if (oneNumber == -2)
      displayReady(oneFrame);
    else if (oneNumber == -3)
      displayZero(oneFrame);
    else
      segment.display((oneNumber / 10) % 10);
    digitalWrite(digitPowerPins[1], HIGH);
  }
  else if (currentDigit == 5)
  {
    digitalWrite(digitPowerPins[4], LOW);
    if (oneNumber == -1)
      displayWaiting(oneFrame);
    else if (oneNumber == -2)
      displayReady(oneFrame);
    else if (oneNumber == -3)
      displayZero(oneFrame);
    else
      segment.display(oneNumber / 100);
    digitalWrite(digitPowerPins[2], HIGH);
  }
  
  else if (currentDigit == 2)
  {
    digitalWrite(digitPowerPins[0], LOW);
    if (twoNumber == -1)
      displayWaiting(twoFrame);
    else if (twoNumber == -2)
      displayReady(twoFrame);
    else if (twoNumber == -3)
      displayZero(twoFrame);
    else
      segment.display(twoNumber % 10);
    digitalWrite(digitPowerPins[3], HIGH);
  }
  else if (currentDigit == 4)
  {
    digitalWrite(digitPowerPins[1], LOW);
    if (twoNumber == -1)
      displayWaiting(twoFrame);
    else if (twoNumber == -2)
      displayReady(twoFrame);
    else if (twoNumber == -3)
      displayZero(twoFrame);
    else
      segment.display((twoNumber / 10) % 10);
    digitalWrite(digitPowerPins[4], HIGH);
  }
  else if (currentDigit == 6)
  {
    digitalWrite(digitPowerPins[2], LOW);
    if (twoNumber == -1)
      displayWaiting(twoFrame);
    else if (twoNumber == -2)
      displayReady(twoFrame);
    else if (twoNumber == -3)
      displayZero(twoFrame);
    else
      segment.display(twoNumber / 100);
    digitalWrite(digitPowerPins[5], HIGH);
  }
  currentDigit++;
}

/**************************************************
* DualThreeDigitSevenSegmentMux: displayWaiting
*  Use: Displays a waiting animation on the currently powered display
*  frame: The frame of animation to draw
**************************************************/
void DualThreeDigitSevenSegmentMux::displayWaiting(int &frame)
{
  int theDigit;
  theDigit = (currentDigit % 2 == 0) ? currentDigit / 2: currentDigit / 2 + 1;
  
  if (frame > 500)
    frame = 0;
  else
    frame = frame + 1;
    
  if (frame < 50 && frame > 0)
  {
    if (theDigit == 1)
      segment.displaySegment(4);
    else
      segment.clearSegment();
  }
  else if (frame < 100 && frame > 50)
  {
    if (theDigit == 2)
      segment.displaySegment(4);
    else
      segment.clearSegment();
  }
  else if (frame < 150 && frame > 100)
  {
    if (theDigit == 3)
      segment.displaySegment(4);
    else
      segment.clearSegment();
  }
  else if (frame < 200 && frame > 150)
  {
    if (theDigit == 3)
      segment.displaySegment(5);
    else
      segment.clearSegment();
  }
  else if (frame < 250 && frame > 200)
  {
    if (theDigit == 3)
      segment.displaySegment(6);
    else
      segment.clearSegment();
  }
  else if (frame < 300 && frame > 250)
  {
    if (theDigit == 3)
      segment.displaySegment(1);
    else
      segment.clearSegment();
  }
  else if (frame < 350 && frame > 300)
  {
    if (theDigit == 2)
      segment.displaySegment(1);
    else
      segment.clearSegment();
  }
  else if (frame < 400 && frame > 350)
  {
    if (theDigit == 1)
      segment.displaySegment(1);
    else
      segment.clearSegment();
  }
  else if (frame < 450 && frame > 400)
  {
    if (theDigit == 1)
      segment.displaySegment(2);
    else
      segment.clearSegment();
  }
  else if (frame < 500 && frame > 450)
  {
    if (theDigit == 1)
      segment.displaySegment(3);
    else
      segment.clearSegment();
  }
  else
    segment.clearSegment();
}

/**************************************************
* DualThreeDigitSevenSegmentMux: displayReady
*  Use: Displays a ready animation on the currently powered display
*  frame: The frame of animation to draw
**************************************************/
void DualThreeDigitSevenSegmentMux::displayReady(int &frame)
{
  int theDigit;
  theDigit = (currentDigit % 2 == 0) ? currentDigit / 2: currentDigit / 2 + 1;
  
  if (frame > 100)
    frame = 0;
  else
    frame = frame + 1;
    
  if (frame < 50)
    segment.displayBorder(theDigit);
  else
    segment.clearSegment();
}

/**************************************************
* DualThreeDigitSevenSegmentMux: displayZero
*  Use: Displays a blinking zero animation on the currently powered display
*  frame: The frame of animation to draw
**************************************************/
void DualThreeDigitSevenSegmentMux::displayZero(int &frame)
{
  int theDigit;
  theDigit = (currentDigit % 2 == 0) ? currentDigit / 2: currentDigit / 2 + 1;
  
  if (frame > 100)
    frame = 0;
  else
    frame = frame + 1;
    
  if (frame < 95)
    segment.display(0);
  else
    segment.clearSegment();
}

/**************************************************
* Class: photoDiode
* Author: Jarrett Kral
* Use: Control a photodiode
**************************************************/
class photoDiode
{
  public:
    photoDiode();
    photoDiode(int pinNumber, float photoSensitivity);
    boolean stateCheck();
    void voltageRead();
    float voltageCheck();
    void phaseNext();
    boolean phaseCheck(int currentPhase);
    void phaseReset();
    unsigned long timerCheck();
    void timerStart(unsigned long startTime);
    void timerUpdate(unsigned long currentMillis);
    void timerReset();

  private:
    int pin;
    int phase;
    int timeResolution;
    boolean state;
    float voltage;
    unsigned long timeStart;
    unsigned long timeCurrent;
    float sensitivity;
};

/**************************************************
* photoDiode: constructor
**************************************************/
photoDiode::photoDiode(int pinNumber, float photoSensitivity)
{
  timeResolution = 10;
  voltage = 0;
  sensitivity = photoSensitivity;
  pin = pinNumber;
  state = false;
  phase = 1;
}

/**************************************************
* photoDiode: constructor
**************************************************/
photoDiode::photoDiode()
{
  timeResolution = 10;
  voltage = 0;
  sensitivity = 0.2;
  state = false;
}

/**************************************************
* photoDiode: voltageRead
**************************************************/
void photoDiode::voltageRead()
{
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = analogRead(pin) * (5.0 / 1023.0);
  
  if (voltage > sensitivity)
    state = true;
  else
    state = false;
}

/**************************************************
* photoDiode: stateCheck
**************************************************/
boolean photoDiode::stateCheck(){
  return state;
}

/**************************************************
* photoDiode: voltageCheck
**************************************************/
float photoDiode::voltageCheck(){
  return voltage;
}

/**************************************************
* photoDiode: timerStart
**************************************************/
void photoDiode::timerStart(unsigned long startTime)
{
  timerReset();
  timeStart = startTime;
}

/**************************************************
* photoDiode: timerReset
**************************************************/
void photoDiode::timerReset()
{
  timeStart = 0;
  timeCurrent = 0;
}

/**************************************************
* photoDiode: timerUpdate
**************************************************/
void photoDiode::timerUpdate(unsigned long currentMillis){
  timeCurrent = (currentMillis - timeStart) / timeResolution;
}

/**************************************************
* photoDiode: timerCheck
**************************************************/
unsigned long photoDiode::timerCheck(){
  return timeCurrent;
}

/**************************************************
* photoDiode: phaseNext
**************************************************/
void photoDiode::phaseNext()
{
  phase++;
}

/**************************************************
* photoDiode: phaseCheck
**************************************************/
boolean photoDiode::phaseCheck(int currentPhase){
  return phase == currentPhase;
}

/**************************************************
* photoDiode: phaseReset
**************************************************/
void photoDiode::phaseReset(){
  phase = 1;
}

/**************************************************
**************************************************
* Main Program
* Author: Jarrett Kral
**************************************************
**************************************************/

/**************************************************
* Globals: Connection
**************************************************/
// Output pins for the LED segments a,b,c,d,e,f,g
byte segmentPins[] ={1,2,3,4,5,6,7};
// Output pins for the LED power/display a 1,2,3, timer b 1,2,3
byte displayPins[] ={13,12,11,10,9,8};
// Input pins for the photodiode
byte detectorPins[] ={1,2};

/**************************************************
* Globals: Program
**************************************************/
DualThreeDigitSevenSegmentMux timers;
photoDiode sensorOne;
photoDiode sensorTwo;

unsigned long currentMillis = 0;
int debug = 0;
int loopDelay = 1;
int gravMeterPhase = 1;
int sensorIntTime = 200;
float photoSensitivity = 0.1;

/**************************************************
* Function: setup
**************************************************/
void setup()
{
  timers = DualThreeDigitSevenSegmentMux(segmentPins, displayPins);
  
  sensorOne = photoDiode(detectorPins[0], photoSensitivity);
  sensorTwo = photoDiode(detectorPins[1], photoSensitivity);
  
  if (debug)
  {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
    Serial.println("Start");
  }
}

/**************************************************
* Function: loop
**************************************************/
void loop()
{
  currentMillis = millis();
  
  // Read the input
  sensorOne.voltageRead();
  sensorTwo.voltageRead();
  
  delay(loopDelay);
  
  if (gravMeterPhase == 1)
  {
    if (sensorOne.phaseCheck(1))
    {
      if (sensorOne.stateCheck())
      {
        sensorOne.timerStart(currentMillis);
        sensorOne.phaseNext();
      }
      //Else Wait to initialize
      else
        timers.updateSingle(1, -1);
    }
    else if (sensorOne.phaseCheck(2))
    {
      if (sensorOne.stateCheck())
        sensorOne.timerUpdate(currentMillis);
      else
        sensorOne.phaseReset();
        
      if (sensorOne.timerCheck() > sensorIntTime)
        sensorOne.phaseNext();
        
      timers.updateSingle(1,sensorOne.timerCheck());
    }
    else if (sensorOne.phaseCheck(3))
    {
      if (sensorOne.stateCheck())
        timers.updateSingle(1, -2);
      else
        sensorOne.phaseReset();
    }

    if (sensorTwo.phaseCheck(1))
    {
      if (sensorTwo.stateCheck())
      {
        sensorTwo.timerStart(currentMillis);
        sensorTwo.phaseNext();
      }
      //Else Wait to initialize
      else
        timers.updateSingle(2, -1);
    }
    else if (sensorTwo.phaseCheck(2))
    {
      if (sensorTwo.stateCheck())
        sensorTwo.timerUpdate(currentMillis);
      else
        sensorTwo.phaseReset();
        
      if (sensorTwo.timerCheck() > sensorIntTime)
        sensorTwo.phaseNext();
        
      timers.updateSingle(2,sensorTwo.timerCheck());
    }
    else if (sensorTwo.phaseCheck(3))
    {
      if (sensorTwo.stateCheck())
        timers.updateSingle(2, -2);
      else
        sensorTwo.phaseReset();
    }

     if (sensorOne.phaseCheck(3) && sensorTwo.phaseCheck(3))
     {
       gravMeterPhase = 2;
       sensorOne.phaseReset();
       sensorTwo.phaseReset();
       timers.update(-3,-3);
     }
  }
  
  else if (gravMeterPhase == 2)
  {
    if (sensorOne.phaseCheck(1))
    {
      if (sensorOne.stateCheck() == false)
      {
        sensorOne.phaseNext();
        sensorOne.timerStart(currentMillis);
      }
    }
    else if (sensorOne.phaseCheck(2))
    {     
      if (sensorOne.stateCheck())
        sensorOne.phaseNext();
      else
      {
        sensorOne.timerUpdate(currentMillis);
        timers.updateSingle(1,sensorOne.timerCheck());  
      }
    }
    else if (sensorOne.phaseCheck(3))
    {
      if (sensorOne.stateCheck() == false)
        sensorOne.phaseNext();
      else
      {
        sensorOne.timerUpdate(currentMillis);
        timers.updateSingle(1,sensorOne.timerCheck());
      }
    }
    else if (sensorOne.phaseCheck(4))
    {
      if (sensorOne.stateCheck())
        sensorOne.phaseNext();
      else
      {
        sensorOne.timerUpdate(currentMillis);
        timers.updateSingle(1,sensorOne.timerCheck());
      }
    }
    
    if (sensorTwo.phaseCheck(1))
    {
      if (sensorTwo.stateCheck() == false)
      {
        sensorTwo.phaseNext();
        sensorTwo.timerStart(currentMillis);
      }
    }
    else if (sensorTwo.phaseCheck(2))
    {     
      if (sensorTwo.stateCheck())
        sensorTwo.phaseNext();
      else
      {
        sensorTwo.timerUpdate(currentMillis);
        timers.updateSingle(2,sensorTwo.timerCheck());  
      }
    }
    else if (sensorTwo.phaseCheck(3))
    {
      if (sensorTwo.stateCheck() == false)
        sensorTwo.phaseNext();
      else
      {
        sensorTwo.timerUpdate(currentMillis);
        timers.updateSingle(2,sensorTwo.timerCheck());
      }
    }
    else if (sensorTwo.phaseCheck(4))
    {
      if (sensorTwo.stateCheck())
        sensorTwo.phaseNext();
      else
      {
        sensorTwo.timerUpdate(currentMillis);
        timers.updateSingle(2,sensorTwo.timerCheck());
      }
    }
  }
  
  timers.display();
  
  if(debug)
  {
    if(sensorOne.stateCheck())
    {
      Serial.print("vOne: ");
      Serial.println(sensorOne.voltageCheck(),5);
    }
    if(sensorTwo.stateCheck())
    {
      Serial.print("     vTwo: ");
      Serial.println(sensorTwo.voltageCheck(),5);
    }
  }
}

