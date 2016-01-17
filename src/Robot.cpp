#include "WPILib.h"
#include "Commands/Command.h"
#include "CommandBase.h"
#include "Commands/AutonomousLowBar.h"
#include "Commands/Portcullis.h"
#include "Commands/ChevalDeFrise.h"
#include "Commands/Ramparts.h"
#include "Commands/Moat.h"
#include "Commands/Drawbridge.h"
#include "Commands/SallyPort.h"
#include "Commands/RockWall.h"
#include "Commands/RoughTerrain.h"

//2016 Stronghold code
class Robot: public IterativeRobot
{
private:
	Command *autonomousCommand;
	LiveWindow *lw;
	SendableChooser *autoMode;


	void RobotInit()
	{
		CommandBase::init();
//		autonomousCommand = new ExampleCommand();
		lw = LiveWindow::GetInstance();
		autoMode = new SendableChooser();
		autoMode->AddDefault("Default-Low Bar", new AutonomousLowBar());
		autoMode->AddObject("Portcullis", new AutonomousPortcullis());
		autoMode->AddObject("Cheval de Frise", new AutonomousChevalDeFrise());
		autoMode->AddObject("Ramparts", new AutonomousRamparts());
		autoMode->AddObject("Moat", new AutonomousMoat());
		autoMode->AddObject("Drawbridge", new AutonomousDrawbridge());
		autoMode->AddObject("Sally Port", new AutonomousSallyPort());
		autoMode->AddObject("Rock Wall", new AutonomousRockWall());
		autoMode->AddObject("Rough Terrain", new AutonomousRoughTerrain());
//		autoMode->AddObject("SpyBot", new AutonomousSpyBot()); //Restore when command is written, plus need one for each position
		SmartDashboard::PutData("Autonomous Mode", autoMode);

	}
	
	void DisabledPeriodic()
	{
		Scheduler::GetInstance()->Run();
	}

	void AutonomousInit()
	{
		if (autonomousCommand != NULL)
			autonomousCommand->Start();
	}

	void AutonomousPeriodic()
	{
		Scheduler::GetInstance()->Run();
	}

	void TeleopInit()
	{
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to 
		// continue until interrupted by another command, remove
		// this line or comment it out.
		if (autonomousCommand != NULL)
			autonomousCommand->Cancel();
	}

	void TeleopPeriodic()
	{
		Scheduler::GetInstance()->Run();
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
