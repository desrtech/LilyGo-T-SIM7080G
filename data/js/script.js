console.log("Carga script");

document.addEventListener("DOMContentLoaded", generaSystemVoltage);

function generaSystemVoltage() {
    // Create Humidity Gauge
    var gaugeHum = new RadialGauge({
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
};