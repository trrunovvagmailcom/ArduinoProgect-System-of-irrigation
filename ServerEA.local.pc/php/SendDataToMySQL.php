<?php

require ('E:/openserver/OSPanel/vendor/autoload.php');
use WebSocket\Client;

require ('/ParametersForMySQL.php');

$arrayName = array('timeout' => 61);
$client = new Client("ws://192.168.0.105:81",$arrayName);

$MySQL_CONNECTION_SEND = mysqli_connect($MySQL_host, $MySQL_user, $MySQL_pass, $MySQL_db_name);
if (!$MySQL_CONNECTION_SEND){
	echo 'Unsuccessful  connection send.<br>Error';
};

for($it=0;$it<200;$it++){
	$recievedFromESP = $client->receive();
	$comment = str_replace(array("\r\n", "\r", "\n"), '',  strip_tags($recievedFromESP));

	$dataTime = date("Y-m-d H:i:s");
	if(!($recievedFromESP[0] == "{") || empty($recievedFromESP)){
		echo "\n",$it," - ",$comment," - Isn't checked! - ",$dataTime;
		continue;
	};

	$data = json_decode($recievedFromESP);

	$sensorS = $data->{'S'};
	$sensorG = $data->{'G'};
	$sensorH = $data->{'H'};
	$sensorT = $data->{'T'};

	if($sensorS >= 0  && $sensorG >=0){
		mysqli_query($MySQL_CONNECTION_SEND,"INSERT INTO new_sensor_bank VALUES ('$sensorS','$sensorG','$sensorH','$sensorT',NOW())");
		echo "\n",$it," - ",$comment," - ",$dataTime;
	} else {
		echo "Isn't written! - the parametr is corrupted ",$dataTime;
	};

};
mysqli_close($MySQL_CONNECTION_SEND);
$client->close();
?>
