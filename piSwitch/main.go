package main

import (
	"fmt"
	"github.com/stianeikeland/go-rpio"
	"flag"
	"time"
)


func setPower(power bool){
	fmt.Println("Modifying power: ", power)
	err := rpio.Open()
	if err != nil {
    	fmt.Println("Failed to initialize gpio library")
    	return
	}
	pin := rpio.Pin(7)
	pin.Output()       // Output mode

	if power {
		pin.High()        // Set pin High
	} else {
		pin.Low()         // Set pin Low
	}

	rpio.Close()
	return
}


func main() {
	var numbPtr = flag.Int("p", 0, "Desired Power (on or off) [0|1]")
	var togglePtr = flag.Bool("t", false, "Toggle Power")
	flag.Parse()


	if *togglePtr {
		setPower(false)
		time.Sleep(100 * time.Millisecond)
		setPower(true)
		return
	}

	// if *numbPtr is not 0 then turn power on
	setPower(*numbPtr!=0)
}
