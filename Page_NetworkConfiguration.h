

//
//  HTML PAGE
//
const char PAGE_NetworkConfiguration[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Настройки сети</strong>
<hr>
Настройка соединения с РОУТЕРОМ:<br>

<strong>Состояние соединения:</strong><div id="connectionstate">Нет данных</div>
<hr>
<strong>Сети:</strong><br>
<table border="0"  cellspacing="3" style="width:310px" >
<tr><td><div id="networks">Сканирование...</div></td></tr>
<tr><td align="center"><a href="javascript:GetState()" style="width:150px" class="btn btn--m btn--green">Обновить</a></td></tr>
</table>

<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="right">SSID:</td><td><input type="text" id="ssid" name="ssid" value=""></td></tr>
<tr><td align="right">Пароль:</td><td><input type="password" id="password" name="password" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--green" value="Сохранить"></td></tr>
</table>
</form>
<hr>


<br>
<script>

function GetState()
{
	setValues("/admin/connectionstate");
}
function selssid(value)
{
	document.getElementById("ssid").value = value; 
}


window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
					setValues("/admin/values");
					setTimeout(GetState,3000);
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}




</script>


)=====";

const char PAGE_WaitAndReload[] PROGMEM = R"=====(
<meta http-equiv="refresh" content="5; URL=config.html">
Пожалуйста подождите.... Приминение конфигурации и перезапуск.
)=====";


//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
// 

void send_network_configuration_html()
{
	
	if (server.args() > 0 )  // Save Settings
	{
		String temp = "";
		config.dhcp = false;
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			if (server.argName(i) == "ssid") config.ssid =   urldecode(server.arg(i));
			if (server.argName(i) == "password") config.password =    urldecode(server.arg(i));
		}
		 server.send_P ( 200, "text/html", PAGE_WaitAndReload );
    WriteConfig();
   // ConfigureWifi();
   ESP.reset();

				
	}
	else
	{
		server.send_P ( 200, "text/html", PAGE_NetworkConfiguration ); 
	}
	Serial.println(__FUNCTION__); 
}



//
//   FILL THE PAGE WITH VALUES
//

void send_network_configuration_values_html()
{

	String values ="";

	values += "ssid|" + (String) config.ssid + "|input\n";
	values += "password|" +  (String) config.password + "|input\n";
	server.send ( 200, "text/plain", values);
	Serial.println(__FUNCTION__); 
	
}


//
//   FILL THE PAGE WITH NETWORKSTATE & NETWORKS
//

void send_connection_state_values_html()
{

	String state = "НЕТ ДАННЫХ";
	String Networks = "";
	if (WiFi.status() == 0) state = "Idle";
	else if (WiFi.status() == 1) state = "НЕТ ДОСТУПНЫХ СЕТЕЙ";
	else if (WiFi.status() == 2) state = "СКАНИРОВАНИЕ ЗАВЕРШЕНО";
	else if (WiFi.status() == 3) state = "СОЕДИНЕНИЕ УСТАНОВЛЕНО";
	else if (WiFi.status() == 4) state = "ОШИБКА СОЕДИНЕНИЯ";
	else if (WiFi.status() == 5) state = "РАЗЪЕДИНЕНО";
	else if (WiFi.status() == 6) state = "НЕТ СОЕДИНЕНИЯ";



	 int n = WiFi.scanNetworks();

	 if (n == 0)
	 {
		 Networks = "<font color='#FF0000'>Нет доступных сетей!</font>";
	 }
	else
    {
	 
		
		Networks = "Доступно сетей: " +String(n) + "<br>";
		Networks += "<table border='0' cellspacing='0' cellpadding='3'>";
		Networks += "<tr bgcolor='#DDDDDD' ><td><strong>Имя</strong></td><td><strong>Качество</strong></td><td><strong>Защита</strong></td><tr>";
		for (int i = 0; i < n; ++i)
		{
			int quality=0;
			if(WiFi.RSSI(i) <= -100)
			{
					quality = 0;
			}
			else if(WiFi.RSSI(i) >= -50)
			{
					quality = 100;
			}
			else
			{
				quality = 2 * (WiFi.RSSI(i) + 100);
			}


			Networks += "<tr><td><a href='javascript:selssid(\""  +  String(WiFi.SSID(i))  + "\")'>"  +  String(WiFi.SSID(i))  + "</a></td><td>" +  String(quality) + "%</td><td>" +  String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*")  + "</td></tr>";
		}
		Networks += "</table>";
	}
   
	String values ="";
	values += "connectionstate|" +  state + "|div\n";
	values += "networks|" +  Networks + "|div\n";
	server.send ( 200, "text/plain", values);
	Serial.println(__FUNCTION__); 
	AdminTimeOutCounter=0;
}


