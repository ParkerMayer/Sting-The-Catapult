// these are all the libraries we're using
#include <Arduino.h>
#include <Ultrasonic.h>
#include <PS2X_lib.h>

//
//	code by ochoa
//

void reload();
void myWittleMacwo();

// giving names to the pins
byte leftMotorBck = 2; // bytes store values 0 to 255--perfect!
byte leftMotorFwd = 3;
byte leftPWM = 4;
byte rightMotorFwd = 5;
byte rightMotorBck = 6;
byte rightPWM = 7;
byte catapult1 = 8;
byte catapult2 = 9;
byte catapultPWM = 10;
byte limitSwitch = 22;

PS2X controller;		// creating a PS2X object
const byte PS2_DAT = 51;// assigning pin numbers to variables
const byte PS2_CMD = 50;
const byte PS2_ATT = 53;
const byte PS2_CLK = 52;
int vibrate = 0;		// create a variable for vibration level
int motorPower = 128;
bool pressures = false;	// set pressures to false
bool rumble = false;	// set rumble to false

Ultrasonic ultrasonic(46, 48); // create an ultrasonic object

void setup() {

	delay(300); // lil pause for start up

	// configuring the controller!!!
	controller.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, pressures, rumble);

	// setting which pins are output/input
	pinMode(leftMotorBck, OUTPUT);
	pinMode(leftMotorFwd, OUTPUT);
	pinMode(leftPWM, OUTPUT);
	pinMode(rightMotorFwd, OUTPUT);
	pinMode(rightMotorBck, OUTPUT);
	pinMode(rightPWM, OUTPUT);
	pinMode(catapult1, OUTPUT);
	pinMode(catapult2, OUTPUT);
	pinMode(catapultPWM, OUTPUT);
	pinMode(limitSwitch, INPUT_PULLUP);

	reload();
}

void loop() {

	// check for joystick position & button pressings
	controller.read_gamepad(false, vibrate);

	//IF the left joystick is pushed forward
	if(controller.Analog(PSS_LY) >= 0 && controller.Analog(PSS_LY) <= 120){
		// GO FORWARD in coast mode
		motorPower = map(controller.Analog(PSS_LY), 0, 120, 255, 0);
		digitalWrite(leftPWM, HIGH);
		analogWrite(leftMotorFwd, motorPower);

	// OTHERWISE if the left joystick is pulled backwards
	}else if(controller.Analog(PSS_LY) >= 140 && controller.Analog(PSS_LY) <= 255){
		// GO BACKWARD in coast mode
		motorPower = map(controller.Analog(PSS_LY), 140, 255, 0, 255);
		digitalWrite(leftPWM, HIGH);
		analogWrite(leftMotorBck, motorPower);

	// when the left joystick is in the d e a d z o n e
	}else{
		// STOP MOVIN
		analogWrite(leftMotorFwd, 0);
		analogWrite(leftMotorBck, 0);
	}

	//IF the right joystick is pushed forward
	if(controller.Analog(PSS_RY) >= 0 && controller.Analog(PSS_RY) <= 120){
		// GO FORWARD in coast mode
		motorPower = map(controller.Analog(PSS_RY), 0, 120, 255, 0);
		digitalWrite(rightPWM, HIGH);
		analogWrite(rightMotorFwd, motorPower);

	// OTHERWISE if the right joystick is pulled backwards
	}else if(controller.Analog(PSS_RY) >= 140 && controller.Analog(PSS_RY) <= 255){
		// GO BACKWARD in coast mode
		motorPower = map(controller.Analog(PSS_RY), 140, 255, 0, 255);
		digitalWrite(rightPWM, HIGH);
		analogWrite(rightMotorBck, motorPower);

	// when the right joystick is in the d e a d z o n e
	}else{
		// STOP MOVIN
		analogWrite(rightMotorFwd, 0);
		analogWrite(rightMotorBck, 0);
	}

	// if the circle button is pressed
	if(controller.ButtonPressed(PSB_CIRCLE)){
		myWittleMacwo();
	}
	delay(10);
}

//function for reloading the catapult
void reload(){

	digitalWrite(catapult2, HIGH); // rotate gear clkwse
	digitalWrite(catapult1, LOW);
	analogWrite(catapultPWM, 255);

	while(true){ // constantly check

		if(digitalRead(limitSwitch) == LOW){ // if limit switch is pressed
			delay(50);
			analogWrite(catapultPWM, 0); // stop motor
			break;
		}
	}
}

//function that stores the macro!
void myWittleMacwo(){

	// drive forward
	digitalWrite(leftMotorFwd, HIGH);
	digitalWrite(rightMotorFwd, HIGH);
	digitalWrite(leftMotorBck, LOW);
	digitalWrite(rightMotorBck, LOW);
	analogWrite(leftPWM, 200);
	analogWrite(rightPWM, 200);

	while(true){
		if(ultrasonic.read() < 10){ // if barricade is 10cm away

			// stop the drivetrain
			analogWrite(leftPWM, 0);
			analogWrite(rightPWM, 0);

			// RELEASE THE BOULDER
			digitalWrite(catapult2, HIGH); // rotate gear clkwse
			digitalWrite(catapult1, LOW);
			analogWrite(catapultPWM, 255);
			delay(200);
			analogWrite(catapultPWM, 0);

			// call reload function
			reload();

			// bacc up
			digitalWrite(leftMotorFwd, LOW);
			digitalWrite(rightMotorFwd, LOW);
			digitalWrite(leftMotorBck, HIGH);
			digitalWrite(rightMotorBck, HIGH);
			analogWrite(leftPWM, 200);
			analogWrite(rightPWM, 200);
			delay(1500);
			analogWrite(leftPWM, 0);
			analogWrite(rightPWM, 0);

			break;
		}
	}
}
