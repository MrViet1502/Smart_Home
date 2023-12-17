document.addEventListener('DOMContentLoaded', function () {
  // Reference to the 'current-password' node in Firebase
  const currentPasswordInput = document.getElementById('current-password');
  const currentPasswordRef = firebase.database().ref('current-password');

  // Fetch the current password value from Firebase
  currentPasswordRef.once('value')
    .then(snapshot => {
      const currentPasswordValue = snapshot.val();

      // Update the current password input field on the web page
      console.log(currentPasswordValue);
      currentPasswordInput.value = currentPasswordValue;

      // Optionally, you can perform additional actions with the retrieved value here

    })
    .catch(error => {
      console.error('Error fetching current password from Firebase:', error);
      // Handle the error as needed
    });
});
const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");


const sideLinks = document.querySelectorAll('.sidebar .side-menu li a:not(.logout)');

sideLinks.forEach(item => {
  const li = item.parentElement;
  item.addEventListener('click', () => {
    sideLinks.forEach(i => {
      i.parentElement.classList.remove('active');
    })
    li.classList.add('active');
  })
});

const menuBar = document.querySelector('.content nav .bx.bx-menu');
const sideBar = document.querySelector('.sidebar');

menuBar.addEventListener('click', () => {
  sideBar.classList.toggle('close');
});

const searchBtn = document.querySelector('.content nav form .form-input button');
const searchBtnIcon = document.querySelector('.content nav form .form-input button .bx');
const searchForm = document.querySelector('.content nav form');

searchBtn.addEventListener('click', function (e) {
  if (window.innerWidth < 576) {
    e.preventDefault;
    searchForm.classList.toggle('show');
    if (searchForm.classList.contains('show')) {
      searchBtnIcon.classList.replace('bx-search', 'bx-x');
    } else {
      searchBtnIcon.classList.replace('bx-x', 'bx-search');
    }
  }
});

window.addEventListener('resize', () => {
  if (window.innerWidth < 768) {
    sideBar.classList.add('close');
  } else {
    sideBar.classList.remove('close');
  }
  if (window.innerWidth > 576) {
    searchBtnIcon.classList.replace('bx-x', 'bx-search');
    searchForm.classList.remove('show');
  }
});

const toggler = document.getElementById('theme-toggle');

toggler.addEventListener('change', function () {
  if (this.checked) {
    document.body.classList.add('dark');
  } else {
    document.body.classList.remove('dark');
  }
});

/**--------------------------------------- Chart ------------------------------------------ */
// Tạo dữ liệu cho trục x (labels)
const labels = [];
const temperatureData = [];
const humidityData = [];

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
    },
    data: {
      labels: labels,
      datasets: [
        {
          label: 'Temperature',
          data: temperatureData,
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
          data: humidityData,
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

// function updateChartData() {
//   // Lấy nhiệt độ và độ ẩm mới
//   const newTemperature = getNewTemperatureData(); // Thay thế hàm này bằng cách lấy dữ liệu nhiệt độ mới
//   const newHumidity = getNewHumidityData(); // Thay thế hàm này bằng cách lấy dữ liệu độ ẩm mới
//   const currentTime = new Date();

//   // Thêm nhiệt độ và độ ẩm mới vào mảng dữ liệu
//   temperatureData.push(newTemperature);
//   humidityData.push(newHumidity);

//   // Thêm nhãn thời gian mới vào mảng nhãn
//   labels.push(currentTime.getHours() + "H");

//   // Giới hạn mảng nhãn và dữ liệu chỉ trong 24 giờ
//   if (labels.length > 24) {
//     labels.shift(); // Xóa nhãn cũ nhất
//     temperatureData.shift(); // Xóa dữ liệu nhiệt độ cũ nhất
//     humidityData.shift(); // Xóa dữ liệu độ ẩm cũ nhất
//   }

//   // Cập nhật biểu đồ
//   chart.update();
// }

// // Gọi hàm `updateChartData()` ban đầu và sau mỗi 5 phút
// updateChartData();
// setInterval(updateChartData, 5 * 60 * 1000); // Cập nhật mỗi 5 phút (5 * 60 * 1000 ms)


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

/* ============================= MQTT ================================= */
let mqttClient;
const MAX_HOURS = 24;
const MAX_DATA_POINTS = 100;
var POINTS = 0;

window.addEventListener("load", (event) => {
  connectToBroker();
  document.getElementById("day").innerHTML = new Date().getDate();
  document.getElementById("month").innerHTML = new Date().getMonth() + 1;
  document.getElementById("year").innerHTML = new Date().getFullYear();
});

function connectToBroker() {
  const clientId = "client" + Date.now();
  const host = "wss:3f3c3cae23984b4cb5a8834ee9d5d0b6.s1.eu.hivemq.cloud:8884/mqtt";

  // Fetch CA certificate
  const options = {
    username: 'bkunoder',
    password: 'Thong182003',
    keepalive: 60,
    clientId: clientId,
    protocolId: "MQTT",
    protocolVersion: 4,
    clean: true,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000,
  };

  const mqttClient = mqtt.connect(host, options);
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
    mqttClient.subscribe("led-bed-room", { qos: 0 });
    mqttClient.subscribe("window-bed-room", { qos: 0 });
    mqttClient.subscribe("fan-bed-room", { qos: 0 });

    mqttClient.subscribe("led-living-room", { qos: 0 });
    mqttClient.subscribe("door-living-room", { qos: 0 });

    mqttClient.subscribe("led-kitchen", { qos: 0 });

    mqttClient.subscribe("temperature kitchen", { qos: 0 });
    mqttClient.subscribe("humidity kitchen", { qos: 0 });
    mqttClient.subscribe("temperature bedroom", { qos: 0 });
    mqttClient.subscribe("humidity bedroom", { qos: 0 });

  });
  // Received
  mqttClient.on('message', function (topic, message) {
    let temp1;
    let humi1;
    let temp2;
    let humi2;
    var safeDiv = document.getElementById('safeDiv');
    var warningDiv = document.getElementById('warningDiv');
    if (topic === 'led-bed-room') {
      console.log('Nhận được tin nhắn với chủ đề "led-bed-room": ' + message.toString());
      const value = message.toString();
      const checkbox = document.getElementById("checkbox-led-bed-room");
      if (value === "1") {
        checkbox.checked = true;
        document.getElementById("led-bed-room").innerHTML = "Open";
      } else {
        checkbox.checked = false;
        document.getElementById("led-bed-room").innerHTML = "Close";
      }
    }

    if (topic === 'window-bed-room') {
      console.log('Nhận được tin nhắn với chủ đề "window-bed-room": ' + message.toString());
      const value = message.toString();
      const checkbox = document.getElementById("checkbox-window-bed-room");
      if (value === "1") {
        checkbox.checked = true;
        document.getElementById("window-bed-room").innerHTML = "Open";
      } else {
        checkbox.checked = false;
        document.getElementById("window-bed-room").innerHTML = "Close";
      }
    }

    if (topic === 'fan-bed-room') {
      console.log('Nhận được tin nhắn với chủ đề "fan-bed-room": ' + message.toString());
      const value = message.toString();
      const checkbox = document.getElementById("checkbox-fan-bed-room");
      if (value === "1") {
        checkbox.checked = true;
        document.getElementById("fan-bed-room").innerHTML = "Open";
      } else {
        checkbox.checked = false;
        document.getElementById("fan-bed-room").innerHTML = "Close";
      }
    }

    if (topic == 'temperature kitchen') {
      document.getElementById('temp-kitchen').innerHTML = message.toString();
      temp1 = parseFloat(message);
      console.log('Temperature kitchen: ' + message.toString() + typeof (message));
      addTemperature(chart, temp1);
      if (temp1 > 60) {
        safeDiv.style.display = 'none';
        warningDiv.style.display = 'block';
        mqttClient.publish("led-kitchen", '2', {
          qos: 0,
          retain: false,
        });
      } else if (temp1 < 60) {
        safeDiv.style.display = 'block';
        warningDiv.style.display = 'none';
        mqttClient.publish("led-kitchen", '0', {
          qos: 0,
          retain: false,
        });

      }
      addTemperature(chart, temp1);
      
    }
    else if (topic == 'humidity kitchen') {
      document.getElementById('hum-kitchen').innerHTML = message.toString();
      humi1 = parseFloat(message);
      console.log('Humidity kitchen: ' + message.toString() + typeof { message });
      addHumidity(chart, humi1);
    }
  });
  function ChangeStatus(mycheckbox, state, status1, status2) {
    const myCheckbox = document.getElementById(mycheckbox);

    // Add an event listener to the checkbox
    myCheckbox.addEventListener("change", function () {
      if (myCheckbox.checked) {
        document.getElementById(state).innerHTML = status1;
        console.log(state + " - " + status1);
        mqttClient.publish(state, '1', {
          qos: 0,
          retain: false,
        });
      } else {
        document.getElementById(state).innerHTML = status2;
        console.log(state + " - " + status2);
        mqttClient.publish(state, '0', {
          qos: 0,
          retain: false,
        });
      }
    });
  }

  ChangeStatus("checkbox-led-bed-room", 'led-bed-room', 'Open', 'Close');
  ChangeStatus("checkbox-window-bed-room", 'window-bed-room', 'Open', 'Close');
  ChangeStatus("checkbox-fan-bed-room", 'fan-bed-room', 'Open', 'Close');

  ChangeStatus("checkbox-led-living-room", 'led-living-room', 'Open', 'Close');
  ChangeStatus("checkbox-door-living-room", 'door-living-room', 'Open', 'Close');

  ChangeStatus("checkbox-led-kitchen", 'led-kitchen', 'Open', 'Close');
  // ChangeStatus("checkbox-door-kitchen", 'door-kitchen', 'Open', 'Close');
  // ChangeStatus("checkbox-fan-kitchen", 'fan-kitchen', 'Open', 'Close');
  const newPasswordInput = document.getElementById('newPasswordInput');
  const currentPasswordInput = document.getElementById('current-password');
  const changePasswordButton = document.getElementById('changePasswordButton');

  changePasswordButton.addEventListener('click', function () {
    // Prompt for confirmation
    const confirmation = confirm('Are you sure you want to change the password?');

    // If user confirms, update the current password
    if (confirmation) {
      const newPassword = newPasswordInput.value;
      currentPasswordInput.value = newPassword;
      mqttClient.publish("newpassword", newPassword, {
        qos: 0,
        retain: false,
      });
      firebase.database().ref('current-password').set(newPassword);  
      alert('Password changed successfully!');
    }
    
  });const timeInput = document.getElementById('time-input');
  const okButton = document.getElementById('btn');
  let countdown;

  okButton.addEventListener('click', function () {
    // Get the time value from the input
    const timeInSeconds = parseInt(timeInput.value, 10);

    if (!isNaN(timeInSeconds) && timeInSeconds > 0) {
      // Disable the input and button during countdown
      timeInput.disabled = true;
      okButton.disabled = true;

      // Start the countdown
      startCountdown(timeInSeconds);
    } else {
      alert('Please enter a valid positive number for time.');
    }
  });

  function startCountdown(timeInSeconds) {
    // Set up a timeout to unlock the input after the specified time
    countdown = setTimeout(function () {
      timeInput.disabled = false;
      okButton.disabled = false;
      const ledBedRoomElement = document.getElementById('led-bed-room');
      if (ledBedRoomElement.textContent.trim() === 'Close') {
        console.log('The content is "Close".');
        mqttClient.publish("led-bed-room", "1", {
          qos: 0,
          retain: false,
        });
      } else {
        console.log('The content is not "Close".');
        mqttClient.publish("led-bed-room", "0", {
          qos: 0,
          retain: false,
        });
      } 
      alert('Countdown completed!');
    }, timeInSeconds * 1000);
  }



}

function addInputAndButton() {
  // Create input element
  const inputElement = document.createElement("input");
  inputElement.type = "text";
  inputElement.placeholder = "Enter text";

  // Create button element
  const addButton = document.createElement("button");
  addButton.textContent = "Add";
  addButton.addEventListener("click", () => {
    const textValue = inputElement.value;

    // Save the text value to Firebase (replace 'your-firebase-path' with the actual path)
    firebase.database().ref('remember').push({
      text: textValue
    });

    // Convert input to text
    const textElement = document.createElement("p");
    textElement.textContent = textValue;

    // Replace input with text
    inputElement.replaceWith(textElement);
    addButton.remove(); // Remove the "Add" button
  });

  // Append input and button to the container (replace 'orders' with the actual container class or ID)
  const ordersContainer = document.querySelector('.orders');
  ordersContainer.appendChild(inputElement);
  ordersContainer.appendChild(addButton);
}
