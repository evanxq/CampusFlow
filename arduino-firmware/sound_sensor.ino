
short soundArray [2500];
const int measuresPerSec = sizeof(soundArray) / sizeof(soundArray[0]);

//record average measurement and average peak value per millisecond
//return number counted
void recordSoundSecond() {
  //unsigned long tStart = micros();
  unsigned long timer = millis();
  //float microsPerMeasure = 1000000.0 / measuresPerSec;
  for(int i = 0; i < measuresPerSec; i++) {
    soundArray[i] = analogRead(SOUND_PIN);
   // while(micros() < tStart + microsPerMeasure*i) { }
  }
  Serial.println((millis()-timer));
  delay(1000- (millis() - timer));
}

void updateSoundVars() {
  soundAvg = getSoundAvg();
  soundAvgSqr = getSoundAvgSqr();
  soundPeakAvg = getSoundPeakAvg();
  soundDeviation = getSoundDeviation(soundAvg);
  
  rollingSoundAvg = soundRollingAvg(soundAvg);
  rollingSoundAvgSqr = soundRollingAvgSqr(soundAvgSqr);
  rollingSoundPeakAvg = soundRollingPeakAvg(soundPeakAvg);
  rollingSoundDeviation = soundRollingDeviation(soundDeviation);
}

int getSoundAvg() {
  unsigned long sum = 0;
  for(int i = 0; i < measuresPerSec; i++) {
    sum += soundArray[i];
  }
  return sum / measuresPerSec;
}

//average of the highest 5 measurements this second
int getSoundPeakAvg() {
  long sum = 0;
  for(int i = 0; i < 5; i++) {
    int peak = 0;
    for(int j = 0; j < measuresPerSec/5; j++) {
      peak = max(peak, soundArray[32*i+j]);
    }
    sum += peak;
  }
  return sum / 5;
}

int getSoundAvgSqr() {
  unsigned long sum = 0;
  for(int i = 0; i < measuresPerSec; i++) {
    sum += soundArray[i] * soundArray[i];
  }
  return sqrt(sum / measuresPerSec);
}

int getSoundDeviation(int avg) {
  unsigned long sum = 0;
  for(int i = 0; i < measuresPerSec; i++) {
    int n = (soundArray[i]-avg);
    sum += n*n;
  }
  return sqrt(sum / measuresPerSec);
}





long sumOfAvgs = 0;
int avgIndex = 0;
int rollingAvgs [30];
float soundRollingAvg(int newVal) { 
  Serial.println(avgIndex);
  return updateRollingAvg(rollingAvgs,&sumOfAvgs,&avgIndex,newVal); }

long sumOfAvgsSqr = 0;
int avgSqrIndex = 0;
int rollingAvgsSqr [30];
float soundRollingAvgSqr(int newVal) { return updateRollingAvg(rollingAvgsSqr,&sumOfAvgsSqr,&avgSqrIndex,newVal); }

long sumOfPeakAvgs= 0;
int peakAvgIndex = 0;
int rollingPeakAvgs [30];
float soundRollingPeakAvg(int newVal) { return updateRollingAvg(rollingPeakAvgs,&sumOfPeakAvgs,&peakAvgIndex,newVal); }

long sumOfDevs = 0;
int devIndex = 0;
int rollingDevs [30];
float soundRollingDeviation(int newVal) { return updateRollingAvg(rollingDevs,&sumOfDevs,&devIndex,newVal); }

float updateRollingAvg(int arr[], long *total, int *updateIndex, int newVal) {
  *total -= arr[*updateIndex];
  *total += newVal;
  arr[*updateIndex] = newVal;

  *updateIndex = (*updateIndex + 1) % 30;
  return (float) *total / 30;
}


