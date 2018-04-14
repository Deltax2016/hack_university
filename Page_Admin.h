//
//  HTML главная странициа
//

const char PAGE_AdminMainPage[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<div class="container">
<div class="row">
<div class="col-md-12 col-sm-12 col-lg-12 col-lg-offset-3"><h1>Управление устройством.</h1></div>
<div class="col-md-12 col-sm-12 col-lg-12 "><hr></div>
<div class="col-md-12 col-sm-12 col-lg-12 col-lg-offset-4"><a href="/text" style="width:250px" class="btn btn--m btn--green" >Текст</a><br></div>
<div class="col-md-12 col-sm-12 col-lg-12 col-lg-offset-4"><a href="config.html" style="width:250px" class="btn btn--m btn--green" >Настройки сети</a><br></div>
<div class="col-md-12 col-sm-12 col-lg-12 col-lg-offset-4"><a href="update"   style="width:250px"  class="btn btn--m btn--green" >Магазин приложений</a><br></div>
</div>
</div>
<script>
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


