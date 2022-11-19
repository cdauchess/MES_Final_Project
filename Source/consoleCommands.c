// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.

#include <string.h>
#include <stdio.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "version.h"
#include "Adxl343.h"
#include "userFuncs.h"

extern uint8_t exitConsole;

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}

static eCommandResult_T ConsoleCommandComment(const char buffer[]);
static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);
static eCommandResult_T ConsoleCommandUsers(const char buffer[]);
static eCommandResult_T ConsoleCommandRelay(const char buffer[]);
static eCommandResult_T ConsoleCommandAccel(const char buffer[]);
static eCommandResult_T ConsoleCommandExit(const char buffer[]);

static const sConsoleCommandTable_T mConsoleCommandTable[] =
{
    {";", &ConsoleCommandComment, HELP("Comment! You do need a space after the semicolon. ")},
    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
	{"users", &ConsoleCommandUsers, HELP("Show current list of authorized users and admins")},
	{"relay", &ConsoleCommandRelay, HELP("Show the current status of the output relay")},
	{"accel", &ConsoleCommandAccel, HELP("Show the current values from the accelerometer")},
	{"exit", &ConsoleCommandExit, HELP("Exit the console and return to normal operation")},

	CONSOLE_COMMAND_TABLE_END // must be LAST
};

static eCommandResult_T ConsoleCommandComment(const char buffer[])
{
	// do nothing
	IGNORE_UNUSED_VARIABLE(buffer);
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandHelp(const char buffer[])
{
	uint32_t i;
	uint32_t tableLength;
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
	for ( i = 0u ; i < tableLength - 1u ; i++ )
	{
		ConsoleIoSendString(mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(" : ");
		ConsoleIoSendString(mConsoleCommandTable[i].help);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandVer(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	ConsoleIoSendString(VERSION_STRING);
	ConsoleIoSendString(STR_ENDLINE);
	return result;
}

static eCommandResult_T ConsoleCommandUsers(const char buffer[])
{
	IGNORE_UNUSED_VARIABLE(buffer);
	ConsoleIoSendString("Authorized Users:");
	ConsoleSendString(STR_ENDLINE);
	users authUsers;
	authUsers = readDatabase();
	char txBuffer[60];
	for(int i = 0; i< authUsers.numUsers; i++){
		sprintf(txBuffer, "User %d : 0x%x%x%x%x", i, authUsers.userList[i].uiduint8_t[0],authUsers.userList[i].uiduint8_t[1],authUsers.userList[i].uiduint8_t[2],authUsers.userList[i].uiduint8_t[3]);
		ConsoleSendString(txBuffer);
		ConsoleSendString(STR_ENDLINE);
	}
}

static eCommandResult_T ConsoleCommandRelay(const char buffer[])
{
	IGNORE_UNUSED_VARIABLE(buffer);
	ConsoleIoSendString("Relay status:");
	//Show relay status here
}

static eCommandResult_T ConsoleCommandAccel(const char buffer[])
{
	IGNORE_UNUSED_VARIABLE(buffer);
	accels accelRes;
	accelRes = accl_read();
	char txBuffer[45];

	sprintf(txBuffer, "Accel: X = %3dmg, Y = %3dmg, Z = %3dmg", accelRes.X, accelRes.Y, accelRes.Z);
	ConsoleIoSendString(txBuffer);
	ConsoleIoSendString(STR_ENDLINE);
}

static eCommandResult_T ConsoleCommandExit(const char buffer[])
{
	IGNORE_UNUSED_VARIABLE(buffer);
	exitConsole = 1;
	ConsoleIoSendString("Exiting console...");
	ConsoleIoSendString(STR_ENDLINE);
}

const sConsoleCommandTable_T* ConsoleCommandsGetTable(void)
{
	return (mConsoleCommandTable);
}


