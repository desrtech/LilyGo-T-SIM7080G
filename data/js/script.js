var gateway = 'ws://'+window.location.hostname+'/ws';
var websocket;
var gpsData;

var map;

function mapView(latitud, longitud) {
    var marker = new L.Marker([latitud, longitud]);
    var mapOptions = {
        center: [latitud, longitud],
        zoom: 16
    }

    map = new L.map('map', mapOptions);
    var layer = new L.TileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png');
    map.addLayer(layer);
    marker.addTo(map);
}

function updateMap(lati, long, map) {
    if (lati && long && map) {
        console.log("Update map");
        var marker = new L.Marker([lati, long]);
        marker.addTo(map);
        map.panTo(new L.LatLng(lati, long));
    }
    

    // Assuming you're using a Leaflet map
    // marker = L.marker([lat, lng]);
    // marker.addTo(map);
    // Update or reposition the marker on your map

    // Optionally, center the map on the new location
    // map.panTo(new L.LatLng(lat, lng));
};



window.addEventListener('load', onLoad);
window.addEventListener('load', onMessage);
// document.addEventListener('DOMContentLoaded', onMessage);

function onLoad(event) {
    var lat = -33.09821927222869;
    var lng = -71.73812543390552;
    mapView(lat, lng);
    initWebSocket();
    // setTimeout(() => {
    //     console.log("Timeout 5");
    //     updateMap(-33.0486653604574, -71.60520491006206, map);
    // }, 5000);
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
    gpsData = data.gpsData ? JSON.parse(data.gpsData) : JSON.parse('{"response":"error"}');
    var latitud = gpsData.latitud;
    var longitud = gpsData.longitud;
    updateMap(latitud, longitud, map);
    console.log('On message Latitud: ', latitud, ' - Lomgitud: ', longitud);
}

function initWebSocket() {
    console.log('Trying to open a Websocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}
