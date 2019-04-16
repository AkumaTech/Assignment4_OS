/*
Bankers Algorithm

Miguel Hernandez
Miguel Manuel
Jose Avina
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define MAX_COMMANDS 10

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/*the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// should return 0 if successful else -1
int request_resources(int customer_num, int request[]);

void release_resources(int customer_num, int release[]);

bool isSafe();//checks to see if the next procedure is safe
void displayAll();//displays all the info
void displayTable(int (*tab)[NUMBER_OF_RESOURCES], int c, int r);//used to display tables
bool checkAllLessThan(int *a1, int *a2, int n);//checks if all int in a2 are less than a1
bool checkAnyLessThan(int *a1, int *a2, int n);//checks if any int in a1 are less than a2

int main(int argc, char *argv[])
{
int i, j;//for the loops and nested loops
FILE *fin = NULL;
char command[MAX_COMMANDS];
int input[9];

if(argc <= NUMBER_OF_RESOURCES)
	{
	printf("error\n");
	return 1;
	}
for(i = 0; i < NUMBER_OF_RESOURCES; ++i)
	available[i] = atoi(argv[i+1]);

fin = fopen("maxR.txt", "r");//allows for the reading of the maxR.txt file
if(fin == NULL)
	{
	printf("error, no such file exists.\n");
	return 1;
	}
for(i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
	{
	fscanf(fin, "%d", &maximum[i][0]);
	for(j = 1; j < NUMBER_OF_RESOURCES; ++j)
		{
		fgetc(fin);
		fscanf(fin, "%d", &maximum[i][j]);
		}
	}
fclose(fin);

for(i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
	{
	for(j = 0; j < NUMBER_OF_RESOURCES; ++j)
		{
		allocation[i][j] = 0;
		need[i][j] = maximum[i][j];
		}
	}

while(true)
	{
	printf("Command is ");
	scanf("%s", command);
	if(strcmp(command, "*") == 0)
		displayAll();
	else if(strcmp(command, "RQ") == 0)
		{
		scanf("%d", &j);
		for(i = 0; i < NUMBER_OF_RESOURCES; ++i)
			scanf("%d", &input[i]);
		if(!request_resources(j, input))
			printf("Request granted.\n");
		}
	else if(strcmp(command, "RL") == 0)
		{
		scanf("%d", &j);
		for(i = 0; i < NUMBER_OF_RESOURCES; ++i)
			scanf("%d", &input[i]);
		release_resources (j, input);	
		printf("Resources released.\n");
		}
	else if(strcmp(command, "exit") == 0)
		break;
	else
		{
		printf("Invalid command.\n");
		continue;
		}
	}
return 0;
}

bool checkAllLessThan(int *a1, int *a2, int n)
{
for(int i = 0; i < n; ++i)
	{
	if(a2[i] < a1[i])
		return false;
	}
return true;
}

bool checkAnyLessThan(int *a1, int *a2, int n)
{
for(int i = 0; i < n; ++i)
	{
	if(a1[i] < a2[i])
		return true;
	}
return false;
}

void displayTable( int (*tab)[NUMBER_OF_RESOURCES], int c, int r)
{
int i, j;
for(i = 0; i < c; ++i)
	{
	for(j = 0; j < r; ++j)
		printf("%d", tab[i][j]);
	printf("\n");
	}
}

void displayAll()
{
printf("Available\n");
for(int i = 0; i < NUMBER_OF_RESOURCES; ++i)
	printf("%d", available[i]);
printf("\n");

printf("Need\n");
displayTable(need, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
printf("Allocation\n");
displayTable(allocation, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
printf("Maximum\n");
displayTable(maximum, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
}

bool isSafe()
{
int w[NUMBER_OF_RESOURCES];
bool finish[NUMBER_OF_CUSTOMERS];
int i, j;
bool q_flag;
bool info = false;

for(i = 0; i < NUMBER_OF_RESOURCES; ++i)
	w[i] = available[i];
for(i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
	finish[i] = false;

while(true)
	{
	q_flag = true;
	for(i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		{
		if((!finish[i]) && checkAllLessThan(need[i], w, NUMBER_OF_RESOURCES))
			{
			q_flag = false;
			for(j = 0; j < NUMBER_OF_RESOURCES; ++j)
				w[j] += allocation[i][j];
			finish[i] = true;
			}
		}
	if(q_flag)
		break;
	}
for(i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
	{
	if(!finish[i])
		return false;
	}

return true;
}

int request_resources(int customer_num, int * request)
{
if(customer_num < 0 || customer_num >= NUMBER_OF_CUSTOMERS)
	{
	printf("invalid customer number.\n");
	return -1;
	}
if(checkAnyLessThan(need[customer_num], request, NUMBER_OF_RESOURCES))
	{
	printf("error.\n");
	return -1;
	}
if(checkAnyLessThan(available, request, NUMBER_OF_RESOURCES))
	{
	printf("error.\n");
	return -1;
	}
if(isSafe())
	{
	for(int i = 0; i < NUMBER_OF_RESOURCES; ++i)
		{
		available[i] -= request[i];
		need[customer_num][i] -= request[i];
		allocation[customer_num][i] += request [i];
		}
	return (0);
	}
else
	return -1;


}

void release_resources(int customer_num, int * release)
{
int i;
if(checkAnyLessThan(allocation[customer_num], release, NUMBER_OF_RESOURCES))
	{
	for(i = 0; i < NUMBER_OF_RESOURCES; ++i)
		{
		if(allocation[customer_num][i] < release[i])
			release[i] = allocation[customer_num][i];
		}

	}
for(i = 0; i < NUMBER_OF_RESOURCES; ++i)
	{
	allocation[customer_num][i] -= release[i];
	need[customer_num][i] += release[i];
	available[i] += release[i];
	}
}












