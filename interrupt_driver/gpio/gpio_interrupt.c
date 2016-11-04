
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/timer.h>


#define PERIPH_BASE 0xFFFEC000
/* This is for portB */
#define GPIO_PORTA  0xFF708000
#define GPIO_PORTB  0xFF709000
#define GPIO_PORTC  0xFF70A000


static void __iomem *periphBase;
static volatile u32* gpioBase;
static int idNum = 196;
static struct timer_list timer1;
static int timerCount =0;
static int interruptCount = 0;

struct pDev {
	const char* name;
	int someVal;
};

struct pDev* gpioDev = {"GPIO_int", 12};

irq_handler_t interruptHandler(void* dev)
{
	int offset = 0x4C;
	int bitNum = 0;

	//printk(KERN_ALERT "interrupt caught! Open the champagne! \n");
	interruptCount++;
	gpioBase[offset>>2] |= (1<<bitNum);

	//tryna clear dat interrupt flag

	printk(KERN_ALERT "%s\n",((struct pDev*)dev)->name);

	return IRQ_HANDLED;
}

void timer1_routine(unsigned long data)
{
	int offset = 0x50;
	printk(KERN_ALERT"Inside Timer interrupt count-> %i.\n", interruptCount);
	if(gpioBase[offset>>2] & 0x01)
	{
		printk(KERN_ALERT "pin set \n");
	}
	else
		printk(KERN_ALERT "pin not set \n");

	mod_timer(&timer1, jiffies + HZ); /* restarting timer */
}


static int __init interrupt_init(void)
{
	int offset;
	int regVal;
	int irq;
	int bitNum = 0;

	if( (periphBase = ioremap(PERIPH_BASE, SZ_4K)) == NULL )
		   printk(KERN_ALERT "ioremap failed for periphBase \n");
	else
		printk(KERN_ALERT "ioremap worked! periphBase = %08X \n",periphBase);

	if( (gpioBase = (u32*)ioremap(GPIO_PORTA, SZ_4K)) == NULL )
		   printk(KERN_ALERT "ioremap failed for gpioBase \n");
	else
		printk(KERN_ALERT "ioremap worked! gpioBase = %08X \n",gpioBase);

	/* Timer inits */
	init_timer(&timer1);

	timer1.function = timer1_routine;
	timer1.data = 1;
	timer1.expires = jiffies + HZ; /* 1 second */
	add_timer(&timer1); /* Starting the timer */

	printk(KERN_ALERT"Timer Module loaded\n");

/*

	u32 distBase = (u32)periphBase + 0x1000;
	u32 cpuBase =  (u32)periphBase + 0x0100;

	printk(KERN_ALERT "distBase = %08X \n", distBase);
	printk(KERN_ALERT "cpuBase = %08X \n", cpuBase);

// distributor register defines
	u32 icdDcr    = distBase;
	u32 icdIser   = distBase + 0x100;
	u32 icdIptr   = distBase + 0x800;
	u32 icdIcfr   = distBase + 0xC00;

// cpu interface register defines
	u32 iccIcr = cpuBase;
	u32 iccPmr = cpuBase + 0x04;
	printk(KERN_ALERT "icdDcr = %08X \n", icdDcr);
	printk(KERN_ALERT "icdIser = %08X \n", icdIser);





//   Distributor setup
	//don't think, disable first... #writeAccess
	//writel(0x0, (u32*)icdDcr);
	//printk(KERN_ALERT "just wrote icdDcr \n");
	// 197 is the GIC interrupt number for portb
	// following the architecture spec 1.0: ID number = N, N=197
	// icdIser number = M = (N/32), M = (197/32) = 0x6
	// offset = 4*M = 4*6 = 0x18
	printk(KERN_ALERT "value of idNum where compiler said it wasn't initialized... %d", idNum);
	offset = 4*(idNum/32);

	// the bit number of the required set-enable bit in this register is: N % 32
	// regOffset = 197 % 32 =  0x5
	regVal = idNum % 32;

	//alt_int_dist_target_set. set icdIptr to point to cpu0
	//offset = 4*(197/4) = 197 -> to hex = c5
	offset = 4*(idNum/4);
	writel( 0x01, (u32*)(icdIptr + offset) );
	printk(KERN_ALERT "just wrote to icdIptr value read back %08X\n", readl( (u32*)(icdIptr + offset) ) );

	// alt_int_dist_trigger_set. Set Interrupt Configuration Registers (ICDICFR) to edge triggered.
	offset = (idNum/16) * 4;
	//writel( 1 << (((idNum & 0x0F) * 2) + 1), (u32*)(icdIcfr + offset) );
	printk(KERN_ALERT "just wrote to icdIcfr \n");

//   CPU STUFF
 	// Priority Mask. Enable interrupts for highest priority
	//any interrupt with priority higher than 0 will be sent along
	// Setting the interrupt priority
	printk(KERN_ALERT "(u32*)iccPmr = %08X \n", (u32*)iccPmr);
	writel(0xFFFF, (u32*)iccPmr);  //setting this to 0x00 and even 0xAFFF hung the processor.
	printk(KERN_ALERT "just wrote to iccPmr \n");

	// Enable signaling of interrupt, non secure
	printk(KERN_ALERT "(u32*)iccIcr = %08X \n", (u32*)iccIcr);
	writel(0x3, (u32*)iccIcr);  //fails with 0x2 ... no idea why
	printk(KERN_ALERT "just wrote to iccIcr \n");


	// Enabling the interrupt
	//alt_int_dist_enable
	printk(KERN_ALERT "(u32*)iccPmr = %08X \n", (u32*)icdIser);
	offset = 4*(idNum/32);

	writel( 1<<(idNum%32), (u32*)(icdIser + offset));
	printk(KERN_ALERT "just wrote to icdIser \n");"Object-Oriented Programming" by Coad and Nicola


	//Configure the Distributor Control Register (ICDDCR) to send pending interrupts to cpus
	//basically enable it after having configured it all. Global enable
	writel(0x01, (u32*)(icdDcr));
*/

// setup the gpio interrupt
	//we only care about portb...
	//offset for the gpio_inten bit

	//bitNum = 26;  //for the enet interrupt on portB
	bitNum = 0;


	//status clear
	offset = 0x40;
	gpioBase[offset>>2] = 0;
	//writel(0x00, (gpioBase + offset>>2) );

	//we care about pin 26
	//offset for gpio_intmask
	offset = 0x34;
	gpioBase[offset>>2] &= ~(1<<bitNum);
	//writel(1<<bitNum, (gpioBase + offset>>2));

	//offset for gpio_inttype_level
	offset = 0x38;
	gpioBase[offset>>2] |= 1<<bitNum;
	//set 1 makes the interrupt edge sensitive
	//ritel(1<<bitNum, gpioBase + offset>>2));

	//offset for gpio_int_polarity
	offset = 0x3C;
	//set 1 makes the interrupt triggered on active high
	gpioBase[offset>>2] |= 1<<bitNum;
	//writel(1<<bitNum, (gpioBase + offset>>2));

	//enable interrupt on bit 26
	offset = 0x30; //offset with respect to port offset
	gpioBase[offset>>2] |= 1<<bitNum;
	//writel( 1<<bitNum, (gpioBase + offset>>2));


	irq = request_irq( (idNum) , interruptHandler, 0, "custom_gpio_interrupt", gpioDev);

	if(irq == 0)
	{
		printk(KERN_ALERT "Success Stories! we registered an interrupt line! \n");
	}
	else
	{
		printk(KERN_ALERT "Things did not work out with the interrupt line...error: %d \n", irq);
	}

	printk(KERN_ALERT "Finished init\n");

    return 0;
}

static void __exit interrupt_exit(void)
{
	printk(KERN_ALERT "Goodbye \n");
	//atleast perform some io-unmapping
	iounmap(periphBase);
	iounmap(gpioBase);
	//free at last
	free_irq( idNum , gpioDev);

	del_timer_sync(&timer1); /* Deleting the timer */

	printk(KERN_ALERT "Timer module unloaded \n");

}


module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Clouser bclouser@idirectgt.com");
