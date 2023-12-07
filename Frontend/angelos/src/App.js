import React, { Component } from "react";
import axios from "axios";
import { Route, Routes } from "react-router-dom";
import Charts from "./components/charts";
import NavBar from "./components/navBar";
import TextRotator from "./components/textRotator";
import "./App.css";

class App extends Component {
  state = {
    sensor_id_array: [],
    SERVER_API_ADDR: "http://152.66.158.66:5000/api/data",
  };

  async componentDidMount() {
    const currentURL = window.location.href;
    const urlObject = new URL(currentURL);

    if (urlObject.hostname !== "localhost") {
      this.setState(
        {
          SERVER_API_ADDR: `http://${urlObject.host}/api/data`,
        },
        () => {
          // Callback function to ensure the state is updated before making the API call
          this.fetchData();
        }
      );
    } else {
      // If it is localhost, use the default API address
      this.fetchData();
    }
  }

  async fetchData() {
    try {
      const {
        data: { sensor_id_array },
      } = await axios.get(`${this.state.SERVER_API_ADDR}?unique_ids`);
      this.setState({ sensor_id_array });
    } catch (error) {
      console.error("Error in componentDidMount:", error);
    }
  }

  render() {
    const { sensor_id_array, SERVER_API_ADDR } = this.state;
    return (
      <React.Fragment>
        <NavBar ids={sensor_id_array} />
        <Routes>
          <Route
            path="/sensor/:id"
            element={<Charts SERVER_API_ADDR={SERVER_API_ADDR} />}
          />
          <Route path="/" element={<TextRotator username={"User"} />} />
        </Routes>
      </React.Fragment>
    );
  }
}

export default App;
