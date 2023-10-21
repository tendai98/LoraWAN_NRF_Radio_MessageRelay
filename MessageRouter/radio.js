const { SerialPort } = require('serialport')

let port = null;
let dataEventHandler = null
let dataTXCounter = 0
let dataRXCounter = 0
let radioTag = null

function dataHandler(data){
	let res = data.toString().replace(/[\x00-\x09\x0B-\x1F\x7F-\x9F]/g, '')
	dataTXCounter += res.length
	dataEventHandler(res)
}

function errorHandler(err){
	console.log(`[!] RADIO ERROR: ${err}`);
}

function create_radio_device(args){
	if(!port){
		port = new SerialPort({path: args.port, baudRate: args.baudRate})
		port.on('data', dataHandler)
		port.on('error', errorHandler)
		radioTag = args.tag
		console.log(`[+] INIT: RADIO INTERFACE: ${args.tag}`);
	}else{
		console.log(`[!] ACTIVE INTERFACE: ${radioTage}`);
	}
}

function destroy_radio_device(args){
	if(port){
		port.close()
		port = null
		console.log(`[+] RADIO INTERFACE: SHUTDOWN -> ${radioTag}`)
	}else{
		console.log(`[!] RADIO OFFLINE: ${radioTag}`);
	}
}

function set_radio_handler(args){
	dataEventHandler = args.handler
}

function get_radio_parameters(args){
	return {tag: radioTag, tx: dataTXCounter, rx: dataRXCounter}
}

function write_radio_device(args){
	port.write(args.data, function(err){
		if(err){
			console.log("[!] RADIO INTERFACE: WRITE ERROR")
		}else{
			dataTXCounter += args.data.length
		}
	})
}


module.exports = {
	createDevice: create_radio_device,
	writeDevice:write_radio_device,
	destroyDevice: destroy_radio_device,
	getDeviceInfo: get_radio_parameters,
	setMessageHandler: set_radio_handler
}
