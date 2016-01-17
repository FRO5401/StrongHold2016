#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include "Subsystems/DriveBase.h"
#include "Subsystems/Shooter.h"
#include <string>
#include "Commands/Command.h"
#include "OI.h"
#include "WPILib.h"


/**
 * The base for all commands. All atomic commands should subclass CommandBase.
 * CommandBase stores creates and stores each control system. To access a
 * subsystem elsewhere in your code in your code use CommandBase.examplesubsystem
 */

class CommandBase: public Command
{
public:
	CommandBase(char const *name);
	CommandBase();
	static void init();
	// Create a single static instance of all of your subsystems
//	static ExampleSubsystem *examplesubsystem;
	static OI *oi;

	//Creates statics. I don't really understand it, but statics are like the virtual robot parts
	//Statics are manipulated to affect the physical robot
	//A static is necessary for each subsystem
	static DriveBase *drivebase;
	static Shooter *shooter;
};

#endif