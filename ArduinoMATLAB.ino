
const int inputPin = A5;

const int sampleSize = 10;

int bottomLimitSwitch = A1;

int dPin1 = 4 ,dPin2 = 2,sPin1 = 5,sPin2 = 3; // 1 for polar , 2 for vertical

char analogValue[200], incomingByte;

boolean isDataTransmitted = true,isStarted = false;

int iBC = 0;

void setup(){
  Serial.begin(115200);
  
  pinMode(dPin1,OUTPUT);
  pinMode(dPin2,OUTPUT);
  pinMode(sPin1,OUTPUT);
  pinMode(sPin2,OUTPUT);

  pinMode(inputPin,INPUT); // for analog Input from IR sensor
  pinMode(bottomLimitSwitch,INPUT);
  
  while(digitalRead(bottomLimitSwitch) == HIGH){ 
    spinDown(true);
    if(iBC < 3){
      rotateBase();
      iBC++;
      }
  }
  
}

void loop(){
      
      if(!isStarted && Serial.available() > 0){
       incomingByte = Serial.read();
       isStarted = true; 
      }
       if(isStarted){
        if(isDataTransmitted){
          spinDown(false);
          digitalWrite(dPin1,HIGH);
          for(int i = 0; i < 200 ; i++){
            digitalWrite(sPin1,HIGH);
            delayMicroseconds(1500);
            digitalWrite(sPin1,LOW);
            delayMicroseconds(1500);
            analogValue[i] = getAnalogValue(sampleSize);
            delay(1);
            }
          isDataTransmitted = false;
          }
        else{
          while(Serial.available()){
            incomingByte = Serial.read();
            for(int i = 0 ; i < 200 ; i++){
              Serial.print(analogValue[i]);
            }
            isDataTransmitted = true;
        }
        }
       }
        
    }

void spinDown(boolean d){ // LOW for down
        if(d)digitalWrite(dPin2,HIGH);
        else digitalWrite(dPin2,LOW);
      
      for(int i = 0; i < 50 ; i++){
        digitalWrite(sPin2,HIGH);
        delayMicroseconds(2000);
        digitalWrite(sPin2,LOW);
        delayMicroseconds(2000);
      }
}

void rotateBase(){
      for(int i = 0; i < 200 ; i++){
            digitalWrite(sPin1,HIGH);
            delayMicroseconds(1200);
            digitalWrite(sPin1,LOW);
            delayMicroseconds(1200);
            delay(1);
      }
}

char getAnalogValue(int sampleSize){
  int sLimit = 160;
  int arr[10];
  for(int i = 0; i < sampleSize ; i++){
     arr[i] = analogRead(inputPin);
     delay(1);
  }
   sortArray(arr,sampleSize);
   if(arr[sampleSize/2 - 1] < sLimit) return sLimit;
   return arr[sampleSize/2 - 1];
  
}
void sortArray(int arr[],int lenght){
  float tmp;
  for(int i = 0 ; i < lenght ; i++){
    for(int j = 0 ; j < lenght ; j++){
        if(arr[i] > arr[j]){
          tmp = arr[i];
          arr[i] = arr[j];
          arr[j] = tmp;
        }
    }
  }
}
