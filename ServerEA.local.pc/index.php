<!DOCTYPE HTML>
<html>
<head>
  <meta charset="utf-8">

  <meta name="Access-Control-Allow-Origin" content="*" />
  <meta name="Access-Control-Allow-Headers" content="*" />

  <!------------------------Styles--------------------------------->
  <link   rel = "stylesheet" href = "/styles/fonts.css">
  <link   rel = "stylesheet" href = "/styles/style.css">
  <!------------------------Libraryies----------------------------->
  <script src = "/scripts/libraryies/jquery-3.5.0.min.js"></script>
  <script src = "/scripts/libraryies/Chart.min.js"></script>
  <!------------------------Charts--------------------------------->
  <script defer src = "/scripts/ChartsCreator.js"></script>

  <title>Local PC Server</title>

</head>
<body>
    <div class="StatusBlock">
      <div class = "StatusElement" id="WebSocketStatus">WS status: disconnected</div>
      <div class = "StatusElement" id="ArduinoStatus">Arduino: disconnected</div>
      <div class = "StatusElement" id="ServerIP">IP:0.0.0.0</div>
      <div class = "StatusElement" id="CurrentTime">0:0:0</div>
    </div>
  <div id = "SensorGraphs">
  	<div id = "SensorGraphsHeader"></div>
  	<canvas id="myChart"></canvas>
  </div>
  <div id = "SensorArea">
    <div class = "SensorPlot">
        <canvas id="ChartS"></canvas>
    </div>
    <div class = "SensorPlot">
      <canvas id="ChartG"></canvas>
    </div>
    <div class = "SensorPlot">
      <canvas id="ChartH"></canvas>
    </div>
    <div class = "SensorPlot">
      <canvas id="ChartT"></canvas>
    </div>
  </div>
  <div class = "StatisticBlock">
    <div class = "DiagnosticBlock">
      <div class = "CurrentDataValue">
          <div class = "Expectation">
            <p>Math expectation</p>
            <div id = "SoilSensorE">Soil:0</div>
            <div id = "GazSensorE">Gaz:0</div>
            <div id = "HumiditySensorE">Humidity:0</div>
            <div id = "TemperatureSensorE">Temperature:0</div>
          </div>
          <div class = "Deviation">
            <p>Deviation</p>
            <div id = "SoilSensorD">Soil:0</div>
            <div id = "GazSensorD">Gaz:0</div>
            <div id = "HumiditySensorD">Humidity:0</div>
            <div id = "TemperatureSensorD">Temperature:0</div>
          </div>
          <div class = "containerSensor">
            <p>Standart deviation</p>
            <div id = "SoilSensor">Soil:0</div>
            <div id = "GazSensor">Gaz:0</div>
            <div id = "HumiditySensor">Humidity:0</div>
            <div id = "TemperatureSensor">Temperature:0</div>
          </div>
      </div>
    </div>
    <div class = "histogramArea">
          <!--Гистограмма относительных частот-->
            <canvas id="frequencyRange"></canvas>
    </div>
    <div class = "histogramArea">
          <!--Полигон относительных частот-->
            <canvas id="PolygonRange"></canvas>
    </div>

  <div class = "buttonStileSensor">
  	<!-- <button onclick='relay_inverse()'>Turn Off/On power pin 2</button> -->
  </div>
  <div class = "buttonsArea">
  	<div class = "buttonStileSensor">
  		<button class = "buttonStyleSensorRefreshRealtime" type="button" onclick="switchStream()">Realtime refresh</button>
  		<button class = "buttonStyleSensorRefreshOneTime" type="button" onclick="refreshDataSend()">Refresh one time</button>
  		<button class = "buttonStyleSensorReboot" type="button" onclick="reboot()"></button>
  	</div>
  </div>
  <div class = "formPompTimer">
    <label>Seconds <input id="timeS"  type="number" min="1" max="60" value="1"></label>
    <label>Minutes <input id="timeM"  type="number" min="1" max="60" value="1"></label>
    <label>Hours <input id="timeH"  type="number" min="1" max="24" value="1"></label>
    <label>Days <input id="timeD"  type="number" min="1" max="31" value="1"></label>
    <label>Month <input id="timeMn" type="number" min="1" max="12" value="1"></label>
    <label>Years <input id="timeY"  type="number" min="20" max="40" value="20"></label>
    <label>Day of week <input id="timeWD" type="number" min="1" max="7" value="1"></label>
    <button id = "UpdateButton" type="button" class = "buttonS">Update time</button>
  </div>
  <div class = "formPompTimer">
    <label>Timer per minutes <input id="perM" type="number" min="1" max="60" value="1"></label>
    <label>Timer per hours <input id="perH" type="number" min="1" max="24" value="1"></label>
    <label>Timer per month <input id="perD" type="number" min="1" max="31" value="1"></label>
    <label>Duration <input id="dur"  type="number" min="500" value="200"></label>
    <button id = "TimerPompSetButton" type="button" class = "buttonS">Update pomp timer</button>
  </div>
  <div class = "LogGraphsBlock">
      <div class = "BodyGraphsLog">
        <div id = "TextLog"></div>
        <div class = "InputConsoleLog">
          <input id="InputConsoleLine" type="text" onkeypress="return ConsoleEventManager(event)">
        </div>
      </div>
  </div>
  <div class="FooterBlock">

  </div>

</div>
</body>
</html>
