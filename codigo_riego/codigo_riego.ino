//Código prototipo sistema de riego temporizado en Arduino

/*Pines definidos*/
#define Moto_Bomba 32//Pin donde se da la salida de la motobomba
#define RXD2 16//Pin de recepción del protocolo Serial2
#define TXD2 17//Pin de transmisión del protocolo Serial2

/*********************************************************
 * Variables Globales
 ********************************************************/
char inChar;
String string = "";

//Variables para contadores
unsigned long tiempo_inicio;
unsigned long tiempo_riego;
unsigned long tiempo_actual;
unsigned long tiempo_serial;
unsigned long tiempo_inicio_serial;

//Variables modificables de los tiempos de: La duración del riego, tiempo entre riegos y envió de datos por serial del sensor
long int periodo_riego = 1*259200*1000;//259200 equivale a dos dias
long int duracion_riego = 1*1200*1000;//1200 equivale a 20 minutos
long int periodo_envio = 1*60*1000;//60 equivale a 1 minuto

String Tem_hum[3];//Variable que recibe las lecturas del Sensor de Teros
bool riego_activo = false;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.print("-----Inicio Prototipo riego UPB----- "); 
  pinMode(Moto_Bomba, OUTPUT);
  tiempo_inicio = millis();
  tiempo_inicio_serial = millis();
}

void loop() {
  sensor();
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

  tiempo_serial = millis();
  if(tiempo_serial - tiempo_inicio_serial >= periodo_envio){
    double humedad = atof(Tem_hum[0].c_str());
    //double humedad_real=((4.3e-6*pow(humedad,3))-(5.5e-4*pow(humedad,2))+(2.92e-2*humedad)-(5.3e-2))*100;// para ech20
    double humedad_real=((3.879e-4*humedad)-0.6956)*100;//para teros12: ecucación para obtener humedad del suelo
    double temperatura = atof(Tem_hum[1].c_str());
    Serial.print("Humedad Suelo: ");
    Serial.print(humedad_real);
    Serial.print(" %, ");
    Serial.print("Temperatura Suelo: ");
    Serial.print(temperatura);
    Serial.println(" C");
    tiempo_inicio_serial = tiempo_serial;
  }
}

/*********************************************************
 * Lectura de sensor de humedad del suelo
 ********************************************************/
void sensor() {
   while (Serial2.available()) {
    inChar = Serial2.read();
    if (inChar == '\n') {
      //Tem_hum[0] = getValue(string, '+', 1);//para ech20
      //Tem_hum[1] = getValue(string, '+', 3);//para ech20
      Tem_hum[0] = getValue(string, '+', 1);//Para teros12
      Tem_hum[1] = getValue(string, '+', 2);//Para teros12
      string = "";
    } else {
      string += inChar;
    }
  }
}

/*********************************************************
 * Lectura de comunicación Serial entre Arduino y ESP32
 ********************************************************/
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "-1";
}
