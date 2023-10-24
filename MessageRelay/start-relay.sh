#!/bin/bash
function configure_relay(){
	sleep 5
	curl 'http://127.0.0.1:9191/create_interface?name=RadioNet0&port=/dev/ttyUSB0&baudrate=9600&type=0x1000'
	curl 'http://127.0.0.1:9191/create_interface?name=RadioNet1&port=/dev/ttyUSB1&baudrate=9600&type=0x1001'
	curl 'http://127.0.0.1:9191/bridge_interface'

}

function start_relay(){
	node relay.js
}

start_relay &
configure_relay
