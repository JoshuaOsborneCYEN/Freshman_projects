#Makefile for Joshua Osborne's final Tech Shell for Assignment 1
#CSC 345 
#25 April 2017

techShell: TechShellFinal.c
	g++ -o techShell TechShellFinal.c

clean:
	rm techShell
