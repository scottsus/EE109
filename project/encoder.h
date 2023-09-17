extern volatile unsigned char new_state;
extern volatile unsigned char old_state;
extern volatile unsigned char encoderChanged; // Flag for state change
extern volatile unsigned char a, b, regC;

extern volatile unsigned int nearThreshold;
extern volatile unsigned int farThreshold;
extern volatile unsigned char isFar;

void encoder_init(void);
void threshold_init(void);
void increment(unsigned char);
void decrement(unsigned char);