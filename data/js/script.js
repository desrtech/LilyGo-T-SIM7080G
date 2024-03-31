console.log("Carga script");

var gateway = 'ws://'+window.location.hostname+'/ws';
var websocket;
window.addEventListener('load', onLoad);
window.addEventListener('load', onMessage);
// document.addEventListener('DOMContentLoaded', onMessage);

function onLoad(event) {
    console.log("On Load Function", event);
    initWebSocket();
}

function getReadings() {
    websocket.send("getReading");
}

function onOpen(event) {
    console.log("Connection open", event);
    getReadings();
}

function onClose(event) {
    console.log("Connection close");
    setTimeout(initWebSocket, 3000);
}

function onMessage(event) {
    var data = null;
    try {
        data = JSON.parse(event.data);
    } catch (error) {
        data = {};
    }
    
    document.getElementById('ipAddress').innerHTML = data.ip_address != undefined ? data.ip_address : "127.0.0.1";
    document.getElementById("systemVoltage").innerHTML = data.system_voltage != undefined ? data.system_voltage / 1000 : "5.100";
    document.getElementById("battVoltage").innerHTML = data.batt_voltage != undefined ? data.batt_voltage / 1000 : "3.300";
}

function initWebSocket() {
    console.log('Trying to open a Websocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}
