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
-- 	process until the carriage return key is recieved, which then sends the entire line to the translate 
-- 	process. The translate process reads data from the input pipe and translate characters based on given requirements,
--	and then the modified data is sent to the ouput process. The output process reads data from pipe that is used by
--	the input and translate process and echo whatever that is recieved onto the screen.
--	
--	Inputs such as ^K and T will send abort and terminate signals to all three processes.
--------------------------------------------------------------------------------------------------------------------*/

#ifndef _PROCESSES_H
#define _PROCESSES_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "utilities.h"

#define MSG_SIZE		128		/* buffer size to write to pipe*/

#define CHAR_FROM		0x61	/* character 'a' */
#define CHAR_TO			0x7A	/* character 'z' */	
#define CARRIAGE_RETURN	0x45	/* character 'E' */
#define BACKSPACE		0x58	/* character 'X' */
#define NORM_TERM		0x54	/* character 'T' */
#define LINE_KILL		0x4B	/* character 'K' */
#define ABNORM_TERM		0x0B	/* character '^K' */

/* process IDs */
extern pid_t id_trans, id_out, id_in;

/* set to true when NORM_TERM is recieved in the translate process*/
extern int isterm;

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
void handle_input(int pipe_in_trans[2], int pipe_in_out[2]);

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
void handle_translate(int pipe_in_trans[2], int pipe_in_out[2]);


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
void handle_output(int pipe_in_out[2]);


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
void translate(char from[MSG_SIZE], char to[MSG_SIZE]);

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
void init_empty_buf(char buf[MSG_SIZE]);

#endif