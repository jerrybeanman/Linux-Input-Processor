#include "utilities.h"
#include "processes.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	Asn1.c - An application that reads keyboard input, then processes and displays
							   the results.
-- 
-- PROGRAM:		Asn1
-- 
-- FUNCTIONS:	intt main(void)
--
-- 
-- DATE:		January 8, 2016
-- 
-- REVISIONS:
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- NOTES: This program demonstrates practical use of pipes, processes, and signals. The program creates three
-- different processes: input, output, and translate. The input process reads inputs recived from a terminal keyboard
-- and echoed to the screen by the output process. each line is modified and handled by the translate process, which will
-- also be echoed out by the output process. Each invidivual processes will communicate via pipes.
--
--------------------------------------------------------------------------------------------------------------------*/
int main(void) 
{
	/* Catch signals */
	signal(SIGABRT, handle_signal);
	signal(SIGTERM, handle_signal);

	/* pipes */
	int pipe_in_trans[2], pipe_in_out[2]; 

	/* create pipe for communication */
	create_pipe(pipe_in_trans);
	create_pipe(pipe_in_out);

	/* Get input pid */
	id_in = getpid();

	/* toogle off terminal proccesses */
	toogle_termproc(OFF);

	/* translate process */
	if(create_process(&id_trans) == 0)	
	{
		handle_translate(pipe_in_trans, pipe_in_out);
	}else
	/* output process */
	if(create_process(&id_out) == 0)
	{
		handle_output(pipe_in_out);
	}else
	/* parent(input) process */
	{
		handle_input(pipe_in_trans, pipe_in_out);
	}
   	return 0;
}









