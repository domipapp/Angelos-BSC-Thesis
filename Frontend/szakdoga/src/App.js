import React, { Component } from "react";
import axios from "axios";
import MyChart from "./components/common/chart";
import NavBar from "./components/navBar";
import "./App.css";

class App extends Component {
  state = {
    posts: {
      humid: [],
      temp: [],
    },
    sensor_id_array: [],
  };

  async componentDidMount() {
    try {
      const response1 = await axios.get(
        "http://127.0.0.1:5000/api/data?unique_ids"
      );
      const { sensor_id_array } = response1.data;
      console.log(sensor_id_array);
      this.setState({ sensor_id_array });

      const response2 = await axios.get("http://127.0.0.1:5000/api/data?id=3");
      const data = response2.data;
      this.setState({ posts: data });
    } catch (error) {
      console.error("Error in componentDidMount:", error);
    }
  }

  transformData(array) {
    const resultArray = [];
    // Iterate through the humidArray and transform each pair
    for (let i = 0; i < array.length; i++) {
      const pair = array[i];
      const x = new Date(pair[1]); // Convert the date-time string to a Date object
      const y = pair[0]; // Use the first value as 'y'

      // Create an object with 'x' and 'y' properties and push it to the resultArray
      resultArray.push({ x, y });
    }
    return resultArray;
  }

  render() {
    const { humid, temp } = this.state.posts;
    const { sensor_id_array } = this.state;
    return (
      <div>
        <NavBar ids={sensor_id_array} />
        <table className="table">
          <thead>
            <tr>
              <th scope="col">Humidity</th>
              <th scope="col">Temperature</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>
                <MyChart data={this.transformData(humid)} />
              </td>
              <td>
                <MyChart data={this.transformData(temp)} />
              </td>
            </tr>
            <tr>
              <td>
                <MyChart data={this.transformData(humid)} />
              </td>
              <td>
                <MyChart data={this.transformData(temp)} />
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    );
  }
}

export default App;
