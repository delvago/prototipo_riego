#define Moto_Bomba 32
unsigned long tiempo_inicio;
unsigned long tiempo_riego;
unsigned long tiempo_actual;
long int periodo_riego = 1*259200*1000;//259200 equivale a dos dias
long int duracion_riego = 1*1200*1000;//1200 equivale a 20 minutos
bool riego_activo = false;

void setup() {
  Serial.begin(115200);
  Serial.print("-----Inicio Prototipo riego UPB----- "); 
  pinMode(Moto_Bomba, OUTPUT);
  tiempo_inicio = millis();
}

void loop() {
  tiempo_actual = millis();
  if(tiempo_actual - tiempo_inicio >= periodo_riego){
    if (!riego_activo){
      digitalWrite(Moto_Bomba, HIGH);
      riego_activo = true;
      tiempo_riego = millis();
    }

    if(tiempo_actual - tiempo_riego >= duracion_riego){
      digitalWrite(Moto_Bomba, LOW);
      riego_activo = false;
      tiempo_inicio = tiempo_actual;
    }
  }
}
