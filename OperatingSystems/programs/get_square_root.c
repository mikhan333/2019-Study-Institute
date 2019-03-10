#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[], char* env[])
{
	int i = 0;

	while(env[i] != NULL)
	{
		int comp = strncmp(env[i], "mysqrt=", 7);
		if (comp == 0)
		{
			int number = atoi(env[i]+7);
			if (number == 0){ printf("that's string\n"); }
			else if(number < 0) { printf("that's negative\n"); }
			else {
				int a = number;
                        	int i;
                        	double first = 0;
                        	double second = 1;
                        	while(fabs(second - first) > 0.000001)
                        	{
                                	first = second;
                                	second = (first + a/first) / 2;
                        	}
                        	printf("\n%f\n", second);

			}
			break;
		}
		i++;
	}
	if(env[i] == NULL){ printf("dont have mysqrt\n"); }

	return 0;

}
