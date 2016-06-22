#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

float* asm_ext_ln(float* a);

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
								int i = (int)argv[2][0];
								if(i == 0)
								{
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
								float* pr = resultloglist;
								float* p = loglist;
								float* time = timelist;
								clock_t start, ende;
								for(int i = 0; i < length; i++)
								{
																start = clock();
																*pr = *asm_ext_ln(p);
																ende = clock();
																*time = (float)(ende-start);
																pr++;
																p++;
																time++;
								}
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
																*pr = log(*p);
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
								for(int x = 0; x < argc-3; x++)
								{
																*p = strtof(argv[x+3], NULL);
																p++;
								}
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
								for(int i = 0; i < length; i++)
								{
																printf("%s\n", sepline);
																printf(lineone, "%7f\n", *ploglist);
																printf(linetwo, "%7f\n", *presultloglistc);
																printf(linethree, "%7f\n", *presultloglistasm);
																printf(linefour, "%7f\n", *ptimelistc);
																printf(linefive, "%7f\n", *ptimelistasm);
																printf("%s\n", sepline);
																ploglist++;
																presultloglistc++;
																presultloglistasm++;
																ptimelistc++;
																ptimelistasm++;
								}
}
