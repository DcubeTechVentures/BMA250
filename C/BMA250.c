// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMA250
// This code is designed to work with the BMA250_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=BMA250_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main()
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, BMA250 I2C address is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x18);

	// Select range selection register(0x0F)
	// Set range +/- 2g(0x03)
	char config[2] = {0};
	config[0] = 0x0F;
	config[1] = 0x03;
	write(file, config, 2);
	// Select bandwidth register(0x10)
	// Bandwidth = 7.81 Hz(0x08)
	config[0] = 0x10;
	config[1] = 0x08;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from register(0x02)
	// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
	char reg[1] = {0x02};
	write(file, reg, 1);
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else 
	{
		// Convert the data to 10-bits
		int xAccl = (data[1] * 256 + (data[0] & 0xC0)) / 64;
		if(xAccl > 511)
		{
			xAccl -= 1024;
		}

		int yAccl = (data[3] * 256 + (data[2] & 0xC0)) / 64;
		if(yAccl > 511)
		{
			yAccl -= 1024;
		}

		int zAccl = (data[5] * 256 + (data[4] & 0xC0)) / 64;
		if(zAccl > 511)
		{
			zAccl -= 1024;
		}

		// Output data to screen
		printf("Acceleration in X-Axis : %d \n", xAccl);
		printf("Acceleration in Y-Axis : %d \n", yAccl);
		printf("Acceleration in Z-Axis : %d \n", zAccl);
	}
}
