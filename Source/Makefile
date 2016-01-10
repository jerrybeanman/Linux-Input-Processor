
# Makefile 
# Ruoqi Jia

CC=gcc	
NAME=Asn1		
FLAGS=-Wall
SFILES= main.c   utilities.c processes.c
OFILES=$(SFILES:.c=.o)


$(NAME): 	$(OFILES)
		$(CC) $(FLAGS) -o $(NAME) $(OFILES) 

clean:
		rm $(OFILES) $(NAME)

main.o:
		$(CC) -c main.c

utilities.o:
		$(CC) -c utilities.c

processes.o:
		$(CC) -c processes.c