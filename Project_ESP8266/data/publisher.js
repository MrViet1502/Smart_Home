let mqttClient;
const MAX_HOURS = 24;
const MAX_DATA_POINTS =100;
var POINTS = 0;

window.addEventListener("load", (event) => {
  connectToBroker();
  const publishBtn = document.querySelector(".publish");
  publishBtn.addEventListener("click", function () {
    publishMessage();
  });
});

function connectToBroker() {
  const clientId = "client" + Math.random().toString(36).substring(7);

  // Change this to point to your MQTT broker
  const host = "ws://192.168.219.101:8080/mqtt";

  const options = {
    keepalive: 60,
    clientId: clientId,
    protocolId: "MQTT",
    protocolVersion: 4,
    clean: true,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000,
  };

  mqttClient = mqtt.connect(host, options);

  mqttClient.on("error", (err) => {
    console.log("Error: ", err);
    mqttClient.end();
  });

  mqttClient.on("reconnect", () => {
    console.log("Reconnecting...");
  });

  mqttClient.on('connect', function () {
    console.log('Đã kết nối tới MQTT broker');
  
    // Đăng ký sự kiện lắng nghe tin nhắn với chủ đề "led
    mqttClient.subscribe("led", { qos: 0 });
    mqttClient.subscribe("temperature", {qos:0});
    mqttClient.subscribe("humidity", {qos:0});

  });
  // Received
  mqttClient.on('message', function (topic, message) {
    let temp1;
    let humi1;
    if (topic === 'led') {
      console.log('Nhận được tin nhắn với chủ đề "led": ' + message.toString());
      // Chuyển giá trị tin nhắn thành số nguyên
      const value = parseInt(message.toString());
      // Kiểm tra giá trị của "led" và cập nhật trạng thái của checkbox
      if (value === 1) {
        myCheckbox.checked = true;
      } else {
        myCheckbox.checked = false;
      }
    }
    if(topic == 'temperature'){
      document.getElementById('numtemp').innerHTML = message.toString();
      temp1 = parseFloat(message);
      console.log('Temperature: ' + message.toString() + typeof(message));
      addTemperature(chart,temp1);
    }
    if(topic == 'humidity'){
      document.getElementById('numhumi').innerHTML = message.toString();
      humi1 = parseFloat(message);
      console.log('Humidity: ' + message.toString() + typeof{message});
      addHumidity(chart, humi1);
    }
  });
}

function publishMessage() {
  const messageInput = document.querySelector("#message");

  const topic = document.querySelector("#topic").value.trim();
  const message = messageInput.value.trim();

  console.log(`Sending Topic: ${topic}, Message: ${message}`);

  mqttClient.publish(topic, message, {
    qos: 0,
    retain: false,
  });
  messageInput.value = "";
}

// Get the checkbox element by its id
const myCheckbox = document.getElementById("myCheckbox");

// Add an event listener to the checkbox
myCheckbox.addEventListener("change", function() {
  if (myCheckbox.checked) {
    document.getElementById('state').innerHTML = "ON";
    console.log("1");
    mqttClient.publish("led", "1", {
      qos: 0,
      retain: false,
    });
  } else {
    console.log("0");
    document.getElementById('state').innerHTML = "OFF";
    mqttClient.publish("led", "0", {
      qos: 0,
      retain: false,
    });
  }
});

let currentHour = new Date().getHours();
let currentSec = new Date().getSeconds();

// Line Plot
const chart = new Chart(
  document.getElementById('myChart'),
  {
    type: 'line',
    options: {
      animation: {
        duration: 1000, // Thời gian hoạt hình mỗi lần cập nhật (1 giây)
      },
      plugins: {
        legend: {
          display: true,
        },
        tooltip: {
          enabled: true,
        },
      },
      scales: {
        x: {
          display: false,
          gridLines: {
            display: false, // Ẩn grid cho trục x
          },
        },
        y: {
          gridLines: {
            display: false, // Ẩn grid cho trục y
          },
        },
      },
    },
    data: {
      labels: null,
      datasets: [
        {
          label: 'Temperature',
          data: null,
          borderColor: 'rgba(75, 192, 192, 1)',
          borderWidth: 1,
          fill: false,
          tension: 0.4,
          pointRadius: 2,
          pointStyle: 'circle', // Set the point style to 'circle'
          pointBackgroundColor: 'rgba(75, 192, 192, 1)', // Set the point color
        },
        {
          label: 'Humidity',
          data: null,
          borderColor: 'rgba(192, 75, 192, 1)',
          borderWidth: 1,
          fill: false,
          tension: 0.4,
          pointRadius: 2,
          pointStyle: 'circle', // Set the point style to 'circle'
          pointBackgroundColor: 'rgba(192, 75, 192, 1)', // Set the point color
        },
      ],
    },
  }
);

function addTemperature(chart, newData) {
  const newHour = new Date().getHours();
  const newMinute = new Date().getMinutes();
  const newTimeLabel = `${newHour}:${newMinute}`;
  chart.data.labels.push(newTimeLabel);
  chart.data.datasets[0].data.push(newData);
  
  // Giới hạn số lượng điểm hiển thị trên biểu đồ theo thời gian
  const maxDataPointsInTimeRange = MAX_DATA_POINTS;
  if (chart.data.labels.length > maxDataPointsInTimeRange) {
    chart.data.labels.shift();
    chart.data.datasets[0].data.shift();
  }

  chart.update();
}

function addHumidity(chart, newData) {
  const newHour = new Date().getHours();
  const newMinute = new Date().getMinutes();
  const newTimeLabel = `${newHour}:${newMinute}`;
  chart.data.labels.push(newTimeLabel);
  chart.data.datasets[1].data.push(newData);

  // Giới hạn số lượng điểm hiển thị trên biểu đồ theo thời gian
  const maxDataPointsInTimeRange = MAX_DATA_POINTS;
  if (chart.data.labels.length > maxDataPointsInTimeRange) {
    chart.data.labels.shift();
    chart.data.datasets[1].data.shift();
  }

  chart.update();
}