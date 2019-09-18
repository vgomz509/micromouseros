uint8_t a1=0;
uint8_t b1=0;
uint8_t c1=0;
uint8_t d1=0;

uint8_t a2=0;
uint8_t b2=0;
uint8_t c2=0;
uint8_t d2=0;


uint32_t pulsos1=0;
uint32_t pulsos2=0;

uint32_t pasos=0;

#define e1 14
#define e2 27
#define e3 16
#define e4 17

const int freq = 5000;
const int ledChannel0 = 0;
const int ledChannel1 = 1;
const int ledChannel2 = 2;
const int ledChannel3 = 3;

const int resolution = 8;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(e1, INPUT); // encoder Right 1
 pinMode(e2, INPUT); // encoder Right 2
 pinMode(e3, INPUT); // encoder Left 1
 pinMode(e4, INPUT); // encoder Left 2
 
ledcSetup(ledChannel0, freq, resolution);
ledcAttachPin(02, ledChannel0);
ledcSetup(ledChannel1, freq, resolution);
ledcAttachPin(04, ledChannel1);

ledcSetup(ledChannel2, freq, resolution);
ledcAttachPin(05, ledChannel2);
ledcSetup(ledChannel3, freq, resolution);
ledcAttachPin(15, ledChannel3);


ledcWrite(ledChannel0, 0);//232
 ledcWrite(ledChannel1, 0);
ledcWrite(ledChannel2, 0);//255
 ledcWrite(ledChannel3, 0);
}

void loop() {

 if (pasos%100000==0){Serial.print(pulsos1);Serial.print(",");Serial.println(pulsos2);}
 pasos++;

  uint8_t c2=digitalRead(e3);
 uint8_t d2=digitalRead(e4); 
 uint8_t c1=digitalRead(e1);
 uint8_t d1=digitalRead(e2);

 
 if ((a1!=c1)||(b1!=d1)){
  if ((a1==0)&&(b1==0)&&(c1==1)&&(d1==0)){
      pulsos1++;
    }
  if ((a1==1)&&(b1==0)&&(c1==1)&&(d1==1)){
      pulsos1++;
    }
  if ((a1==1)&&(b1==1)&&(c1==0)&&(d1==1)){
      pulsos1++;
    }
  if ((a1==0)&&(b1==1)&&(c1==0)&&(d1==0)){
      pulsos1++;
    }
  if ((a1==0)&&(b1==0)&&(c1==0)&&(d1==1)){
      pulsos1--;
    }
if ((a1==0)&&(b1==1)&&(c1==1)&&(d1==1)){
      pulsos1--;
    }
if ((a1==1)&&(b1==1)&&(c1==1)&&(d1==0)){
      pulsos1--;
    }
if ((a1==1)&&(b1==0)&&(c1==0)&&(d1==0)){
      pulsos1--;
    }
      a1=c1;
      b1=d1;
      
}


 
 if ((a2!=c2)||(b2!=d2)){
  if ((a2==0)&&(b2==0)&&(c2==1)&&(d2==0)){
      pulsos2++;
    }
  if ((a2==1)&&(b2==0)&&(c2==1)&&(d2==1)){
      pulsos2++;
    }
  if ((a2==1)&&(b2==1)&&(c2==0)&&(d2==1)){
      pulsos2++;
    }
  if ((a2==0)&&(b2==1)&&(c2==0)&&(d2==0)){
      pulsos2++;
    }
  if ((a2==0)&&(b2==0)&&(c2==0)&&(d2==1)){
      pulsos2++;
    }
if ((a2==0)&&(b2==1)&&(c2==1)&&(d2==1)){
      pulsos2--;
    }
if ((a2==1)&&(b2==1)&&(c2==1)&&(d2==0)){
      pulsos2--;
    }
if ((a2==1)&&(b2==0)&&(c2==0)&&(d2==0)){
      pulsos2--;
    }
      a2=c2;
      b2=d2;
}


}
