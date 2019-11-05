#define GREEN 8
#define YELLOW 9 
#define RED 10

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
pinMode(GREEN, INPUT);
pinMode(YELLOW, INPUT);
pinMode(RED, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);

 //String str = " ";
 //if (digitalRead(GREEN)) str = "1";
 //if (digitalRead(YELLOW)) str = "0";
 //if (digitalRead(RED)) str = "-1";


 
 if (digitalRead(GREEN)) Serial.print("1");
 if (digitalRead(YELLOW)) Serial.print("0");
 if (digitalRead(RED)) Serial.print("-1");


}
