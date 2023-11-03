import React, { Component } from "react";
import axios from "axios";
import { Route, Routes } from "react-router-dom";
import Charts from "./components/charts";
import NavBar from "./components/navBar";
import TestChart from "./test/testChart";
import "./App.css";

class App extends Component {
  state = {
    sensor_id_array: [],
  };

  async componentDidMount() {
    try {
      const response1 = await axios.get(
        "http://192.168.1.71:5000/api/data?unique_ids"
      );
      const { sensor_id_array } = response1.data;
      this.setState({ sensor_id_array });
    } catch (error) {
      console.error("Error in componentDidMount:", error);
    }
  }

  render() {
    const { sensor_id_array } = this.state;
    return (
      <div>
        <NavBar ids={sensor_id_array} />
        <TestChart />
        <Routes>
          <Route path="/sensor/:id" Component={Charts} />
        </Routes>
      </div>
    );
  }
}

export default App;
