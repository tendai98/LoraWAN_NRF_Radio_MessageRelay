const express = require("express")
const PORT = 9191

let radioInterfaces = {}
let interface1 = null
let interface2 = null

const app = express()


function createInterface(req, res){

	let interfaceId = req.query.name
	let radioType = req.query.type
	let devicePort = req.query.port
	let deviceBaudrate = parseInt(req.query.baudrate)

	switch(radioType){
		case "0x1000":
			radioInterfaces[interfaceId] = require("./drivers/lorawanV1")
			interface1 = interfaceId
			radioInterfaces[interfaceId].createDevice({tag:interfaceId, port: devicePort, baudRate:deviceBaudrate})
			res.send(`[+] RADIO DEVICE ONLINE => ${interfaceId}:${radioType}`)
			break

		case "0x1001":
                	radioInterfaces[interfaceId] = require("./drivers/lorawanV2")
			interface2 = interfaceId
			let event = {interface:  radioInterfaces[interface1], tag: interface1}
                	radioInterfaces[interfaceId].createDevice({tag:interfaceId, port: devicePort, baudRate:deviceBaudrate})
			radioInterfaces[interface2].bridgeDevice(event)
                	res.send(`[+] RADIO DEVICE ONLINE => ${interfaceId}:${radioType}`)
			break

		default:
			res.send("[!] INVALID RADIO TYPE")
	}
}


function bridgeInterface(req, res){
	let event = {interface: radioInterfaces[interface2], tag: interface2}
	radioInterfaces[interface1].bridgeDevice(event)
	res.send("[+] RADIO BRIDGE: LINK UP")
}

function destroyInterface(req, res){
	let interfaceId = req.query.name
	if(radioInterfaces[interfaceId]){
		radioInterfaces[interfaceId].destroyDevice()
		res.send("[+] RADIO DEVICE SHUTDOWN")
	}else{
		res.send("[!] INVALID RADIO INTERFACE")
	}
}

function getInterfaceInfo(req, res){
	let interfaceId = req.query.name
	if(radioInterfaces[interfaceId]){
		let info = radioInterfaces[interfaceId].getDeviceInfo()
		res.send(`${info.tag},${info.tx},${info.rx}`)
	}else{
		res.send("[!] INVALID RADIO INTERFACE")
	}
}

app.get("/create_interface", createInterface)
app.get("/bridge_interface", bridgeInterface)
app.get("/destroy_interface", destroyInterface)
app.get("/interface_info", getInterfaceInfo)

app.listen(PORT, function (){
	console.log("[+] LORAWAN RADIO: DATA PACKET RELAY v1.0")
})
