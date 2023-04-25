int buttonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

void setup() {
  pinMode(2, INPUT);
  buttonState = digitalRead(2);
  Serial.begin(9600);
  attachInterrupt(0, buttonPressed, CHANGE);
}

void loop() {
}

void buttonPressed(){
  unsigned long currentMillis = millis();

  if (currentMillis - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentMillis;
    int newState = digitalRead(2);
    if(buttonState != newState){
      Serial.println("Button has been pressed");
      buttonState = newState;
    }
  }
}