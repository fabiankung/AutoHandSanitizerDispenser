// 6 March 2020
// Fabian Kung
// Automatic Hand Sanitizer Dispenser Prototype
//
int pA4988DIR = 2;
int pA4988STEP = 3;
int pLED = 12;
int pIRSensor = 0;
int pA4988Enable = 4;

#define   _UP   HIGH
#define   _DOWN LOW
#define   _ENABLE LOW
#define   _DISABLE HIGH
#define   _IRSEN_THRESHOLD  250

int nIRSensor;
int nPressCount = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pA4988DIR, OUTPUT);
  pinMode(pA4988STEP, OUTPUT);
  pinMode(pA4988Enable, OUTPUT);
  digitalWrite(pA4988DIR, HIGH);
  digitalWrite(pA4988Enable, _DISABLE); // Under normal condition we should disable the A4988 module,
                                        // this would cut off the current to the stepper motor, prevent
                                        // wastage of electric power and reduce heat dissipation on the
                                        // A4988 chip.
  Serial.begin(9600);
}

void loop() {

 // Read IR distance sensor output.
 nIRSensor = analogRead(pIRSensor);    
 Serial.println(nIRSensor);
 delay(100);
 
 if (nIRSensor > _IRSEN_THRESHOLD)      // Check if threshold is crossed.
 {
  nIRSensor = analogRead(pIRSensor);    // Reread the IR distance sensor output and recheck again.
  {
    if (nIRSensor > _IRSEN_THRESHOLD)
    {
      Press();        // Activate press and release actions.
      nPressCount++;  // Increment press counter.     
    }
  }
 }
}

void Press(void)
{
  int nCount;

 digitalWrite(pA4988Enable, _ENABLE); // Enable the A4988 chip and wait for a short while 
 delay(50);                           // for it to stabilize before we proceed.
 
  // Lower the press.
 digitalWrite(pA4988DIR, _DOWN);   // Set press direction.
 digitalWrite(pLED, HIGH);    // Lights up indicator LED.
 for (nCount = 0; nCount<2400; nCount++)
 {
  digitalWrite(pA4988STEP, HIGH);
  delayMicroseconds(50);
  digitalWrite(pA4988STEP, LOW);
  delayMicroseconds(450);
 }
 delay(300);
 // Retract the press.
 digitalWrite(pA4988DIR, _UP);
 for (nCount = 0; nCount<2400; nCount++)
 {
  digitalWrite(pA4988STEP, HIGH);
  delayMicroseconds(50);
  digitalWrite(pA4988STEP, LOW);
  digitalWrite(pLED, LOW);
  delayMicroseconds(450);   // Off indicator LED.
 } 
 delay(50);                             // Delay and then disable the A4988 chip.
 digitalWrite(pA4988Enable, _DISABLE);
}
