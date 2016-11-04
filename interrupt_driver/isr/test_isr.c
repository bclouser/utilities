
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

// These are the linux numbers! The GIC number would be this number - 32
//static int irqNum = 139;
static int irqNum = 106;
//static int irqNum = 74;
//static int irqNum = 139;
static int timerCount =0;
static int interruptCount = 0;

struct pDev {
	const char* name;
	int someVal;
};

struct pDev devId = {"isr_test", 12};

irq_handler_t interruptHandler(void)
{
	printk(KERN_ALERT "interrupt caught! Open the champagne! \n");
	interruptCount++;

	//tryna clear dat interrupt flag
	return IRQ_HANDLED;
}


static int __init interrupt_init(void)
{
	int irq;

	//irq = request_irq( (73) , interruptHandler, 0, "custom_gpio_interrupt", &devId);
	//irq = request_irq( (106) , interruptHandler, 0, "custom_gpio_interrupt", &devId);
	//irq = request_irq( (74) , interruptHandler, 0, "custom_gpio_interrupt", &devId);
	irq = request_irq( (irqNum) , interruptHandler, 0, "custom_gpio_interrupt", &devId);

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

	//free at last
	free_irq( irqNum , &devId);


	printk(KERN_ALERT "Isr module unloaded Successfully \n");
}


module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Clouser bclouser@idirectgt.com");
