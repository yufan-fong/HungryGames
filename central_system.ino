int sensorPin1 = A0;
int sensorPin2 = A1;
int sensorPin3 = A2;
int digitalPin1 = 8;
int digitalPin2 = 9;
int digitalPin3 = 10;
int digital_delay = 200;                //wait after switching digitalPin to HIGH
int pushbutton = 7;
int push_value = 0;

int reading_array[9];                   //array to store the 9 light reading values from the LDRs
char state_array[9][1];                 //9 elements, each element has 1char+1end char
char order_no[10];
String order_string;
String order_array[]= {"212000000", "002120000", "002110000", "101000020", "210001000"};               //compilation of order_no that will be stored beforehand
char dish_array[][17] = {"Fish 'n' Chips", "Grilled Salmon", "Grilled Dory", "CS Wanton Noodle", "Mushroom Soup"};    //3 elements in the array, each with 16 char+1end char to fit in 16x2 LCD

const int OUT_PIN = A4;
const int IN_PIN = A3;
const float IN_STRAY_CAP_TO_GND = 33;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;  
const int MAX_ADC_VALUE = 1023;

#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


 static bool state = 0;

void setup() {

  lcd.begin(16,2);
  
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  
  Serial.begin(9600);
  pinMode(digitalPin1, OUTPUT);          //set each digitalPin to distribute power
  pinMode(digitalPin2, OUTPUT);
  pinMode(digitalPin3, OUTPUT);
  pinMode(A0, INPUT);                    //set each analog pin to receive LDR reading
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(pushbutton, INPUT);
  digitalWrite(pushbutton, HIGH);
}

void loop() {

  if (state == 0){
      checkphone();
  } else if(state == 1){
    checkdish();
  }
}  
 

void checkphone(){

  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);

  if (val < 1000){
    pinMode(IN_PIN, OUTPUT);
    delay(500);
    float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

    if (capacitance < 900) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please Insert");
    
    lcd.setCursor(0,1);
    lcd.print("Your Phone :)");
    Serial.print("No Phone: ");
    Serial.print(capacitance);
    Serial.println(" pF");
    delay(1000);
    state = 0;
    }
    
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Phone Detected!");
      Serial.print("With Phone: ");
      Serial.print(capacitance);
      Serial.println(" pF");
      delay(1000);
      state = 1;
    }
}}

void checkdish(){
      
      push_value = digitalRead(pushbutton);
      if (push_value == LOW){
        digitalWrite(digitalPin1, HIGH);                   //turn on D10
        delay(digital_delay);                          //wait before storing LDR reading
        reading_array[0] = analogRead(sensorPin1); 
        reading_array[1] = analogRead(sensorPin2);
        reading_array[2] = analogRead(sensorPin3);
        digitalWrite(digitalPin1, LOW);
      
        digitalWrite(digitalPin2, HIGH);                   //repeat for D11
        delay(digital_delay);
        reading_array[3] = analogRead(sensorPin1);
        reading_array[4] = analogRead(sensorPin2);
        reading_array[5] = analogRead(sensorPin3);
        digitalWrite(digitalPin2, LOW);
       
        digitalWrite(digitalPin3, HIGH);                   //repeat for D12
        delay(digital_delay);
        reading_array[6] = analogRead(sensorPin1);
        reading_array[7] = analogRead(sensorPin2);
        reading_array[8] = analogRead(sensorPin3);
        digitalWrite(digitalPin3, LOW);
      
        Serial.println("--------");                 //print out LDR values on serial monitor to check whether LDR working
        Serial.print("G1 = ");
        Serial.print(reading_array[0]);
        Serial.print(" G2 = ");
        Serial.print(reading_array[1]);
        Serial.print(" G3 = ");
        Serial.println(reading_array[2]);
        Serial.print("G4 = ");
        Serial.print(reading_array[3]);
        Serial.print(" G5 = ");
        Serial.print(reading_array[4]);
        Serial.print(" G6 = ");
        Serial.println(reading_array[5]);
        Serial.print("G7 = ");
        Serial.print(reading_array[6]);
        Serial.print(" G8 = ");
        Serial.print(reading_array[7]);
        Serial.print(" G9 = ");
        Serial.println(reading_array[8]);
      
        for(int i = 0; i<9; i++){                     //group reading values into 2,1,0
          if (reading_array[i] >= 200) {              //high light reading, no playing chip
            state_array[i][0] = '0';                  
            } 
            else if (reading_array[i] >= 80) {       //medium light reading, translucent chip, i.e. "O"
            state_array[i][0] = '1';
            } 
            else {
            state_array[i][0] = '2';                  //low light reading, opaque chip, i.e. "X"
            }
            order_no[i] = state_array[i][0];          //combine elements in state_array to form order_no (1 string with 9 char)
            }
      
      order_string = String(order_no);
      
      Serial.println(order_string);                 //check order_string
    
      boolean check = false;
      
      for (int i = 0; i < 5; i++) {
        if (order_array[i] == order_string){
          check = true;
          Serial.println(dish_array[i]);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("You Have Ordered");
          lcd.setCursor(0,1);
          lcd.print(dish_array[i]);
          break;
        }
      } 
      
      if(check == false){
        Serial.println("Please Scan Your Order");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Please Scan");
        lcd.setCursor(0,1);
        lcd.print("Your Order");
        }
      delay(1000);

     }
      else{
        Serial.println("Press Button to Scan");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Press Button to");
        lcd.setCursor(0,1);
        lcd.print("Scan");
        delay(1000);
      }
}
