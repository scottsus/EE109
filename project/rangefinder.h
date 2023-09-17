extern volatile unsigned char startedISR;
extern volatile unsigned char distanceChanged;
extern volatile unsigned char error;
extern volatile unsigned char ignoreEcho;

void checkAcquire(void);
int calcDistance(int);