#define Moto_Bomba 32
void setup() {
  Serial.begin(115200);
  Serial.print("-----Inicio Prototipo riego UPB----- "); 
  pinMode(Moto_Bomba, OUTPUT);
}

void loop() {
  
  digitalWrite(Moto_Bomba, HIGH);
  digitalWrite(Moto_Bomba, LOW);

}
