const int inputPin = A4;

const int sampleSize = 10;

int bottomLimitSwitch = A1;

int dPin1 = 4 ,dPin2 = 2,sPin1 = 5,sPin2 = 3; // 1 for polar , 2 for vertical

int analogValue[200], incomingByte;

boolean isDataTransmitted = true,isStarted = false;

int verticalCount = 0;

int iBC = 0;

void setup(){
  Serial.begin(115200);
  pinMode(dPin1,OUTPUT);
  pinMode(dPin2,OUTPUT);
  pinMode(sPin1,OUTPUT);
  pinMode(sPin2,OUTPUT);

  pinMode(inputPin,INPUT); // for analog Input from IR sensor
  pinMode(bottomLimitSwitch,INPUT);
  
}

void loop(){
      
      if(Serial.available() > 0){
       incomingByte = Serial.read(); 
       if(verticalCount != 0) {
        Serial.println("The rowcount of the matrix is ");
        Serial.print(verticalCount);
       }else {
        Serial.print("The model matrix is below , in order to create 3D model, please copy and paste the matrix to the MATLAB code.");
        Serial.println();
       }
       while(digitalRead(bottomLimitSwitch) == HIGH){ 
        spinDown(true);
        if(iBC < 3){
          rotateBase();
          iBC++;
          }
        }
       isStarted = !isStarted; 
      
      }
       if(isStarted){
        
          spinDown(false);
          digitalWrite(dPin1,HIGH);
          for(int i = 0; i < 200 ; i++){
            digitalWrite(sPin1,HIGH);
            delayMicroseconds(3000);
            digitalWrite(sPin1,LOW);
            delayMicroseconds(3000);
            analogValue[i] = getAnalogValue(sampleSize);
            //delay(1);
            }
            
            for(int i = 0 ; i < 200 ; i++){
              Serial.print(analogValue[i]);
              if ( i != 199) Serial.print(",");
            }
            Serial.print(";");
        verticalCount++;
        if(verticalCount >= 500) {
          Serial.println(verticalCount);
          isStarted = false;
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

int getAnalogValue(int sampleSize){
  int sLimit = 160;
  int arr[10];
  for(int i = 0; i < sampleSize ; i++){
     arr[i] = analogRead(inputPin);
     delay(1);
  }
   //sortArray(arr,sampleSize);
   /*if(arr[sampleSize/2 - 1] < sLimit) {
    Serial.print(sLimit); //debug
    Serial.print(",!"); //debug
    return sLimit;
   }*/
   int tmpSum = 0;
   int count = 0;
   for(int i = 0; i < sampleSize; i++) {
    if(arr[i] != 0){
      tmpSum = tmpSum + arr[i];
      count++;
    }
   }
   //Serial.print(arr[sampleSize/2 - 1]); //debug
   //Serial.print(tmpSum/count);
   //Serial.print(','); //debug
   //return arr[sampleSize/2 - 1];
  return tmpSum/count;
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
