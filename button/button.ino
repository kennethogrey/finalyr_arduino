int buttonState = 0;
void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, buttonPressed, CHANGE);
}

void loop() {
}

void buttonPressed(){
  buttonState = digitalRead(2);
  if(buttonState == 1){
    delay(10);
    buttonState = digitalRead(2);
    if(buttonState == 0){
      Serial.println("Button has been pressed");
    }
  }else{
    delay(100);
  }
}