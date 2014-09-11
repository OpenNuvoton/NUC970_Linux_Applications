/* GPIO Driver Test/Example Program
 *
 * Compile with:
 *  gcc -s -Wall -Wstrict-prototypes gpio.c -o gpiotest
 *
 *
 * Note :
 *   PORT NAME[PIN] = GPIO [id]	
 *   PORTA[ 0]      = gpio[ 0x00]
 *   PORTA[ 1]      = gpio[ 0x01]	  
 *                  :
 *   PORTA[31]      = gpio[ 0x1F]
 *   PORTB[ 0]      = gpio[ 0x20]
 *                  :
 *   PORTB[31]      = gpio[ 0x3F]
 *                  :
 *                  :
 *                  :
 *   PORTI[ 0]      = gpio[ 0xC0]
 *                  :
 *                  :
 *   PORTI[31]      = gpio[ 0xDF]
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *fp;
char io[]={'A','B','C','D','E','F','G','H','I'};
char str[256];
int main(void)
{
	int num=0x47;	
	//linux equivalent code "echo 130 > export" to export the port 
	if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL) {
		printf("Cannot open export file.\n");
		exit(1);
	}
	fprintf(fp, "%d", num);
	fclose(fp);
	//  linux equivalent code "echo in > direction" to set the port as an input  
	sprintf(str,"/sys/class/gpio/gpio%d/direction",num);
	if ((fp = fopen(str, "rb+")) == NULL) {
		printf("Cannot open direction file.\n");
		exit(1);
	}
	fprintf(fp, "in");
	fclose(fp);

	// **here comes where I have the problem, reading the value**    
	char buffer[10];
	int value;
	sprintf(str,"/sys/class/gpio/gpio%d/value",num);
	while (1) {		
		if ((fp = fopen(str, "rb")) == NULL) {
			printf("Cannot open value file.\n");
		} else {
			fread(buffer, sizeof(char), sizeof(buffer) - 1, fp);
			value = atoi(buffer);
			printf("GPIO%C%d value: %d\n",io[num/0x20],num%(0x20),value);
			fclose(fp);
		}
	}

	return 0;
}

