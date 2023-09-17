extern volatile unsigned char isOn;
extern volatile unsigned char isDone;
extern volatile unsigned char isPlaying;
extern volatile unsigned int buzzerTime;

void checkBuzzer(void);
void activateBuzzer(int);