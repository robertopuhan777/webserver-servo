$('btn1').on('click', function() {
    $('meta[name=theme-color]').remove();
    $('head').append( '<meta name="theme-color" content="black">' );
    document.body.style.transition = "background 0.2s,color 0.2s";
    document.body.style.backgroundColor = "#000000";
    document.body.style.color = "white";
    document.getElementById("btn1").style.color = "#ED6E4C";
	document.getElementById("btn2").style.color = "darkgrey";
	document.getElementById(btn2).style.display = "none";
});
