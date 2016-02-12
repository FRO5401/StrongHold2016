/*
 * Drive command for 2016 FRC Stronghold game
 * Team 5401 Fightin' Robotic Owls
 * FROGramming team
 * Vi = Ao * (sqrt(5 * (((Qc/Po)+1)^(2/7)-1)))
 */

#include "Commands/XboxMove.h"
#include "RobotMap.h"
//#include "SmartDashboard/SmartDashboard.h"

XboxMove::XboxMove()
{
	//Declares required subsystems
	Requires(drivebase);

}

// Called just before this Command runs the first time
void XboxMove::Initialize()
{
	drivebase	-> Reset();
//	drivebase->Stop();
}

// Called repeatedly when this Command is scheduled to run
void XboxMove::Execute()
{
	double 	Slew       =	oi	->	ReadXboxLeftStickX();
	double 	Throttle 	=	oi	->	GetRightTrigger();
	double 	Reverse 	=	oi	->	GetLeftTrigger();
	bool 	Precision	=	oi	->	GetPrecision();
	bool 	Brake		=	oi	->	GetBrake();
	double Right,Left, Sensitivity;

	if (Precision) { //Sets drive precision based on RobotMap and Precision Mode
		Sensitivity	=	Drive_Sensitivity_Precise;
	} else {
		Sensitivity	=	Drive_Sensitivity_Default;
	}

	if (Brake) {
		Right = 0;
		Left = 0;
	} else if (Slew > 0){									//Positive X axis means right turn
	Left = (Throttle - Reverse) * (1 - Slew) * Sensitivity;
	Right = (Throttle-Reverse) * (1) * Sensitivity;
	} else {
		Left = (Throttle-Reverse) * (1) * Sensitivity;
		Right = (Throttle-Reverse) * (1 + Slew) * Sensitivity;
	}



	drivebase        -> Drive(Left, Right, Encoder_Conversion_To_Inches_Constant);
}

// Make this return true when this Command no longer needs to run execute()
bool XboxMove::IsFinished()
{
	//New stuff
	if(oi -> GetButtonForEncoderDrive())
	{
			return true;
	}else{
		return false;
	}
}

// Called once after isFinished returns true
void XboxMove::End()
{

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void XboxMove::Interrupted()
{

}