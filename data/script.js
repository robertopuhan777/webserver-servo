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

 
  source.addEventListener('SERVO_readings', function(e) {
    var obj = JSON.parse(e.data);
    
	document.getElementById("SLIDERVALUE1").innerHTML = obj.SLIDERVALUE1;
	document.getElementById("SLIDERVALUE2").innerHTML = obj.SLIDERVALUE2;
	
  }, false);
}

function resetPosition(element){
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/"+element.id, true);
  console.log(element.id);
  xhr.send();
}
