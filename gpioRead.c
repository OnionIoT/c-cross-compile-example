#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <ugpio/ugpio.h>


int main(int argc, char **argv, char **envp)
{
	int i;
	int rq, rv;
	int gpio;
	int value;


	if (argc < 2) {
		printf("Usage: gpioRead <gpio>\n\n");
		printf("Reads the input value of the specified GPIO pin once a second for 20 seconds\n");
		exit(-1);
	}


	gpio = atoi(argv[1]);

	// check if gpio is already exported
	if ((rq = gpio_is_requested(gpio)) < 0)
	{
		perror("gpio_is_requested");
		return EXIT_FAILURE;
	}
	// export the gpio
	if (!rq) {
		printf("> exporting gpio\n");
		if ((rv = gpio_request(gpio, NULL)) < 0)
		{
			perror("gpio_request");
			return EXIT_FAILURE;
		}
	}

	// set to input direction
	printf("> setting to input\n");
	if ((rv = gpio_direction_input(gpio)) < 0)
	{
		perror("gpio_direction_input");
	}
	
	// read the gpio 20 times
	printf("> begin reading GPIO%d\n",gpio);
	for (i = 0; i < 20; i++) 
	{ 
		// read the gpio
		value = gpio_get_value(gpio);
		printf("  > Read GPIO%d: value '%d'\n", gpio, value);


		// pause between each read
		sleep(1);
	}


	// unexport the gpio
	if (!rq) {
		printf("> unexporting gpio\n");
		if (gpio_free(gpio) < 0)
		{
			perror("gpio_free");
		}
	}

	return 0;
}


