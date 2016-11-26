
#include "mbed.h"

#define AUTO_MODE 0
#define MANUAL_MODE 1
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define STOP 4

Timeout manualTimeout;

PwmOut dirPinA1(P2_0);
PwmOut dirPinA2(P2_5);

PwmOut dirPinB1(P2_2); //A2
PwmOut dirPinB2(P2_3); //Y2


InterruptIn niceButton(P2_10);
InterruptIn niceButtonLeft(P2_11);
InterruptIn niceButtonRight(P2_12);
DigitalOut testButtonLED(P2_13);
Serial device(P0_15, P0_16);

AnalogIn soundDetector(P1_31);


AnalogIn pod(P0_25);

float getPodVal(){
	return pod.read();
}

//PwmOut buzzer(P2_3);
PwmOut fan1(P2_4);

float speed = 1.0f;
int state = 0;
int currentDirection = 0;
int buttonState = 0;
int isButtonRise = 0;
int control_mode = MANUAL_MODE;

//const int buffer_size = 255;
//char tx_buffer[buffer_size+1];
//char rx_buffer[buffer_size+1];
//
//volatile int tx_in=0;
//volatile int tx_out=0;
//volatile int rx_in=0;
//volatile int rx_out=0;
//
//char tx_line[80];
//char rx_line[80];

float FAN_SPEED = 1.0f;
float buzVal = 0.0f;
bool voring = false; //à¸«à¸§à¸­ ing
bool ascending = true;
char ch;

Timer t;


void timerInterruptHandler(){
		control_mode = AUTO_MODE;
		t.reset();
}

void enFan(bool isEnable){
	if (isEnable)
		if (mode == 0)
			fan1.write(FAN_SPEED);
	else
		fan1.write(0.0f);
}

void setFanSpeed(float spd){
	FAN_SPEED = spd;
}

void forward(){
	speed = 1.0f;

	dirPinA1.write(speed);
	dirPinA2.write(0.0f);

	dirPinB1.write(speed);
	dirPinB2.write(0.0f);
	currentDirection = FORWARD;
}

void backward(){
	speed = 1.0f;
	dirPinA1.write(0.0f);
	dirPinA2.write(speed);

	dirPinB1.write(0.0f);
	dirPinB2.write(speed);
	currentDirection = BACKWARD;
}

void right(){
	speed = 0.5f;
	dirPinA1.write(speed);
	dirPinA2.write(0.0f);

	dirPinB1.write(0.0f);
	dirPinB2.write(speed);
	currentDirection = RIGHT;
}

void left(){
	speed = 0.5f;
	dirPinA1.write(0.0f);
	dirPinA2.write(speed);

	dirPinB1.write(speed);
	dirPinB2.write(0.0f);
	currentDirection = LEFT;
}

void stop(){
	speed = 1.0f;
	dirPinA1.write(speed);
	dirPinA2.write(speed);
	//
	dirPinB1.write(speed);
	dirPinB2.write(speed);
	currentDirection = STOP;
}


void doCommand(char command){
	if (command == 'f'){
		forward();
	}
	else if (command == 'b'){
		backward();
	}
	else if (command == 'r'){
		left();
	}
	else if (command == 'l'){
		right();
	}
	else if (command == 's'){
		stop();
	}
}

void Rx_interrupt() {
	char a;
	control_mode = MANUAL_MODE;
	t.reset();
    while(device.readable()){
		a = device.getc();
        if(a == 'f' || a == 'l' || a == 'r' || a == 'b' || a == 's'){
        	doCommand(a);
        }
    }
    return;
}



void checkCorner(){
	if(currentDirection == FORWARD){
		backward();
		wait(1.0);
		left();
		wait(1.0);
		forward();
	}
}

void niceButtonClickHandler(){
	if (isButtonRise){
		isButtonRise = 0;
		return;
	}
	if (currentDirection == FORWARD){
		checkCorner();
	}
	else if (currentDirection == BACKWARD){
		forward();
	}
	isButtonRise = 1;
	return;
}


void doAutoThings(){
	if (currentDirection == BACKWARD){
		backward();
	}
	else{
		forward();
	}
}

float checkSound(){
	float val = soundDetector.read();
	if (val * 100.0 > 50){
		int a = 5;
	}
	return val;
}

int main(){
	t.start();
	//led = 0;
	device.baud(9600);
	device.attach(&Rx_interrupt, Serial::RxIrq);
	speed = 1.0f;
	forward();
	//manualTimeout.attach(&timerInterruptHandler, 3.0f);
	niceButton.fall(&niceButtonClickHandler);
	while(1){
		enFan(true, 0);
		setFanSpeed(getPodVal());
		checkSound();
		if (control_mode == MANUAL_MODE){
			int ms = t.read_ms();
			if(t.read_ms() > 3000){
				timerOutHandler();
			}
		}
		if (control_mode == AUTO_MODE){
			doAutoThings();

//			if (buttonState != niceButton.read()){
//				niceButtonClickHandler();
//				buttonState = !buttonState;
//			}
		}
	}
}

