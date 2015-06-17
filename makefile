all: proyecto.c
	gcc -g -Wall -o proyecto proyecto.c && ./proyecto

clean:
	$(RM) proyecto