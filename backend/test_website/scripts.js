// Define the API URL and the query parameter
const API_URL = 'http://127.0.0.1:5000/api/data';
const id = 1;

// Create the URL with the query parameter
const apiUrlWithQuery = `${API_URL}?id=${id}`;

// Plot the graph
function plotGraph(data, elementId) {
    const labels = data.map(item => item[1]);
    const temperatures = data.map(item => item[0]);

    const ctx = document.getElementById(elementId).getContext('2d');

    new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [
                {
                    label: 'Temperature (°C)',
                    data: temperatures,
                    fill: false,
                    borderColor: 'rgb(75, 192, 192)',
                    tension: 0.1,
                },
            ],
        },
        options: {
            scales: {
                x: {
                    title: {
                        display: true,
                        text: 'Time',
                    },
                },
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Temperature (°C)',
                    },
                },
            },
        },
    });
}

// Make a GET request to the API with the query parameter
fetch(apiUrlWithQuery)
  .then(response => {
    if (!response.ok) {
      throw new Error('Network response was not ok');
    }
    return response.json();
  })
  .then(data => {
    // Handle the received data here
    plotGraph(data.temp, "temperatureChart");
    plotGraph(data.humid, "humidityChart");
  })
  .catch(error => {
    console.error('Error:', error);
    console.log('Response:', response);
  });
