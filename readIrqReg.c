
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


                                 //0xFFFED000
const unsigned int fpgaIrq_0_1 = 0xFFFEDD0C;
const unsigned int offsetToMap = 0xFFFED000; // Interrupt distributor base offset
const unsigned int sizeToMap = 0x10000;
unsigned int extraOffset;

int main()
{
	
	int fd = open("/dev/mem", O_RDWR);
    if(fd == (-1) )
    {
        printf("couldn't open /dev/mem \n");
        return -1;
    }

    printf("opened dev/mem \n");


	unsigned int pageSize = sysconf(_SC_PAGE_SIZE);

	/*
	printf("pageSize: 0x%0X\n", pageSize);
	if(fpgaIrq_0_1 % pageSize)
	{
		extraOffset = fpgaIrq_0_1 % pageSize;
		offsetToMap = fpgaIrq_0_1 - extraOffset;

		printf("value of offset: 0x%08X\n", extraOffset);
		printf("Value of offsetToMap : 0x%08X\n", offsetToMap);
		
		sizeToMap = (0x10000);
	}
	*/



	printf("value of sizeToMap: 0x%08X\n", sizeToMap);
	//0x1000 b/c 4k page boundary found to work
    unsigned int virtualBaseAddr = (unsigned int)mmap( 0, sizeToMap, PROT_READ,
                                            MAP_PRIVATE, fd, offsetToMap);

    if((int)virtualBaseAddr == -1)
    {
    	printf("Mmmap failed\n");
    	return -1;
    }

    printf("mmap worked... supposedly. value of m_virtualBaseAddr: 0x%08X\n", virtualBaseAddr);

    // increment before casting to an int, otherwise we are going by 4 byte increments #pointerArithmetic
    unsigned volatile int * spi_31_00 = (unsigned volatile int*)(virtualBaseAddr + 0xD04);
    unsigned volatile int * spi_63_32 = (unsigned volatile int*)(virtualBaseAddr + 0xD08);
    unsigned volatile int * spi_95_64 = (unsigned volatile int*)(virtualBaseAddr + 0xD0C);

    unsigned int * ppi_if_n = (unsigned int*)(virtualBaseAddr + 0xD00);
    
    printf("Value of 0xD0C (SPI[95:64]): 0x%08X\n", *(unsigned int*)(virtualBaseAddr + 0xD0C));
    

    printf("Value of 0xFF0(component id0): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFF0));
    printf("Value of 0xFF4(component id1): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFF4));
    printf("Value of 0xFF8(component id2): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFF8));
    printf("Value of 0xFFC(component id3): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFFC));
  


    printf("Value of 0xFE0(periphID[0]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFE0));
    printf("Value of 0xFE4(periphID[1]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFE4));
    printf("Value of 0xFE8(periphID[2]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFE8));
    printf("Value of 0xFEC(periphID[3]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFEC));
    printf("Value of 0xFD0(periphID[4]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFD0));
    printf("Value of 0xFD4(periphID[5]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFD4));
    printf("Value of 0xFD8(periphID[6]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFD8));
    printf("Value of 0xFDC(periphID[7]): 0x%08X\n", *(unsigned volatile int*)(virtualBaseAddr + 0xFDC));



    while(1<2)
	{

        

        printf("MappedBase: 0x%08X, Offset: 0x204(SetPend4SPI[31:00]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x204));
        printf("MappedBase: 0x%08X, Offset: 0x208(SetPend4SPI[63:32]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x208));
        printf("MappedBase: 0x%08X, Offset: 0x20C(SetPend4SPI[95:64]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x20C));

        printf("MappedBase: 0x%08X, Offset: 0x280(ClrPendPPI-SGI). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x280));
        printf("MappedBase: 0x%08X, Offset: 0x284(ClrPend4SPI[31:00]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x284));
        printf("MappedBase: 0x%08X, Offset: 0x288(ClrPend4SPI[63:32]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x288));
        printf("MappedBase: 0x%08X, Offset: 0x28C(ClrPend4SPI[95:64]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned volatile int*)(virtualBaseAddr+0x28C));
        //printf("MappedBase: 0x%08X, Offset: 0x280(ClrPend4SPI[31:00]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned int*)(virtualBaseAddr+0x291));
        //printf("MappedBase: 0x%08X, Offset: 0x280(ClrPend4SPI[31:00]). Register Value: 0x%08X\n", virtualBaseAddr, *(unsigned int*)(virtualBaseAddr+0x280));






		printf("MappedBase: 0x%08X, Offset: 0xD04(SPI[31:00]). Register Value: 0x%08X\n", virtualBaseAddr, *spi_31_00);
		printf("MappedBase: 0x%08X, Offset: 0xD08(SPI[63:32]). Register Value: 0x%08X\n", virtualBaseAddr, *spi_63_32);
		printf("MappedBase: 0x%08X, Offset: 0xD0C(SPI[95:64]). Register Value: 0x%08X\n", virtualBaseAddr, *spi_95_64);

		printf("MappedBase: 0x%08X, Offset: 0xD00(ppi_if<n> ). Register Value: 0x%08X\n", virtualBaseAddr, *ppi_if_n);

		printf("\n");
		usleep(500);
	}
}


