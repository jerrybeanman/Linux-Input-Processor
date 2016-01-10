
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

#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "processes.h"

/* For toogling terminal proccesses */
#define ON 	100
#define OFF 101


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
void toogle_termproc(int allow);

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
void create_pipe(int pipe[2]);

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
void error(char * msg);

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
pid_t create_process(pid_t *pid);

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
void handle_signal(int sig);


#endif 