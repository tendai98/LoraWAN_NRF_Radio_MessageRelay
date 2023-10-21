// Driver for Lora radio

const { SerialPort } = require('serialport')

let port = null;
let dataTXCounter = 0
let dataRXCounter = 0
let radioTag = null
let radioType = 0

let messageDataBuffer = ""
let externalInterface = null
let externalInterfaceTag = null

function dataHandler(res){
        let data = res.toString().replace(/[\x00-\x09\x0B-\x1F\x7F-\x9F]/g, '')

	if(messageDataBuffer.includes("Z")){
		console.log(`[+] RADIO: DATA-TX [ ${radioTag} -> ${externalInterfaceTag} ]`)
		messageDataBuffer = messageDataBuffer.replace("Z","")
		externalInterface.writeDevice({data: messageDataBuffer})
		messageDataBuffer = ""
	}else{
		messageDataBuffer += data
	}
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
		radioType =  args.type
		console.log(`[+] INIT: RADIO INTERFACE: ${args.tag}`);
	}else{
		console.log(`[!] ACTIVE INTERFACE: ${radioTag}`);
	}
}


function set_bridge_interface(args){
        externalInterface = args.interface
        externalInterfaceTag = args.tag
        console.log(`[+] NET BRIDGE: ${radioTag} -> ${externalInterfaceTag}`)
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
	bridgeDevice: set_bridge_interface,
	writeDevice:write_radio_device,
	destroyDevice: destroy_radio_device,
	getDeviceInfo: get_radio_parameters
}
