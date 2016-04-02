#include "AutoHookScimitarOnBumper.h"
#include "Commands/HookShoulderMoveToPosition.h"
#include "Commands/ScimitarMoveToPosition.h"

AutoHookScimitarOnBumper::AutoHookScimitarOnBumper()
{
	//Not needed as scimitar will start at required position AddSequential(new ScimitarMoveToPosition());
	std::cout << "Start AutoHookScimitarOnBumper\n";
	AddSequential(new HookShoulderMoveToPosition(10));//Go down to bumper //TODO Determine correct value
	AddSequential(new ScimitarMoveToPosition(-4)); //Probably hits limit switches first //Retract in to bumper //-2.875 is the high value
	std::cout << "End AutoHookScimitarOnBumper\n";
	// Add Commands here:
	// e.g. AddSequential(new Command1());
	//      AddSequential(new Command2());
	// these will run in order.

	// To run multiple commands at the same time,
	// use AddParallel()
	// e.g. AddParallel(new Command1());
	//      AddSequential(new Command2());
	// Command1 and Command2 will run in parallel.

	// A command group will require all of the subsystems that each member
	// would require.
	// e.g. if Command1 requires chassis, and Command2 requires arm,
	// a CommandGroup containing them would require both the chassis and the
	// arm.
}
