/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	proccesses.c - Provides the functionalities for all three processes 
-- 
-- PROGRAM:		Asn1
-- 
-- FUNCTIONS:	void handle_input(int pipe_in_trans[2], int pipe_in_out[2]);
--				void handle_output(int pipe_in_out[2]);
-- 				void handle_translate(int pipe_in_trans[2], int pipe_in_out[2]);
--				void translate(char buf[MSG_SIZE], char to[MSG_SIZE]);
--				void init_empty_buf(char buf[MSG_SIZE]);
--
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- NOTES: The core functionality of the program. This file include the definition for the three processes: input, 
--	output, and translate. As well as their respective behavior when communicating with one another through the use
--  of pipes. The input proccess takes in user inputs from stdin and sends each respective characters to the output 
-- 	process until the carriage return key is recieved, which then sends the data stream recorded to the translate 
-- 	process. The translate process reads data from the input pipe and translate characters based on given requirements,
--	and then the modified data is sent to the ouput process. The output process reads data from pipe that is used by
--	the input and translate process and echo whatever that is recieved onto the screen.
--	
--	Inputs such as ^K and T will send abort and terminate signals to all three processes.
--------------------------------------------------------------------------------------------------------------------*/
#include "processes.h"

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	handle_input
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void handle_input(int pipe_in_trans[2], int pipe_in_out[2]);
--					int pipe_in_trans[2]: 	pipe that will be used to write data to translate process
--					int pipe_in_out[2]: 	pipe that will be used to write data to output process
-- 
-- RETURNS: void
-- 
-- NOTES: The parent process. Reads keyboard inputs and writes each respective characters to the output process for it
-- to be echoed onto the screen until the carriage return(E) is recived, which then writes the stream of data recorded
-- to the translate process. 
-- When the abnormal terminate key(^k) is caught, it will send the abort signal to all processes running within the 
-- program.
--------------------------------------------------------------------------------------------------------------------*/
pid_t id_trans, id_out, id_in;
int isterm = 0;

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	handle_input
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void handle_input(int pipe_in_trans[2], int pipe_in_out[2]);
--					int pipe_in_trans[2]: 	pipe that will be used to write data to translate process
--					int pipe_in_out[2]: 	pipe that will be used to write data to output process
-- 
-- RETURNS: void
-- 
-- NOTES: The parent process. Reads keyboard inputs and writes each respective characters to the output process for it
-- to be echoed onto the screen until the carriage return(E) is recived, which then writes the stream of data recorded
-- to the translate process. 
-- When the abnormal terminate key(^k) is caught, it will send the abort signal to all processes running within the 
-- program.
--------------------------------------------------------------------------------------------------------------------*/
void handle_input(int pipe_in_trans[2], int pipe_in_out[2])
{
	char c, msg[MSG_SIZE];
	size_t index = 0;
	init_empty_buf(msg);

	/* close translator and output read descriptor */
	close(pipe_in_trans[0]);
	close(pipe_in_out[0]);

	while((c = getchar()))				/* Check for keyboard inputs */
	{
		/* write to ouput pipe */
		if(write(pipe_in_out[1], &c, 1) < 0)
			error("input write()");

		if(c == ABNORM_TERM)			/* '^K' detected */
		{
			kill(getpid(), SIGABRT);
		}else
		if(c == CARRIAGE_RETURN)		/* 'E' detected */
		{
			/* write to translator pipe */
			if (write(pipe_in_trans[1], msg, MSG_SIZE) < 0)
				error("input write()");

			init_empty_buf(msg);
			index = 0;
		}else
			msg[index++] = c;
	}
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	handle_translate
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void handle_translate(int pipe_in_trans[2], int pipe_in_out[2]);
--					int pipe_in_trans[2]:	pipe that will be used to read data sent from the input process
--					int pipe_in_out[2]: 	pipe that will be used to write modified data to the output process
-- 
-- RETURNS: void
-- 
-- NOTES: Child process of input. Reads data sent from the input process and translate characters based on given
-- constraints. For example, 'a' will be converted to 'z', 'X' will be read as backspace, 'K' will discard all 
-- preceeding characters. After the translation, the data is then sent to the output process via its pipe.
--------------------------------------------------------------------------------------------------------------------*/
void handle_translate(int pipe_in_trans[2], int pipe_in_out[2])
{
	/* close output read descriptor */
	close(pipe_in_out[0]);
	/* close translate write descriptor */
	close(pipe_in_trans[1]);

	while(1)
	{

		/* buffer for incoming and outgoing messages from input to output pipe */
		char read_msg[MSG_SIZE], write_msg[MSG_SIZE];

		init_empty_buf(read_msg);
		init_empty_buf(write_msg);

		/* read from translate pipe */
		if(read(pipe_in_trans[0], read_msg, MSG_SIZE) < 0)
			error("translate read()");

		/* replace 'a' with 'z', handles backspace, kill line, and normal terminate*/
		translate(read_msg, write_msg);

		/* write a new line to output pipe */
		if (write(pipe_in_out[1], "\r\n", 2) < 0)
			error("input write()");

		/* write translated message to output pipe */
		if (write(pipe_in_out[1], write_msg, MSG_SIZE) < 0)
			error("translate write()");

		/* write a new line to output pipe*/
		if (write(pipe_in_out[1], "\r\n", 2) < 0)
			error("input write()");

		/* check if NORM_TERM is recieved */
		if(isterm)
			kill(getpid(), SIGTERM);
	}
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	handle_output
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void handle_output(int pipe_in_out[2]);
--					int pipe_in_out[2]: 	pipe that will be used to read data from the input and translate process
-- 
-- RETURNS: void
-- 
-- NOTES: Child process of input. Reads data from a pipe that is used by the input and translate process and echos
-- the data onto the screen.
--------------------------------------------------------------------------------------------------------------------*/
void handle_output(int pipe_in_out[2])
{
	/* close output write descriptor */
	close(pipe_in_out[1]);
	while(1)
	{
		char msg[MSG_SIZE];
		init_empty_buf(msg);

		/* read from output pipe */
		if(read(pipe_in_out[0], msg, MSG_SIZE) == 0)
				error("output read()");

		/* write to standard output */
		if (write(STDOUT_FILENO, msg, MSG_SIZE) < 0)
				error("output write()");
	}
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	translate
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void translate(char from[MSG_SIZE], char to[MSG_SIZE]);
--					from[MSG_SIZE]: buffer data that is recieved from the input process
--					to[MSG_SIZE]: buffer data that will be write to the ouput process after modifications
-- 
-- RETURNS: void
-- 
-- NOTES: Takes in an populated buffer and attempts to translate its content into an empty buffer.
-- Character 'a' will be treated as 'z', 'X' will delete the previous character, 'K' will delete all preceeding 
-- characters.
-- isterm will be set to true when the normal terminate(T) key is detected
--------------------------------------------------------------------------------------------------------------------*/
void translate(char from[MSG_SIZE], char to[MSG_SIZE])
{
	for(int i = 0, j = 0; from[i] != '\0'; i++, j++)
	{
		switch (from[i])
		{
			case CHAR_FROM: 	/* Replace a with z */
				to[j] = CHAR_TO;
				break;
			case BACKSPACE:		/* "Delete" previous character */
				/* make sure X is not the first character */
				if(i != 0)
				{
					to[j-1] = '\0';
					j = j - 2;
				}
				break;
			case LINE_KILL:		/* "Delete" all previous characters*/
				j = 0;
				init_empty_buf(to);
				break;
			case NORM_TERM:		/* 'T' detected */
				isterm = 1;
				return;
				break;
			default:			/* Copy char to char */
				to[j] = from[i];
				break;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------ 
-- FUNCTION:	init_empty_buf
-- 
-- DATE:		January 7, 2015
-- 
-- REVISIONS:	
-- 
-- DESIGNER:	Ruoqi Jia
-- 
-- PROGRAMMER:	Ruoqi Jia
-- 
-- INTERFACE:	void init_empty_buf(char buf[MSG_SIZE]);
--					char buf[MSG_SIZE]: buffer that will be NULL terminated for every index
-- 
-- RETURNS: void
-- 
-- NOTES: Initialize buffer to all NULL characters 
--------------------------------------------------------------------------------------------------------------------*/
void init_empty_buf(char buf[MSG_SIZE])
{
	for(size_t i = 0; i < MSG_SIZE; i++)
		buf[i] = '\0';
}