#define GREEN 8
#define YELLOW 9 
#define BLUE 10

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
pinMode(GREEN, INPUT);
pinMode(YELLOW, INPUT);
pinMode(BLUE, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  delay(200);

  Serial.print("GREEN: "); Serial.println(digitalRead(GREEN));
  Serial.print("YELLOW: "); Serial.println(digitalRead(YELLOW));
  Serial.print("BLUE: "); Serial.println(digitalRead(BLUE));

}
