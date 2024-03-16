console.log("Carga script");

document.addEventListener("DOMContentLoaded", generaSystemVoltage);

// // Get current sensor readings when the page loads  
// window.addEventListener('load', getReadings);

var systemVoltageGauge;

function generaSystemVoltage() {
    // Create Humidity Gauge
    systemVoltageGauge = new RadialGauge({
        renderTo: 'system-voltage',
        value: 3295,
        width: 250,
        height: 250,
        units: "System Voltage (mv)",
        minValue: 0,
        maxValue: 8000,
        colorValueBoxRect: "#049faa",
        colorValueBoxRectEnd: "#049faa",
        colorValueBoxBackground: "#f1fbfc",
        valueInt: 2,
        majorTicks: [
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8"    
        ],
        minorTicks: 10,
        strokeTicks: true,
        highlights: [
            {"from": 0, "to": 1999, "color": "#FF0000"},
            {"from": 2000, "to": 2999, "color": "#FFA500"},
            {"from": 3000, "to": 3299, "color": "#F0E68C"},
            {"from": 3300, "to": 4400, "color": "#03C0C1"},
            {"from": 4400, "to": 4699, "color": "#F0E68C"},
            {"from": 4700, "to": 5999, "color": "#FFA500"},
            {"from": 6000, "to": 8000, "color": "#FF0000"}
        ],
        colorPlate: "#fff",
        borderShadowWidth: 0,
        borders: false,
        needleType: "line",
        colorNeedle: "#007F80",
        colorNeedleEnd: "#007F80",
        needleWidth: 2,
        needleCircleSize: 3,
        colorNeedleCircleOuter: "#007F80",
        needleCircleOuter: true,
        needleCircleInner: false,
        animationDuration: 1500,
        animationRule: "linear"
    }).draw();
    getReadings();
};

// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var myObj = JSON.parse(this.responseText);
        var systemVoltage = myObj.systemVoltage;
        systemVoltageGauge.value = systemVoltage;
      }
    }; 
    xhr.open("GET", "/dataJson", true);
    xhr.send();
}


if (!!window.EventSource) {
  console.log("Windows event source");
  var source = new EventSource("/events");
  
  source.addEventListener("open", function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener("error", function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener("message", function(e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener("ping", function(e) {
    console.log("Ping Mesage");
  }, false);

  source.addEventListener("new_readings", function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    systemVoltageGauge.value = myObj.systemVoltage;
  }, false);
}
