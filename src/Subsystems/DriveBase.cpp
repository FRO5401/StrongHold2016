/*
 * Drive Base subsystem for 2016 FRC Stronghold game
 * Team 5401 Fightin' Robotic Owls
 * FROGramming team
 * Subsystem's got two empty halves of coconut and is bangin' 'em together.
 */

#include "DriveBase.h"
#include "../RobotMap.h"
#include <Commands/XboxMove.h>

const double DPPLeft		= (-1/6.318); 		//TODO Might need to to tune left enc
const double DPPRight		= (1/6.318);
const float GyroScalar		= 10; 		//Preliminarily tuned
const float GyroLinearAdj	= -0.696; 	//Adjusts for Gyro Creep = m
const float GyroOffset		= -6.1395;	// = b
float initialGyro			= 0;

const double AutoDriveSpeed	= 0.5;
const double AutoTurnSpeed	= 0.5;
const double AngleThreshold	= 2; 		//Turn angle in degrees //TODO Must tune this
const double AutoDistThresh	= 2; 		//Distance threshold in inches //TODO Must tune this

const float kP = .835;	//Default kP scalar value

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
	SmartDashboard::PutNumber("Initial Gyro Value", initialGyro);

	MainGyro	= new ADXRS450_Gyro();
 	DS_ForDriveBase -> GetInstance();
 	TimeCount = new Timer();
 	TimeCount -> Reset();
 	MainGyro  -> Reset();
 //	LeftEnc -> Reset();Doesn't work when enabling and disabling
 //	RightEnc -> Reset();
 	
 	//kP = 0;			//Uncomment for getting value from dashboard
 	//SmartDashboard::PutNumber("kP Value", kP);

}

void DriveBase::InitDefaultCommand()
{
	SetDefaultCommand(new XboxMove());
	LeftEnc	  -> Reset();
	RightEnc  -> Reset();
	//MainGyro  -> Calibrate();
	MainGyro  -> Reset();
	TimeCount -> Start();
}

void DriveBase::Drive(double LeftDriveDesired, double RightDriveDesired)
  {

  //kP = SmartDashboard::GetNumber("kP Value", kP); //Uncomment for getting value from dashboard

  LeftDrive1 	-> Set(LeftDriveDesired); //passes desired state to speed controllers
  LeftDrive2	-> Set(LeftDriveDesired);
  RightDrive1 	-> Set(-1 * RightDriveDesired);
  RightDrive2	-> Set(-1 * RightDriveDesired);

  //New stuff
  //Sets the ratio for pulses to inches
  LeftEnc 	-> SetDistancePerPulse(DPPLeft);
  RightEnc 	-> SetDistancePerPulse(DPPRight);

  //Displays certain values in the encoder onto the SmartDashboard
  SmartDashboard::PutNumber("Left Encoder Raw Count Value", 	LeftEnc 	-> Get());
  SmartDashboard::PutNumber("Right Encoder Raw Count Value", 	RightEnc 	-> Get());
  SmartDashboard::PutNumber("Left Encoder Distance Traveled", 	LeftEnc 	-> GetDistance());
  SmartDashboard::PutNumber("Right Encoder Distance Traveled", 	RightEnc 	-> GetDistance());

  SmartDashboard::GetNumber("Initial Gyro Value", initialGyro);
  //SmartDashboard::PutNumber("Gyro Angle", ReportGyro());	//doesn't work for some reason
  SmartDashboard::PutNumber("Gyro GetAngle", MainGyro -> GetAngle());
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

  LeftDrive1	-> Set(0);
  LeftDrive2	-> Set(0);
  RightDrive1	-> Set(0);
  RightDrive2	-> Set(0);
  TimeCount 	-> Stop();

  }

  void DriveBase::Reset()
  {
//  	LeftEnc ->Reset();
//  	RightEnc ->Reset();
  	LeftDrive1	-> Set(0);
  	LeftDrive2	-> Set(0);
  	RightDrive1	-> Set(0);
  	RightDrive2	-> Set(0);

  	TimeCount -> Reset();
  	MainGyro  -> Reset();

  }
  //New stuff
  //A function to use the encoders in driving, the robot will drive in a certain direction depending on the distance left to travel
void DriveBase::AutoDriveDistance(float DesiredDistance){
//	SmartDashboard::GetNumber("Distance for Encoder Drive", DashAutoDistance); //Remove this and 2 lines below once we calibrate, this is just to do it from the dashboard for various distances without recompiling over and over
//	DesiredDistance = DashAutoDistance;

	EncoderReset();
	MainGyro -> Reset();

	float DistanceTraveled = 0;
	if (fabs(DesiredDistance) <= AutoDistThresh){
		std::cout << "DesiredDistance to small!!!\n";
	} else {
		while ((DesiredDistance > 0) ? (DistanceTraveled < fabs(DesiredDistance) - AutoDistThresh) : (DistanceTraveled > AutoDistThresh - fabs(DesiredDistance))){
			if (DesiredDistance > 0){ //DesiredDistance is positive, go forward
				Drive(AutoDriveSpeed * kP, AutoDriveSpeed);
			} else if (DesiredDistance < 0){ //DesiredDistance is negative, go backward
				Drive(-AutoDriveSpeed * kP, -AutoDriveSpeed);
			} else { //error or exactly 0
				std::cout << "AutoDriveDistance Error!!!\n";
				break;
			}
		DistanceTraveled = (RightEnc -> GetDistance());//XXX TODO re-add leftenc for competition robot
		}
	}
}

  void DriveBase::EncoderReset(){
	  LeftEnc -> Reset();
	  RightEnc -> Reset();

	  //Might not be needed
	  LeftEnc 	-> SetDistancePerPulse(DPPLeft);
	  RightEnc 	-> SetDistancePerPulse(DPPRight);
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
	  float FinalAngle = GyroAngle + DesiredTurnAngle;
	  while (fabs(FinalAngle - GyroAngle) > AngleThreshold)
	  {
		  if ((FinalAngle - GyroAngle) > AngleThreshold) {
			  Drive(AutoTurnSpeed, -AutoTurnSpeed);
		  } 	else if ((FinalAngle - GyroAngle) < AngleThreshold) {
			  	  Drive(-AutoTurnSpeed, AutoTurnSpeed);
				}
		GyroAngle = ReportGyro();
	}
	return (FinalAngle - GyroAngle); //return the final delta
}
float DriveBase::ReportGyro()
{
  	float Angle = (GyroScalar * MainGyro	->	GetAngle());
   	double Time = TimeCount -> Get();
   	float AdjAngle = Angle - (GyroLinearAdj * Time + GyroOffset);//Compensates for gyro creep - basically subtracts out mx+b the linear creep function
  	return Angle;
}
