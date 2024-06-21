//1 june 2024
#include <Wire.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "setup_pins.h"
#include "functions.h"

//button and lcd
LiquidCrystal_I2C lcd(0x3E, 20, 4);

//page
enum MenuState {
  WELCOME,
  MODE_SELECTION,
  MANUAL_MENU1,
  MANUAL_MENU2,
  MANUAL_MENU3,
  MANUAL_MENU4,
  MANUAL_MENU5,
  AUTO_MENU1,
  AUTO_MENU2,
  MANUAL_PUMP,
  RECORD,
  SAVE,
  RESULT,
};

MenuState currentState = WELCOME;


//memory eeprom setting
const int rowCount = 3;
const int colCount = 30;
int userInput[rowCount][colCount]; // 2D Array to store user inputs
int currentRow = 0;
int inputIndex = 0;
const int eepromAddress = 0;// EEPROM address to store the data

//time memory
int timestamps[rowCount][colCount];
const int timestampEepromAddress = eepromAddress + rowCount * colCount;

void setup() {
  Serial.begin(9600); 
  setupPins(); 
  
  //memory
  loadArrayFromEEPROM();
  loadTimestampsFromEEPROM();
  
  //lcd
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.noBlink();

  //Initial condition
  controlDCMotor(0,  false, true);
  digitalWrite(pump1, HIGH);
  digitalWrite(pump2, HIGH);

}

void loop() {
  switch (currentState) {
    case WELCOME:
      lcd.setCursor(0, 0);
      lcd.print("Welcome to Autochef");
      lcd.setCursor(0, 1);
      lcd.print("1. Manual");
      lcd.setCursor(0, 2);
      lcd.print("2. Auto");
      handleButtonPress(1, RECORD);
      handleButtonPress(2, AUTO_MENU1);
      break;
    
    case RECORD:
      lcd.setCursor(0, 0);
      lcd.print("SAVE COOKING?");
      lcd.setCursor(0, 1);
      lcd.print("1. Yes");
      lcd.setCursor(0, 2);
      lcd.print("2. No");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev Page");
      handleManualCook(2, RECORD);
      
      handleButtonPress(1, SAVE);
      handleButtonPress(4, WELCOME);
      break;

    case SAVE:
      lcd.setCursor(0, 0);
      lcd.print("Choose Slot:");
      lcd.setCursor(0, 1);
      lcd.print("1. Menu 1");
      lcd.setCursor(0, 2);
      lcd.print("2. Menu 2");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev Page");
      handleManualCook(1, SAVE);
      handleManualCook(2, SAVE);
      
      handleButtonPress(4, RECORD);
      break;
    
    case MANUAL_MENU1:
      lcd.setCursor(0, 0);
      lcd.print("Manual Mode Menu:");
      lcd.setCursor(0, 1);
      lcd.print("1. Pump 1 On");
      lcd.setCursor(0, 2);
      lcd.print("2. Pump 2 On");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev 5. Next");
      handleManualCook(1, MANUAL_MENU1);
      handleManualCook(2, MANUAL_MENU1);
      
      handleButtonPress(4, WELCOME);
      handleButtonPress(5, MANUAL_MENU2);
      break;

    case MANUAL_MENU2:
      lcd.setCursor(0, 0);
      lcd.print("1. Heat High");
      lcd.setCursor(0, 1);
      lcd.print("2. Heat Low");
      lcd.setCursor(0, 2);
      lcd.print("3. Heat Off");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev 5. Next");
      handleManualCook(1, MANUAL_MENU2);
      handleManualCook(2, MANUAL_MENU2);
      handleManualCook(3, MANUAL_MENU2);
      
      handleButtonPress(4, MANUAL_MENU1);
      handleButtonPress(5, MANUAL_MENU3);
      break;

    case MANUAL_MENU3:
      lcd.setCursor(0, 0);
      lcd.print("1. NextIngredient");
      lcd.setCursor(0, 1);
      lcd.print("2. Stir On");
      lcd.setCursor(0, 2);
      lcd.print("3. Stir Off");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev 5. Next");
      handleManualCook(1, MANUAL_MENU3);
      handleManualCook(2, MANUAL_MENU3);
      handleManualCook(3, MANUAL_MENU3);
      
      handleButtonPress(4, MANUAL_MENU2);
      handleButtonPress(5, MANUAL_MENU4);
      break;
      
   
    case MANUAL_MENU4:
      lcd.setCursor(0, 0);
      lcd.print("1. off all");
      lcd.setCursor(0, 1);
      lcd.print("2. SAVE, HOME");
      lcd.setCursor(0, 2);
      lcd.print("3. NO SAVE, HOME");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev Page");
      handleManualCook(1, MANUAL_MENU4);
      handleManualCook(2, MANUAL_MENU4);
      handleManualCook(3, MANUAL_MENU4);
      
      handleButtonPress(4, MANUAL_MENU3);
      break;
    
    

    case AUTO_MENU1:
      lcd.setCursor(0, 0);
      lcd.print("Auto Mode Menu:");
      lcd.setCursor(0, 1);
      lcd.print("1.Menu1");
      for (int i = 0; i < 10; i++) {
        lcd.setCursor(i+8, 1);
        lcd.print(userInput[1][i]);
      }
      lcd.setCursor(0, 2);
      lcd.print("2.Menu2");
      for (int i = 0; i < 10; i++) {
        lcd.setCursor(i+8, 2);
        lcd.print(userInput[2][i]);
      }
      lcd.setCursor(0, 3);
      lcd.print("4. Prev 5. Next");
      handleManualCook(1, AUTO_MENU1);
      handleManualCook(2, AUTO_MENU1);
      handleButtonPress(4, WELCOME);
      handleButtonPress(5, AUTO_MENU2);
      break;
    
    case AUTO_MENU2:
      lcd.setCursor(0, 0);
      lcd.print("1. Menu 3");
      lcd.setCursor(0, 1);
      lcd.print("2. Menu 4");
      lcd.setCursor(0, 2);
      lcd.print("3. Menu 5");
      lcd.setCursor(0, 3);
      lcd.print("4. Prev Page");
      handleButtonPress(1, MANUAL_PUMP);
      handleButtonPress(2, MANUAL_PUMP); // Placeholder for selecting menu in auto mode
      handleButtonPress(1, MANUAL_PUMP);
      handleButtonPress(4, AUTO_MENU1);
      break;

    case MANUAL_PUMP:
      lcd.setCursor(0, 0);
      lcd.print("1. HOME:");
      handleButtonPress(1, WELCOME);
      break;
    case RESULT:
      if (currentRow == 0) {
        lcd.clear();
        currentState = WELCOME;
        break; // Exit the loop
      }else {
        displayRow(currentRow);
        lcd.setCursor(0, 3);
        lcd.print("5. HOME");
        handleButtonPress(5, WELCOME);
        }
        
      break;
  }
}

void handleButtonPress(int option, MenuState nextState) {
  int buttonState = digitalRead(buttonPin[option - 1]);
  if (buttonState == LOW && !buttonPressed[option - 1]) {
    lcd.clear(); // Clear LCD before printing new content
    buttonPressed[option - 1] = true;
    currentState = nextState;
    delay(200); // Debounce delay
  } else if (buttonState == HIGH && buttonPressed[option - 1]) {
    buttonPressed[option - 1] = false;
  }
}

void handleManualCook(int option, MenuState nextState) {
  int buttonState = digitalRead(buttonPin[option - 1]);
  if (buttonState == LOW && !buttonPressed[option - 1]) {
    buttonPressed[option - 1] = true;
    delay(200); // Debounce delay

    // Handle different menu options based on the current state
    switch (currentState) {
      case RECORD:
        if (option == 2) {
          option = 0; //for NO SAVE option
          inputIndex = 0;
          resetRow(option);
          currentRow=option ;
          currentState =  MANUAL_MENU1;
        } 
        break;
      case SAVE:
        if (option == 1) {
          inputIndex = 0;
          resetRow(option);
          currentRow=option ;
          currentState =  MANUAL_MENU1;
        } else if (option == 2) {
          inputIndex = 0;
          resetRow(option);
          currentRow=option ;
          currentState =  MANUAL_MENU1;
        }
        break;
      case MANUAL_MENU1:
        if (option == 1) {
        userInput[currentRow][inputIndex] = 1;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;
        
        liquidRelay(pump1, 5000);
        } else if (option == 2) {
        userInput[currentRow][inputIndex] = 2;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;
        
        liquidRelay(pump2, 5000);         
        }
        break;
      case MANUAL_MENU2:
        if (option == 1) {
        userInput[currentRow][inputIndex] = 3;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;  
        setHeating(1); // High heat       
        } else if (option == 2) {
        userInput[currentRow][inputIndex] = 4;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++; 
        setHeating(2); // Low heat      
        } else if (option == 3) {
        userInput[currentRow][inputIndex] = 5;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;  
        setHeating(3); // Heat off     
        }
        break;
      case MANUAL_MENU3:
        if (option == 1) {
        userInput[currentRow][inputIndex] = 6;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;       
        } else if (option == 2) {
        userInput[currentRow][inputIndex] = 7;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++; 
        controlDCMotor(200, true, true);   
        } else if (option == 3) {
        userInput[currentRow][inputIndex] = 8;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;  
        controlDCMotor(0, false, true); 
        }
        break;
      case MANUAL_MENU4:
        if (option == 1) {
        userInput[currentRow][inputIndex] = 9;
        timestamps[currentRow][inputIndex] = millis() / 1000; // Record timestamp
        inputIndex++;

        shutAll();
          
        }else if (option == 2) {
        shutAll();
        saveArrayToEEPROM(currentRow);
        saveTimestampsToEEPROM(currentRow);//
        lcd.clear();
        currentState = RESULT;
        }else if (option == 3) {
        shutAll(); 
        lcd.clear();
        currentState =  WELCOME;
        }
        break;
      case AUTO_MENU1:
        if (option == 1) {
          //unsigned long startTime = millis() / 1000;
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("cooking>>> MENU ");
          lcd.setCursor(18, 1);
          lcd.print(option);
  
          for (int i = 0; i < colCount; i++) {
        
            // Check if the array value is 0
          if (userInput[option][i] == 0) {
            lcd.clear();
            currentState = WELCOME;
            break; // Exit the loop
          }
            
            // Perform actions based on the userInput values
            switch (userInput[option][i]) {
              case 1:
                liquidRelay(1, 5000);
                Serial.print(" pump. on");
                break;
              case 2:
                liquidRelay(2, 5000);
                Serial.print(" pump. off");
                break;
              case 3:
                setHeating(1);
                Serial.print(" heat high");
                break;
              case 4:
                setHeating(2);
                Serial.print(" heat low");
                break;
              case 5:
                setHeating(3);
                Serial.print(" heat stop");
                break;
              case 6:
                setHeating(1);
                Serial.print(" heat high");
                break;
              case 7:
                controlDCMotor(200, true, true);
                Serial.print(" stir on");
                break;
              case 8:
                controlDCMotor(0, false, true);
                Serial.print(" stir off");
                break;
              case 9:
                shutAll();
                Serial.print(" stop all");
                break;
              default:
                // Do nothing if the value is not recognized
                break;
            } // Calculate the delay based on the difference between the current timestamp and the next one
              if (i < colCount - 1 && timestamps[option][i + 1] != 0) {
                unsigned long waitTime = (timestamps[option][i + 1] - timestamps[option][i]) * 1000;
                Serial.print(waitTime);
                delay(waitTime);
              }
        
          }       //for safety 
                  shutAll();
                  currentState =  WELCOME;
        } else if (option == 2) {
          //unsigned long startTime = millis() / 1000;
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("cooking>>> MENU ");
          lcd.setCursor(18, 1);
          lcd.print(option);
  
          for (int i = 0; i < colCount; i++) {
        
            // Check if the array value is 0
          if (userInput[option][i] == 0) {
            lcd.clear();
            currentState = WELCOME;
            break; // Exit the loop
          }
            
            // Perform actions based on the userInput values
            switch (userInput[option][i]) {
              case 1:
                liquidRelay(1, 5000);
                Serial.print(" pump. on");
                break;
              case 2:
                liquidRelay(2, 5000);
                Serial.print(" pump. off");
                break;
              case 3:
                setHeating(1);
                Serial.print(" heat high");
                break;
              case 4:
                setHeating(2);
                Serial.print(" heat low");
                break;
              case 5:
                setHeating(3);
                Serial.print(" heat stop");
                break;
              case 6:
                setHeating(1);
                Serial.print(" heat high");
                break;
              case 7:
                controlDCMotor(200, true, true);
                Serial.print(" stir on");
                break;
              case 8:
                controlDCMotor(0, false, true);
                Serial.print(" stir off");
                break;
              case 9:
                shutAll();
                Serial.print(" stop all");
        break;
      default:
        // Do nothing if the value is not recognized
        break;
    } 
      // Calculate the delay based on the difference between the current timestamp and the next one
  if (i < colCount - 1 && timestamps[option][i + 1] != 0) {
    unsigned long waitTime = (timestamps[option][i + 1] - timestamps[option][i]) * 1000;
    Serial.print(waitTime);
    delay(waitTime);
  }

  }       //for safety 
          shutAll();
          currentState =  WELCOME;
        }
        break;
    }
    }else if (buttonState == HIGH && buttonPressed[option - 1]) {
    buttonPressed[option - 1] = false;
  } 
  }

 

void saveArrayToEEPROM(int currentRow) {
  int address = eepromAddress + currentRow * colCount;//
    for (int j = 0; j < colCount; j++) {
      EEPROM.write(address, userInput[currentRow][j]);
      address++;
    }
}


void loadArrayFromEEPROM() {
  int address = eepromAddress;
  for (int i = 0; i < rowCount; i++) {
    for (int j = 0; j < colCount; j++) {
      userInput[i][j] = EEPROM.read(address);
      address++;
    }
  }
}

void displayRow(int row) {
  for (int i = 0; i < colCount; i++) {
    lcd.setCursor(i, 0);
    lcd.print(userInput[row][i]);
  }
}

void resetRow(int row) {
  for (int i = 0; i < colCount; i++) {
    userInput[row][i] = 0;
    timestamps[row][i] = 0;//
  }
}

void saveTimestampsToEEPROM(int currentRow) {
  int address = timestampEepromAddress + currentRow * colCount;
  for (int j = 0; j < colCount; j++) {
    EEPROM.write(address, timestamps[currentRow][j]);
    address++;
  }
}

void loadTimestampsFromEEPROM() {
  int address = timestampEepromAddress;
  for (int i = 0; i < rowCount; i++) {
    for (int j = 0; j < colCount; j++) {
      timestamps[i][j] = EEPROM.read(address);
      address++;
    }
  }
}
