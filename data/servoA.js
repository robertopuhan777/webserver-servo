window.addEventListener('load', getSERVO_readings);

// Create gaugeServoA Gauge
var gaugeServoA = new RadialGauge({
  renderTo: 'gauge-servoA',
  width: 300,
  height: 300,
  units: "Servo A \u00B0",
  minValue: 0,
  startAngle: 90, 
  ticksAngle: 180,
  maxValue: 180,
  colorValueBoxRect: "transparent",
  colorValueBoxRectEnd: "transparent",
  colorValueBoxBackground: "transparent",
  colormajorTicks: "grey",
  valueDec: 1,
  valueInt: 1,
  majorTicks: [
      "0",
	  " ",
	  "20",
	  " ",
	  "40",
	  " ",
	  "60",
	  " ",
	  "80",
	  " ",
	  "100",
	  " ",
	  "120",
	  " ",
	  "140",
	  " ",
	  "160",
	  " ",
	  "180"
  ],
  minorTicks: 10,
  strokeTicks: true,
  highlights: [
      {
          "from": 7,
          "to": 10,
          "color": "rgba(200, 50, 50, 0)"
      }
  ],
  colorPlate: "transparent",
  colorBarProgress: "#0000ff",
  colorBarProgressEnd: "#0000ff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 1,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 75,
  animationRule: "linear",
  barWidth: 10,
}).draw();

// Function to get current readings on the webpage when it loads for the first time
function getSERVO_readings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var ServoA = myObj.SLIDERVALUE1;
      gaugeServoA.value = ServoA;
    }
  }; 
  xhr.open("GET", "/owmreadings1", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');
  
  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);
  
  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);
  
  source.addEventListener('SERVO_readings', function(e) {
    console.log("SERVO_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    gaugeServoA.value = myObj.SLIDERVALUE1;
  }, false);
}