
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "devs/lora.h"
#include "drvs/spi.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
          printf("Usage: %s /dev/hidrawX\n", argv[0]);
          exit(1);
    }

    _fd = open(argv[1], O_RDWR);

    uint8_t ver = SPIReadReg(LR_RegVERSION);
    printf("Found SX127X chip version: %02x\n", ver);

    lora_reset();
    lora_config(LR_EXPLICIT_HEADER_MODE);

    lora_rxInit(LR_Mode_RXCONTINUOUS, 0);

    for (uint8_t i = 0; i < 25; i++) {
        uint8_t rand = lora_readRandomNoise();
        printf("%i\n", rand);
    }

    close(_fd);

    return 0;
}
