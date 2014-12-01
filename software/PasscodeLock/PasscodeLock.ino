// Supergeek Challenge Passcode Lock
// Randy Glenn - randy@surrealitylabs.com
// November 21, 2014

// Libraries
#include <Password.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>

// LCD setup
LiquidCrystal lcd(2,3,4,5,6,7,8);

// NeoPixel setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, 9, NEO_GRB + NEO_KHZ800);

// Password object
Password password = Password( "2008" );

// Keypad setup
const byte N_ROWS = 4;
const byte N_COLS = 4;
char keyMap[N_ROWS][N_COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[N_ROWS] = {10, 11, 12, 13};
byte colPins[N_COLS] = {14, 15, 16, 17};
Keypad keypad = Keypad( makeKeymap(keyMap), rowPins, colPins, N_ROWS, N_COLS );

void setup(){
  // Set up the LCD
  lcd.begin(20, 4);
  
  // Setup keypad
  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(50);
  
  // Prepare the NeoPixels
  strip.begin();
  
  // Put the display in the initial state
  resetDisplay();
}

void loop(){
  // All of the logic is triggered from what happens when keys are pressed, so this is all that goes in loop()
  keypad.getKey();
}

// Executes when a key is hit
void keypadEvent(KeypadEvent eKey){
  if((keypad.getState()) == PRESSED) {
    // If a key is pressed, print the key
    lcd.print(eKey);
    switch (eKey){
      // The asterisk key is effectively "enter" - it causes the password to be checked
      // The pound key is reset - it clears the entry so the user can try again
      // Any other key, we append it to the passcode to be guessed
      case '*': 
        checkPassword(); 
        break;
      case '#':
        password.reset();
        resetDisplay();
        break;  
      default:
        password.append(eKey);
    }
  }
}

void resetDisplay() {
  // Turn NeoPixels off
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.setPixelColor(1, strip.Color(0, 0, 0));
  strip.setPixelColor(2, strip.Color(0, 0, 0));
  strip.setPixelColor(3, strip.Color(0, 0, 0));
  strip.show();
  
  // Reset the display with the prompt
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SUPERGEEK CHALLENGE ");
  lcd.setCursor(0,1);
  lcd.print("Please enter code,  ");
  lcd.setCursor(0,2);
  lcd.print("press * when done   ");
  lcd.setCursor(0,3);
}

void checkPassword(){
  if (password.evaluate()){
    // Correct code - set the NeoPixels to Blue
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.setPixelColor(1, strip.Color(0, 0, 255));
    strip.setPixelColor(2, strip.Color(0, 0, 255));
    strip.setPixelColor(3, strip.Color(0, 0, 255));
    strip.show();
    
    // Show the success message
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("********************");
    lcd.setCursor(0,1);
    lcd.print("**  CODE CORRECT  **");
    lcd.setCursor(0,2);
    lcd.print("**   WAY TO GO!   **");
    lcd.setCursor(0,3);
    lcd.print("********************");
    
    // Set up for the next round, and after 5 seconds, reset
    password.reset();
    delay(5000);
    lcd.clear();
  } else {
    // Incorrect code - set the NeoPixels to Red
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.setPixelColor(1, strip.Color(255, 0, 0));
    strip.setPixelColor(2, strip.Color(255, 0, 0));
    strip.setPixelColor(3, strip.Color(255, 0, 0));
    strip.show();
    
    // Show the incorrect code message
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("********************");
    lcd.setCursor(0,1);
    lcd.print("** INCORRECT CODE **");
    lcd.setCursor(0,2);
    lcd.print("**   TRY AGAIN!   **");
    lcd.setCursor(0,3);
    lcd.print("********************");
    
    // Set up for the next rount, and after 5 seconds, reset
    password.reset();
    delay(5000);
    lcd.clear();
  }
  resetDisplay();
}
