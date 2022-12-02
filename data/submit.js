function handleSubmit () {
			const sunrisehour = document.getElementById('sunrisehour').value;
            const sunriseminute = document.getElementById('sunriseminute').value;
            const sunrisesecond = document.getElementById('sunrisesecond').value;
			const sunsethour = document.getElementById('sunsethour').value;
            const sunsetminute = document.getElementById('sunsetminute').value;
            const sunsetsecond = document.getElementById('sunsetsecond').value;
            sessionStorage.setItem("sunrisehour", sunrisehour);
            sessionStorage.setItem("sunriseminute", sunriseminute);
			sessionStorage.setItem("sunrisesecond", sunrisesecond);
			sessionStorage.setItem("sunsethour", sunsethour);
            sessionStorage.setItem("sunsetminute", sunsetminute);
			sessionStorage.setItem("sunsetsecond", sunsetsecond);
            return;
            }
var date = new Date();
var hour = date.getHours();
var minute = date.getMinutes();
var second = date.getSeconds();
if (hour < 10) {hour = "0"+hour;}
if (minute < 10) {minute = "0"+minute;}
if (second < 10) {second = "0"+second;}
var currentTime = ([hour, minute, second].join('\n'));

document.write(currentTime);
document.write('<br>');
var sunrisehour = sessionStorage.getItem("sunrisehour");
var sunriseminute = sessionStorage.getItem("sunriseminute");
var sunrisesecond = sessionStorage.getItem("sunrisesecond");
var sunriseTime = ([sunrisehour, sunriseminute, sunrisesecond].join('\n'));
document.write(sunriseTime);
document.write('<br>');
var sunsethour = sessionStorage.getItem("sunsethour");
var sunsetminute = sessionStorage.getItem("sunsetminute");
var sunsetsecond = sessionStorage.getItem("sunsetsecond");
var sunsetTime = ([sunsethour, sunsetminute, sunsetsecond].join('\n'));
document.write(sunsetTime);
document.write('<br>');
if (document.body) {
    if (sunriseTime <= currentTime && currentTime < sunsetTime) {
   document.body.style.backgroundColor = 'white';
	document.body.style.color = 'black';
	$('meta[name=theme-color]').remove();
    $('head').append( '<meta name="theme-color" content="white">' );
    document.body.style.transition = "background 0.2s,color 0.2s";
    document.body.style.backgroundColor = "#ffffff";
    document.body.style.color = "black";
    document.getElementById("btn2").style.color = "darkorange";
	document.getElementById("btn1").style.color = "black";
    }
	
    else {
   document.body.style.backgroundColor = 'black';
	document.body.style.color = 'white';
	$('meta[name=theme-color]').remove();
    $('head').append( '<meta name="theme-color" content="black">' );
    document.body.style.transition = "background 0.2s,color 0.2s";
    document.body.style.backgroundColor = "#000000";
    document.getElementById("btn1").style.color = "darkorange";
	document.getElementById("btn2").style.color = "white";
    }

}