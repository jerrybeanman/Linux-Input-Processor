
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	utilities.c - Provides some wrapper and delegate functions
-- 
-- PROGRAM:		Asn1
-- 
-- FUNCTIONS:	void toogle_termproc(int allow);
--				void create_pipe(int pipe[2]);
-- 				void error(char * msg);
--				pid_t create_process(pid_t *pid);
--				void handle_signal(int sig);
--
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- NOTES: Provide functions to delegate duplicate codes, such as error handling, creating pipes and processes
--
--------------------------------------------------------------------------------------------------------------------*/

#include "utilities.h"

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	toogle_termproc
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void toogle_termproc(int allow);
--					int allow: turns on/off the system terminal processes 
-- 
-- RETURNS: void
-- 
-- NOTES: Toogle on and off of termianl proccesses. Has to take the defined macro ON and OFF 
--
--------------------------------------------------------------------------------------------------------------------*/
void toogle_termproc(int allow)
{
	if(allow == OFF)
	{
		system("stty raw igncr -echo");
	}
	else
	{
		system("stty -raw -igncr echo");
	}
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	create_pipe
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void create_pipe(int pipe[2]);
--					int pipe[2]: the pipe to create for inter-proccess communications
-- 
-- RETURNS: void
-- 
-- NOTES: Creates an pipe for inter-process communications 
--
--------------------------------------------------------------------------------------------------------------------*/
void create_pipe(int p[2])
{
	if(pipe(p) < 0)
	{
		error("pipe");
	}
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	error
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void error(char * msg);
--					char * msg: The error message to be printed to stderr
-- 
-- RETURNS: void
-- 
-- NOTES:  handle API errors, will exit program after msg is print
--------------------------------------------------------------------------------------------------------------------*/
void error(char * msg)
{
	perror(msg);
	kill(getpid(), SIGTERM);
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	create_process
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	pid_t create_process(pid_t *pid);
--					pid_t * pid: id that the proccess will assign to after creation
-- 
-- RETURNS: The process id
-- 
-- NOTES:  create a proccess, error check it and return the process id
--------------------------------------------------------------------------------------------------------------------*/
pid_t create_process(pid_t *pid)
{
	if((*pid = fork()) == -1)
	{
		error("fork");
	}
	return *pid;
}


/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	handle_signal
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void handle_signal(int sig);
--					int sig: The signal to be caught
-- 
-- RETURNS: The process id
-- 
-- NOTES:   catch SIGTERM and SIGABRT. Send Kill() to all three proccesses running 
--------------------------------------------------------------------------------------------------------------------*/
void handle_signal(int sig)
{
		signal(sig, NULL);

		/* kill all three processes */
		kill(id_in, sig);
		kill(id_out, sig);
		kill(id_trans, sig);

		toogle_termproc(ON);
}

