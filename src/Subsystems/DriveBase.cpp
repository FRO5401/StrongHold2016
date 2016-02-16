/*
 * Drive Base subsystem for 2016 FRC Stronghold game
 * Team 5401 Fightin' Robotic Owls
 * FROGramming team
 * Subsystem's got two empty halves of coconut and is bangin' 'em together.
 */

#include "DriveBase.h"
#include "../RobotMap.h"
#include <Commands/XboxMove.h>

const double DPP			= 1; 		//TODO Must tune this
const float GyroScalar		= 10; 		//Preliminarily tuned
const float GyroLinearAdj	= -0.696; 	//Adjusts for Gyro Creep = m
const float GyroOffset		= -6.1395;	// = b

const double AutoDriveSpeed	= 0.5;
const double AutoTurnSpeed	= 0.5;
const double AngleThreshold	= 2; 		//Turn angle in degrees //TODO Must tune this
const double AutoDistThresh	= 2; 		//Distance threshold in inches //TODO Must tune this

DriveBase::DriveBase() :
		Subsystem("DriveBase")
{
	LeftDrive1 	= new Victor(LeftMotor1);
	LeftDrive2	= new Victor(LeftMotor2);
	RightDrive1	= new Victor(RightMotor1);
	RightDrive2	= new Victor(RightMotor2);
//	LeftShift 	= new DoubleSolenoid(SolenoidCAN_ID, Shift_LeftFwd, Shift_LeftRev); //Gear shifting is OUT of design
//	RightShift 	= new DoubleSolenoid(SolenoidCAN_ID, Shift_RightFwd, Shift_RightRev);

	//Sensors
	LeftEnc		= new Encoder(Enc_Left_A,Enc_Left_B, true, Encoder::k1X);
	RightEnc	= new Encoder(Enc_Right_A,Enc_Right_B,true,Encoder::k1X);
	//Displays the Distance for Encoder Drive for user input
	double DashAutoDistance = 0;//Remove or comment this and below it, this is for calibrating the auto drive
	SmartDashboard::PutNumber("Distance for Encoder Drive", DashAutoDistance);

	MainGyro	= new ADXRS450_Gyro();
 	DS_ForDriveBase -> GetInstance();
 	
}

void DriveBase::InitDefaultCommand()
{
	SetDefaultCommand(new XboxMove());
	LeftEnc		-> Reset();
	RightEnc 	-> Reset();
	MainGyro        -> Calibrate();
}

void DriveBase::Drive(double LeftDriveDesired, double RightDriveDesired)
  {
  LeftDrive1 	-> Set(LeftDriveDesired); //passes desired state to speed controllers
  LeftDrive2	-> Set(LeftDriveDesired);
  RightDrive1 	-> Set(RightDriveDesired);
  RightDrive2	-> Set(RightDriveDesired);

  //New stuff
  //Sets the ratio for pulses to inches
  LeftEnc 	-> SetDistancePerPulse(DPP);
  RightEnc 	-> SetDistancePerPulse(DPP);

  //Displays certain values in the encoder onto the SmartDashboard
  SmartDashboard::PutNumber("Left Encoder Raw Count Value", 	LeftEnc 	-> Get());
  SmartDashboard::PutNumber("Right Encoder Raw Count Value", 	RightEnc 	-> Get());
  SmartDashboard::PutNumber("Left Encoder Distance Traveled", 	LeftEnc 	-> GetDistance());
  SmartDashboard::PutNumber("Right Encoder Distance Traveled", 	RightEnc 	-> GetDistance());
  }
/*
 * Pneumatic shfting is out of design at this point
void DriveBase::ShiftLow()
{
	LeftShift  ->	Set(DoubleSolenoid::Value::kForward);
	RightShift ->	Set(DoubleSolenoid::Value::kForward);
}

void DriveBase::ShiftHigh()
{
	LeftShift  ->	Set(DoubleSolenoid::Value::kReverse);
	RightShift ->	Set(DoubleSolenoid::Value::kReverse);
}
*/
 void DriveBase::Stop()
  {

  LeftDrive1		-> Set(0);
  LeftDrive2	-> Set(0);
  RightDrive1	-> Set(0);
  RightDrive2	-> Set(0);

  }

  void DriveBase::Reset()
  {
//  	LeftEnc ->Reset();
//  	RightEnc ->Reset();
  	LeftDrive1	-> Set(0);
  	LeftDrive2	-> Set(0);
  	RightDrive1	-> Set(0);
  	RightDrive2	-> Set(0);

  }
  //New stuff
  //A function to use the encoders in driving, the robot will drive in a certain direction depending on the distance left to travel
void DriveBase::AutoDriveDistance(float DesiredDistance){
	SmartDashboard::GetNumber("Distance for Encoder Drive", DashAutoDistance); //TODO Remove this and 2 lines below once we calibrate, this is just to do it from the dashboard for various distances without recompiling over and over
	DesiredDistance = DashAutoDistance;

	EncoderReset();
	float Distance = ((LeftEnc -> GetDistance() + RightEnc -> GetDistance())/2); //Average the two sensor inputs
	while (fabs(DesiredDistance - Distance) > AutoDistThresh){
		if (fabs(DesiredDistance - Distance) > AutoDistThresh){
			Drive(AutoDriveSpeed, AutoDriveSpeed);
		} else if (fabs(DesiredDistance - Distance) < -AutoDistThresh){
			Drive(-1 * AutoDriveSpeed, -1 * AutoDriveSpeed);
		 	}
		Distance = ((LeftEnc -> GetDistance() + RightEnc -> GetDistance())/2);
	}
}

  void DriveBase::EncoderReset(){
	  LeftEnc -> Reset();
	  RightEnc -> Reset();
  }

float DriveBase::AutoTurnToAngle(float DesiredAngle)//Turns to an absolute angle based on encoder calibration
{
	float RawErr = (DesiredAngle - ReportGyro());
	float AbsErr = fabs(RawErr);
	if (AbsErr > 180) {
		AbsErr = 360 - AbsErr; //This determines the shortest distance between the angles
	}
if ((RawErr >= 0 && RawErr <=180) || (RawErr >= -360 && RawErr <= -180)) {//Determines sign of angle difference
	return AutoTurnAngle(AbsErr); //AutoTurnAngle returns the final angle difference, then this returns that return
	} else {
		return AutoTurnAngle(-AbsErr);
		}
}

float DriveBase::AutoTurnAngle(float DesiredTurnAngle)	//Turns a number of degrees relative to current position
{
	  float GyroAngle = ReportGyro();
	  while (fabs(DesiredTurnAngle - GyroAngle) > AngleThreshold)
	  {
		  if ((DesiredTurnAngle - GyroAngle) > AngleThreshold) {
			  Drive(AutoTurnSpeed, -AutoTurnSpeed);
		  } 	else if ((DesiredTurnAngle - GyroAngle) < AngleThreshold) {
			  	  Drive(-AutoTurnSpeed, AutoTurnSpeed);
				}
		GyroAngle = ReportGyro();
	}
	return (DesiredTurnAngle - GyroAngle); //return the final delta
}
float DriveBase::ReportGyro()
{
  	float Angle = (GyroScalar * MainGyro	->	GetAngle());
   	SmartDashboard::PutNumber("Gyro Angle", Angle);
   	float AdjAngle = Angle - (GyroLinearAdj * Timer() + GyroOffset);//Compensates for gyro creep - basically subtracts out mx+b the linear creep function
   	//TODO Fix the timer in here
  	return Angle;
}
