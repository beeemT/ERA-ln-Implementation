#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

extern float asm_ext_ln();
float addressInput, addressResult;

float calcasm(float input);
void calculatelnasm(float* resultloglist, float* loglist, int length, float* timelist);
void calculatelnc(float* resultloglist, float* loglist, int length, float* timelist);
void retloglist(float* loglist, char const *argv[], int argc);
void printTable(float* loglist, float* resultloglistc, float* resultloglistasm, float* timelistc, float* timelistasm, int length);
float ln(float p);

int main(int argc, char const *argv[])
{
								if(argc < 2)
								{
																printf("Error: not enough arguments");
																return -1;
								}
								//format of commant: ./log numberinputs input input ...
								int i = 0;
								sscanf(argv[1], "%i", &i);
								if(i == 0) //number of inputs
								{
																//sampleprogram
																//generate 5 randomfloats
																//write randomfloats in loglist
																//calculate results and times
																//print tables
																//if i has value other than 0 then same program but no randomfloats
																i = 5;
																float randomone = (float) rand();
																float randomtwo = (float) rand();
																float randomthree = (float) rand();
																float randomfour = (float) rand();
																float randomfive = (float) rand();
																float loglist[i];
																loglist[0] = randomone;
																loglist[1] = randomtwo;
																loglist[2] = randomthree;
																loglist[3] = randomfour;
																loglist[4] = randomfive;
																float resultloglistc[i];
																float resultloglistasm[i];
																float timelistc[i];
																float timelistasm[i];
																calculatelnc(resultloglistc, loglist, i, timelistc);
																calculatelnasm(resultloglistasm, loglist, i, timelistasm);
																printTable(loglist, resultloglistc, resultloglistasm, timelistc, timelistasm, i);

																return 0;
								}
								if(i < 0)
								{
																printf("%s\n", "wrong number of inputs");
																return -1;
								}
								float loglist[i];
								float resultloglistc[i];
								float resultloglistasm[i];
								float timelistc[i];
								float timelistasm[i];
								retloglist(loglist, argv, argc);
								calculatelnc(resultloglistc, loglist, i, timelistc);
								calculatelnasm(resultloglistasm, loglist, i, timelistasm);
								printTable(loglist, resultloglistc, resultloglistasm, timelistc, timelistasm, i);
								return 0;
}

void calculatelnasm(float* resultloglist, float* loglist, int length, float* timelist)
{
								float* pr = resultloglist; //destination of results
								float* p = loglist; //source of inputs
								float* time = timelist; //table for calculationtimes
								clock_t start, ende;
								for(int i = 0; i < length; i++) //length is length of tables
								{
																start = clock();
																*pr = calcasm(*p); //call asm function for ln
																ende = clock();
																*time = (float)(ende-start); //calculate computationtimes
																pr++;
																p++;
																time++;
								}
								//same for calculatelnc
}

float calcasm(float input)
{
								addressInput = input;
								asm_ext_ln(); //call asm function for ln
								return addressResult;
}

void calculatelnc(float* resultloglist, float* loglist, int length, float* timelist)
{
								float* pr = resultloglist;
								float* p = loglist;
								float* time = timelist;
								clock_t start, ende;
								for(int i = 0; i < length; i++)
								{
																start = clock();
																*pr = log(*p); //call c function for ln
																ende = clock();
																*time = (float)(ende-start);
																pr++;
																p++;
																time++;

								}
}

void retloglist(float* loglist, char const *argv[], int argc)
{
								float* p = loglist;
								for(int x = 0; x < argc-2; x++)
								{
																*p = strtof(argv[x+2], NULL);
																p++;
								}
								//parse input into inputtable
}

void printTable(float* loglist, float* resultloglistc, float* resultloglistasm, float* timelistc, float* timelistasm, int length)
{
								float* ploglist = loglist;
								float* presultloglistc = resultloglistc;
								float* presultloglistasm = resultloglistasm;
								float* ptimelistc = timelistc;
								float* ptimelistasm = timelistasm;
								char sepline[] = "------------";
								char lineone[] =      "Input      | ";
								char linetwo[] =      "C-Result   | ";
								char linethree[] =    "ASM-Result | ";
								char linefour[] =     "C-Time     | ";
								char linefive[] =     "ASM-Time   | ";
								float pointone = 0;
								float pointtwo = 0;
								float pointthree = 0;
								float pointfour = 0;
								float pointfive = 0;
								for(int i = 0; i < length; i++)
								{
																pointone = *ploglist;
																pointtwo = *presultloglistc;
																pointthree = *presultloglistasm;
																pointfour = *ptimelistc;
																pointfive = *ptimelistasm;
																printf("%s\n", sepline);
																printf("%s", lineone);
																printf("%7f\n", pointone);
																printf("%s", linetwo);
																printf("%7f\n", pointtwo);
																printf("%s", linethree);
																printf("%7f\n", pointthree);
																printf("%s", linefour);
																printf("%7f\n", pointfour);
																printf("%s", linefive);
																printf("%7f\n", pointfive);
																printf("%s\n", sepline);
																ploglist++;
																presultloglistc++;
																presultloglistasm++;
																ptimelistc++;
																ptimelistasm++;
								}
}
