package main

import (
	"fmt"
	"log"
	"net/http"
)

/*
I have a cron job that just sends http requests to the /register-ip endpoint
with relevant information stuff into headers just so i can see it
*/

// some hot globals
var setIP string
var setDate string
var setHost string
var setUp string

func ipHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Ok, well. Got a request to update the ip address.")
	setIP = r.Header.Get("ip-address")
	setDate = r.Header.Get("date")
	setHost = r.Header.Get("host-name")
	setUp = r.Header.Get("up-time")

	fmt.Println("Set ip to: " + setIP)
	fmt.Println("Set date to: " + setDate)
	fmt.Println("Set host to: " + setHost)
	fmt.Println("Set upTime to: " + setUp)

	fmt.Fprintf(w, "Ben here, Ack. IP set")
}

func rootHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "IP Set: "+setIP+"   hostname: "+setHost+"    at: "+setDate+"    up for: "+setUp)
}

func main() {
	http.HandleFunc("/", rootHandler)
	http.HandleFunc("/register-ip", ipHandler)
	log.Fatal(http.ListenAndServe(":8080", nil))
}
