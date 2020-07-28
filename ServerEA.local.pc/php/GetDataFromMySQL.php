<?php
require ('/ParametersForMySQL.php');

$MySQL_CONNECTION_GET = mysqli_connect($MySQL_host, $MySQL_user, $MySQL_pass, $MySQL_db_name);
if (!$MySQL_CONNECTION_GET){
	echo '<div id="connectionStatus">Unsuccessful  connection get.<br>Error:' . mysqli_connect_errno() . '--Description: ' . mysqli_connect_error().'</div>';
};

$idSensor    = $_POST['idSensor'];
$valueSensor = $_POST['valueSensor'];

$Limit_SQL_data  = 0;
$Limit_SQL_data  = $_POST['valueLimit'];

if($Limit_SQL_data <= 0){
	$Limit_SQL_data = 20;
}

$MySQL_request = "SELECT soil_sensor, gaz_sensor, air_sensor, temp_sensor, date_time
                  FROM (SELECT *
                  FROM new_sensor_bank
                  ORDER BY date_time DESC
                  LIMIT $Limit_SQL_data) T
                  ORDER BY date_time";

$result = mysqli_query($MySQL_CONNECTION_GET,$MySQL_request);

$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

$result->close();
mysqli_close($MySQL_CONNECTION_GET);

print json_encode($data);
?>
