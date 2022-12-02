var vrefreshed=sessionStorage.getItem('refreshed');
if (typeof vrefreshed === 'undefined')
    vrefreshed = 0;

function time() {

    var d = new Date();
    var h = d.getHours();
    var m = d.getMinutes();
    var s = d.getSeconds();
  var sunrisehour = sessionStorage.getItem("sunrisehour");
    var sunriseminute = sessionStorage.getItem("sunriseminute");
    var sunrisesecond = sessionStorage.getItem("sunrisesecond");
  var sunsethour = sessionStorage.getItem("sunsethour");
var sunsetminute = sessionStorage.getItem("sunsetminute");
var sunsetsecond = sessionStorage.getItem("sunsetsecond");
    if(vrefreshed==0 && ((h==sunrisehour && m==sunriseminute && s==sunrisesecond) || (h==sunsethour && m==sunsetminute && s==sunsetsecond))){
        sessionStorage.setItem('refreshed', 1);
        vrefreshed=1;
        $('#spn').text('');
        location.reload();
    }else{
        if(vrefreshed==1 && ((h==sunrisehour && m==sunriseminute && s==sunrisesecond) || (h==sunsethour && m==sunsetminute && s==sunsetsecond))){
            // do nothing
        }else{
            sessionStorage.setItem('refreshed', 0);
            vrefreshed=0;
            $('#spn').text('');
        }
    }
}
//call time() function every 1000 milliseconds i.e. every second.
setInterval(time, 1000);