console.log("Carga script");

document.addEventListener("DOMContentLoaded", generaSystemVoltage);

// // Get current sensor readings when the page loads  
// window.addEventListener('load', getReadings);

// var systemVoltageGauge;
var gateway = 'ws://'+window.location.hostname+'/ws';
var websocket;
window.addEventListener('load', onLoad);

function generaSystemVoltage() {
  // Create Humidity Gauge
    systemVoltageGauge = new RadialGauge({
        renderTo: 'system-voltage',
        width: 200,
        height: 200,
        value: 1.1,
        units: "V",
        minValue: 4.000,
        startAngle: 90,
        ticksAngle: 90,
        valueBox: true,
        maxValue: 6.000,
        valueInt: 1,
        valueDec: 2,
        majorTicks: [
            "4.0",
            "5.0",
            "6.0"
        ],
        minorTicks: 10,
        strokeTicks: true,
        highlights: [
            {"from": 4.000, "to": 4.599, "color": "#eeffb9"},
            {"from": 4.600, "to": 5.400, "color": "#a1fd54"},
            {"from": 5.401, "to": 6.000, "color": "#eeffb9"},
        ],
        colorPlate: "#fff",
        borderShadowWidth: 0,
        borders: false,
        needleType: "arrow",
        needleWidth: 2,
        needleCircleSize: 7,
        needleCircleOuter: true,
        needleCircleInner: false,
        animationDuration: 1500,
        animationRule: "linear" 
    }).draw();

    battVoltageGauge = new RadialGauge({
        renderTo: 'batt-voltage',
        width: 200,
        height: 200,
        value: 1.1,
        units: "V",
        minValue: 3,
        startAngle: 90,
        ticksAngle: 90,
        valueBox: true,
        valueInt: 1,
        valueDec: 2,
        maxValue: 5,
        majorTicks: [
            "3.0",
            "4.0",
            "5.0"
        ],
        minorTicks: 10,
        strokeTicks: true,
        highlights: [
            {"from": 3.000, "to": 3.599, "color": "#eeffb9"},
            {"from": 3.600, "to": 4.400, "color": "#a1fd54"},
            {"from": 4.401, "to": 5.000, "color": "#eeffb9"},
        ],
        colorPlate: "#fff",
        borderShadowWidth: 0,
        borders: false,
        needleType: "arrow",
        needleWidth: 2,
        needleCircleSize: 7,
        needleCircleOuter: true,
        needleCircleInner: false,
        animationDuration: 1500,
        animationRule: "linear" 
    }).draw();
};

// Function to get current readings on the webpage when it loads for the first time
// function getReadings(){
//     var xhr = new XMLHttpRequest();
//     xhr.onreadystatechange = function() {
//       if (this.readyState == 4 && this.status == 200) {
//         var myObj = JSON.parse(this.responseText);
//         var systemVoltage = myObj.system_voltage;
//         systemVoltageGauge.value = systemVoltage;
//       }
//     }; 
//     xhr.open("GET", "/dataJson", true);
//     xhr.send();
// };

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
    console.log("Procesa mensajes recibidos" + event.data);
    var data = JSON.parse(event.data);

    systemVoltageGauge.value = data.system_voltage / 1000;
    battVoltageGauge.value = data.batt_voltage / 1000;
    document.getElementById('ip_address').innerHTML = data.ip_address;
    // var systemaVoltage = data.system_voltage;
    // var battVoltage = data.batt_voltage;
    // window.addEventListener()
}

function initWebSocket() {
    console.log('Trying to open a Websocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
    console.log(gateway);
}


// if (!!window.EventSource) {
//   console.log("Windows event source");
//   var source = new EventSource("/events");
  
//   source.addEventListener("open", function(e) {
//     console.log("Events Connected");
//   }, false);

//   source.addEventListener("error", function(e) {
//     if (e.target.readyState != EventSource.OPEN) {
//       console.log("Events Disconnected");
//     }
//   }, false);

//   source.addEventListener("message", function(e) {
//     console.log("message", e.data);
//   }, false);

//   source.addEventListener("ping", function(e) {
//     console.log("Ping Mesage");
//   }, false);

//   source.addEventListener("new_readings", function(e) {
//     console.log("new_readings", e.data);
//     var myObj = JSON.parse(e.data);
//     console.log(myObj);
//     systemVoltageGauge.value = myObj.systemVoltage;
//   }, false);
// }
