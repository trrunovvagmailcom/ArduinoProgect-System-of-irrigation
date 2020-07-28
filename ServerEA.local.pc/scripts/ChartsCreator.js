window.chartColors = {
	red: 'rgb(255, 99, 132)',
	orange: 'rgb(255, 159, 64)',
	yellow: 'rgb(255, 205, 86)',
	green: 'rgb(12, 175, 80)',
	blue: 'rgb(54, 162, 235)',
	purple: 'rgb(153, 102, 255)',
	grey: 'rgb(201, 203, 207)'
};

var RemoteIP = prompt("IP", "192.168.0.102");
var serverConnectedToModule = false;


function ViewTime(){
	CurrentTime.innerHTML = GetDateTime(0);
};
setInterval(ViewTime, 1000);

$(document).ready(function() {
  document.getElementById('ServerIP').innerHTML ="IP:" + RemoteIP;
  WebSocketClient(RemoteIP);
  setInterval(UpdateTheParameters,10000);
});

let limitSensorsOut = 10;
function UpdateTheParameters(){
    $.ajax({
        url: "php/GetDataFromMySQL.php",
        type: "POST",
        data: {valueLimit:limitSensorsOut},
        success: function(responseMySQL) {
            console.log(responseMySQL);
            responseMySQL = JSON.parse(responseMySQL);
            let timeChart = [];
            let sensorS = [];
            let sensorG = [];
            let sensorH = [];
            let sensorT = [];

            let getDate = responseMySQL[(responseMySQL.length)-1].date_time;
            document.getElementById('SensorGraphsHeader').innerHTML = 'Plot <b>' + getDate.split(' ')[0] + '<b>';

            for(let i = 0; i < responseMySQL.length; i++) {
                let dateTime = responseMySQL[i].date_time;
                timeChart.push(dateTime.split(' ')[1]); //get time cut date

                sensorS.push(parseInt(responseMySQL[i].soil_sensor, 10));
                sensorG.push(parseInt(responseMySQL[i].gaz_sensor, 10));
                sensorH.push(parseInt(responseMySQL[i].air_sensor, 10));
                sensorT.push(parseInt(responseMySQL[i].temp_sensor, 10));

                // if(timeChart.length > limitSensorsOut){
                //   timeChart.shift();
                //   sensorS.shift();
                //   sensorG.shift();
                //   sensorH.shift();
                //   sensorT.shift();
                // };
            };
          //  let DateOfPlot = timeChart[0]. ;
            let getData = timeChart[0].split(' ')[0]; // 50ml
            //addData(MyChart, timeChart, 0, 1, 2, 3, sensorS, sensorG, sensorH, sensorT);
            addData(MyChart, timeChart, 0, sensorS);
            addData(MyChart, timeChart, 1, sensorG);
            addData(MyChart, timeChart, 2, sensorH);
            addData(MyChart, timeChart, 3, sensorT);

            addData(ChartS, timeChart, 0, sensorS);
      			addData(ChartG, timeChart, 0, sensorG);
        		addData(ChartH, timeChart, 0, sensorH);
      			addData(ChartT, timeChart, 0, sensorT);

            document.getElementById('SoilSensor').innerHTML = Math.sqrt(Deviation(GetMathExpectation('SoilSensorE',sensorS,0.1),sensorS,'SoilSensorD',0.1)).toFixed(2);
        		document.getElementById('GazSensor').innerHTML = Math.sqrt(Deviation(GetMathExpectation('GazSensorE',sensorG,0.1),sensorG,'GazSensorD',0.1)).toFixed(2);
        		document.getElementById('HumiditySensor').innerHTML = Math.sqrt(Deviation(GetMathExpectation('HumiditySensorE',sensorH,0.1),sensorH,'HumiditySensorD',0.1)).toFixed(2);
        		document.getElementById('TemperatureSensor').innerHTML = Math.sqrt(Deviation(GetMathExpectation('TemperatureSensorE',sensorT,0.1),sensorT,'TemperatureSensorD',0.1)).toFixed(2);
            histogramStatic(sensorG);
        }
    });
}
function addData(__chart, __timeInChart, __datasetIndex, __data) {
   __chart.data.labels = __timeInChart;
   __chart.data.datasets[__datasetIndex].data = __data;
   __chart.update();
};

var ctx = document.getElementById('myChart').getContext('2d');
var MyChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [0, 10, 20, 30, 40, 50, 60],
        datasets: [{
            label: 'Soil',
            borderColor: window.chartColors.green,
            borderWidth:2
        },{
            label: 'Gaz',
            borderColor: window.chartColors.purple,
            //borderDash: [20,5],
            borderWidth:2
        },{
            label: 'Humidity',
            borderColor: window.chartColors.red,
            //borderDash: [20,5],
            borderWidth:2
        },{
            label: 'Temperaturer',
            borderColor:'rgba(255, 9, 132, 1)',
            //borderDash: [20,5],
            borderWidth:2
        }]
    },options: ChartOption
});


var LogDiv = document.getElementById('TextLog');
var CurrentTime = document.getElementById('CurrentTime');

//-----------------------------------Chart Vareables----------------------------
var ClearSensor = [0,0];

var SensorTime = [];

//-----------------------------------Command List-------------------------------
var CommandServerList;
var CommandsNotRecieved = true;

function WebSocketClient(ip){
  window.socket = new WebSocket(`ws://${ip}:81`);

  let WSstatusDiv = document.getElementById('WebSocketStatus');
  socket.onmessage = function(event) {
    let WebSocketData = event.data;
    ResponsManager(WebSocketData);
  };
  socket.onopen = function() {
    serverConnectedToModule = true;
    socket.send(">5");
    WSstatusDiv.innerHTML = 'WS status: Connected';
  };
  socket.onclose  = function(event) {
    if (event.wasClean) {
      WSstatusDiv.innerHTML = 'The connection was close.';
    } else {
      WSstatusDiv.innerHTML = 'The connection was interrupted. Reconnection.';
      //let ServerIPWS = JSON.parse(GetHTTPCommandRespons("ip",RemoteIP));
      WebSocketClient(ServerIPWS.ServerIP);
    };
    WSstatusDiv.innerHTML += `Code:${event.code} <br>Reason: ${event.reason}`;
  };
  socket.onerror = function(error) {
    WSstatusDiv.innerHTML = `WS status: Error: ${error.message} Reconnection`;
  };
};
//------------------------------Respons Manager---------------------------------

function ResponsManager(__Respons){
  console.log(__Respons);

  if(__Respons.includes("data")){
    //GetCommandList(__Respons);
  }
//	};
  //if(__Respons.includes("ServerIP")){  ShowIP(__Respons)};

  LogDiv.innerHTML += `${GetDateTime(0)} - ${__Respons}<br>`;
  LogDiv.scrollTop  = LogDiv.scrollHeight;
};
// var globalMassSensor = [];
// function UpdateTheParameters(__WebSocketData){
//     SensorTime.push(GetDateTime(0));
//     __WebSocketData = JSON.parse(__WebSocketData);
// 		globalMassSensor.push(parseInt(__WebSocketData.S, 10));
//     sensorS.push(parseInt(__WebSocketData.S, 10));
//     sensorG.push(parseInt(__WebSocketData.G, 10));
//     sensorH.push(parseInt(__WebSocketData.H, 10));
//     sensorT.push(parseInt(__WebSocketData.T, 10));
//     if(SensorTime.length > 10){
//       SensorTime.shift();
//       sensorS.shift();
//       sensorG.shift();
//       sensorH.shift();
//       sensorT.shift();
//     };
//       addData(ChartS, SensorTime, 0, sensorS);
// 			addData(ChartG, SensorTime, 0, sensorG);
//   		addData(ChartH, SensorTime, 0, sensorH);
// 			addData(ChartT, SensorTime, 0, sensorT);
// };



function ShowIP(__WebSocketData){
    __WebSocketData = JSON.parse(__WebSocketData);
    document.getElementById('ServerIP').innerHTML ="IP:" + __WebSocketData.ServerIP;
	return __WebSocketData.ServerIP;
};
//------------------------------Request Manager---------------------------------
function ConsoleEventManager(e) {
   if (e.keyCode == 13) {
    //if(CommandsNotRecieved){CommandsNotRecieved = false;GetCommandList();}
    let ConsoleInput = document.getElementById('InputConsoleLine').value;
    let ShowInput = '<br><b>' + ConsoleInput + '</b><br>';
    LogDiv.innerHTML += ShowInput;

    if(ConsoleInput.includes(">")){
      ConsoleManager(ConsoleInput.slice(1));
      LogDiv.scrollTop = LogDiv.scrollHeight;
    };
  };
};

function GetCommandList(command_list){
    let JSONcommandList = JSON.parse(GetHTTPCommandRespons(command_list));
    JSONcommandList = Object.keys(JSONcommandList);
    CommandServerList = JSONcommandList;
};

function ConsoleManager(__Request){
  if(__Request == 'help'){
    let ShowCommands = '';
    for(let i = 0;i < CommandServerList.length;i++){
      ShowCommands += `${CommandServerList[i]}<br>`;
    };
    LogDiv.innerHTML += ShowCommands;
    return;
  };

 if(__Request.indexOf('{') > -1 && __Request.indexOf('}') > -1){
    let SetParemetersJson = __Request;
    socket.send(`>3${SetParemetersJson}`);
    LogDiv.innerHTML += SetParemetersJson;
    return;
  };

  for(let i = 0;i < CommandServerList.length;i++){
    if(CommandServerList[i]==__Request){
      //console.log(`${__Request} - command was found in commands list.`);
      LogDiv.innerHTML += `${__Request} - command is sended to ESP.\n`;
      socket.send(`>${i}`);
      return;
    };
  };
  LogDiv.innerHTML += `${__Request} - command is not found. ->help<br>`;
};



function TemplateChartSet(label,_color){
	var TemplateChartSet = [{
	          label: label,
	          borderColor: _color,
	          borderWidth:2,
			      //showLines:0
	      	}];
	return TemplateChartSet;
};
var HistogramStaticChartDataSet = {
			labels: [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8,0.9,1],
			datasets: [{
							type: 'line',
							label: 'Histogram',
							borderColor: window.chartColors.blue,
							borderWidth: 2,
							steppedLine: 'middle',
						},{
							type: 'line',
							label: 'Polygon',
							borderColor: window.chartColors.green,
							//borderColor: 'white',
							borderWidth: 2,
							fill: false,
						}]

		};
var ChartOption = {
			legend: {
				labels: {
					fontColor: '#05a20b'
				 }
		 },elements: {
				line: {
					tension: 0.5 // disables bezier curves
				}
		 },title: {
				display: true,
				//text: 'Value sensors by time.',
				fontColor: '#05a20b'
		 },scales: {
				yAxes: [{
					gridLines: {
						drawBorder: false,
						fontColor: '#05a20b',
						color: ['#2e3133','#f44336','#2e3133', '#7a7b37','#2e3133','#2e3133', 'green']
					}
				}]
			}
	}


var SoilChartPosition = document.getElementById('ChartS').getContext('2d');
var GazChartPosition = document.getElementById('ChartG').getContext('2d');
var HumidityChartPosition = document.getElementById('ChartH').getContext('2d');
var TemperatureChartPosition = document.getElementById('ChartT').getContext('2d');
var HistogramStaticChartPosition = document.getElementById('frequencyRange').getContext('2d');
var PolygonRangePosition = document.getElementById('PolygonRange').getContext('2d');


var ChartS = new Chart(SoilChartPosition, {
    type: 'line',
    data: {
      labels: [],
      datasets: TemplateChartSet('Soil',window.chartColors.green)
    },options: ChartOption
});
var ChartG = new Chart(GazChartPosition, {
    type: 'line',
    data: {
      labels: [],
      datasets: TemplateChartSet('Gaz',window.chartColors.grey)
    },options: ChartOption
});
var ChartH = new Chart(HumidityChartPosition, {
    type: 'line',
    data: {
      labels: [],
      datasets: TemplateChartSet('Humidity',window.chartColors.blue)
    },options: ChartOption
});
var ChartT = new Chart(TemperatureChartPosition, {
    type: 'line',
    data: {
      labels: [],
      datasets: TemplateChartSet('Temperature',window.chartColors.red)
    },options: ChartOption
});
var ChartHistogramStatic = new Chart(HistogramStaticChartPosition, {
    type: 'bar',
    data:HistogramStaticChartDataSet,
		options: ChartOption
});
var ChartMath = new Chart(PolygonRangePosition, {
	type: 'line',
	data: {
		labels: [],
		datasets: TemplateChartSet('accumulated relative frequency',window.chartColors.red)
	},options: ChartOption
});
//--------------------------DateTime Function-----------------------------------
function GetDateTime(__DataFormat){
  let DateNow = new Date();
  let timeF = `${formatTime(DateNow.getHours())}:${formatTime(DateNow.getMinutes())}:${formatTime(DateNow.getSeconds())}`;
  let dataF = `${formatTime(DateNow.getDate())}:${formatTime(DateNow.getMonth()+1)}:${DateNow.getFullYear() - 2000}`;
  switch(__DataFormat){
    case 0:__DataFormat = timeF;break;//Time
		case 1:__DataFormat = dataF;break;//Data
    case 2:__DataFormat = `${dataF} - ${timeF}`;
		case 3:__DataFormat = `{\"s\":${DateNow.getSeconds()},\"m\":${DateNow.getMinutes()},\"h\":${DateNow.getHours()},\"d\":${DateNow.getDate()},\"mn\":${DateNow.getMonth()},\"y\":${DateNow.getDate()}}`;break;//Data
  };
  return __DataFormat;
};

function formatTime(Ftime){
	if(Ftime < 10){
		Ftime = "0" + Ftime;
	}
	return Ftime;
};
//------------------------------------------------------------------------------
function GetHTTPCommandRespons(HttpCommand,ip){
  let ServerHTTPRespons = $.get({
    url:`http://${ip}/commands?command=${HttpCommand}`,
    async:false,
    data:{}
  }).responseText;
  return ServerHTTPRespons;
};


var PompTimerJson = '';
//var TimeJson = '';
function minId(_id){
    return document.getElementById(_id).value;
};

document.getElementById('UpdateButton').onclick = function CustomeUpdateTimeJson(){
    let TimeJson = `>2{\"s\":${minId('timeS')},\"m\":${minId('timeM')},\"h\":${minId('timeH')},\"d\":${minId('timeD')},\"mn\":${minId('timeMn')},\"y\":${minId('timeY')},\"wd\":${minId('timeWD')}}`;
    socket.send(TimeJson);
}
document.getElementById('TimerPompSetButton').onclick  = function CustomeTimerPompJson(){
    let PompTimerJson = `>3{\"m\":${minId('perM')},\"h\":${minId('perH')},\"d\":${minId('perD')},\"dur\":${minId('dur')}}`;
    socket.send(PompTimerJson);
}

function GetMathExpectation(_id,_MassOfValues,_Probability){// Мат. ожидание
	//let SRV = [];//Static random values Xi // массив дискретных случайных велечин
	//let Probability = 1/100;//Probability Pi //вероятность
	let ArrayLength = _MassOfValues.length;

	let MathExpectation = 0;
	for(let i=0;i<ArrayLength;i++){
		if(_Probability.length != 1 && _Probability.length == ArrayLength){
			MathExpectation += _MassOfValues[i] * _Probability[i];
		}else{
			MathExpectation += _MassOfValues[i] * _Probability;
		}
  }
	document.getElementById(_id).innerHTML = MathExpectation.toFixed(2);
	return MathExpectation.toFixed(2);
};

function Deviation(_MathExpectation, _MassOfValues, _id, _Probability){// Дисперсия
	let ArrayLength = _MassOfValues.length;
	let Deviation = 0;
	for(let i = 0; i < ArrayLength; i++ ){
		if(_Probability.length != 1 && _Probability.length == ArrayLength){
			Deviation += (_MassOfValues[i] * _MassOfValues[i]) * _Probability[i];
		}else{
			Deviation += (_MassOfValues[i] * _MassOfValues[i]) * _Probability;
		}
	}
 	Deviation = Deviation - (_MathExpectation * _MathExpectation);
	document.getElementById(_id).innerHTML = Deviation.toFixed(2);
	return Deviation.toFixed(2);
};


let ttt = [7.5,6.1,7,6,7.4,6.8,6.3,7.5,7,7.5,7.6,10.6,6,8.2,7.1,9.6,8.5,9.2,8,8,8.7,9.8,8.3,8.5,9.5,6.3,5.8,7.2,7.5,6.5];

var massValueHistogram = [];
for(let i=0;i<100;i++) massValueHistogram[i]=i;

function histogramStatic(Xi){
	let n = Xi.length; // объем выборочной совокупности
	let Xmin = Math.min(...Xi); // 1
	let Xmax = Math.max(...Xi);
	let k = Math.floor((1 + 3.3221 * Math.log10(n)).toFixed(2));//количество частичных интервалов по формуле Стерджеса
	let h = (Xmax - Xmin)/k; // длина частичного интервала h в равноинтервальной группировке

	let massStatistic = [];//интервальный вариационный ряд
	for(let i=0; i < k; i++){massStatistic[i] = 0;};

	let massIntervalValue = [];
	massIntervalValue[0] = Xmin;
	for(let i=0; i < k+1; i++){if(i>0)massIntervalValue[i] = (h * i) + Xmin;};

	let massIntervalMidpoint  = [];//середины интервалов
	for(let i=0; i < k; i++){
			massIntervalMidpoint[i] = parseFloat(((massIntervalValue[i] + massIntervalValue[i+1])/2).toFixed(1));
	};

	for(let i=0; i < n; i++){
		for(let j=0; j < k; j++){
			if(massIntervalValue[j] <= Xi[i] &&  Xi[i] <= massIntervalValue[j+1]){
				massStatistic[j]++;
				break;
			}
		}
	}

	let W = [];//относительные частоты
	let Wsum = 0;
	for(let i=0; i < k; i++){
		W[i] = parseFloat((massStatistic[i]/n).toFixed(2));
		Wsum +=	parseFloat(W[i]);
	};
	let Mn = [];//накопленные относительные частоты
	Mn[0] = W[0];
	for(let i=1; i < k; i ++){
		Mn[i] = Mn[i-1]+W[i];
	};
	console.log(Xi,massStatistic,massIntervalValue,massIntervalMidpoint);
	let Vn = 0;
	for(let i=0; i < massStatistic.length; i++){Vn += massStatistic[i];};
	console.log(Vn,W,Wsum);
	addData(ChartHistogramStatic,massIntervalValue, 0, W);
	addData(ChartHistogramStatic,massIntervalValue, 1, W);// Полигон распределения
	addData(ChartMath,massIntervalValue, 0, Mn);
	return massStatistic;
}
