
const char PAGE_Action[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<div class="container">
<div class="row">
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<div class="col-md-12 col-sm-12 col-lg-12"><h1 style="text-align: center;">Текст для дисплея</h1></div>
<div class="col-md-12 col-sm-12 col-lg-12 "><hr></div>
<form action="" method="get">
<div class="col-md-12 col-sm-12 col-lg-12 col-lg-offset-4 col-sm-offset-2"><input type="text" style="width:250px;margin:10px" id="text" name="text" value=""><br></div>
<div class="col-md-12 col-sm-12 col-lg-12 col-lg-offset-4 col-sm-offset-2"><a href="javascript:GetState()" style="width:250px" class="btn btn--m btn--green">Сохранить</a><br></div>
</form>
</div>
</div>
<script>
function GetState()
{
  setValues("/check?=" + document.getElementById("text").value);
}
window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				//
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

)=====";

void drawText(String q)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(utf8rus(q));
  display.display();
  Serial.println(q);
}

void sendText()
{
	
	if (server.args() > 0 )
	{
		drawText(server.arg(0));			
	}
	else
	{
		server.send_P ( 200, "text/html", PAGE_Action ); 
	}
	Serial.println(__FUNCTION__); 
}

