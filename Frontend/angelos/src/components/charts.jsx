import React, { Component } from "react";
import { useParams } from "react-router-dom";
import axios from "axios";
import MyChart from "./common/myChart";
import { transformData } from "../utils/transformData";
import TestChart from "../test/testChart";
import { testTransformData } from "../test/testTransformData";
const Charts = () => {
  const { id } = useParams();
  return <InternalCharts id={id} />;
};

export default Charts;

class InternalCharts extends Component {
  state = {
    posts: {
      humid: [],
      temp: [],
    },
    id: this.props.id,
  };
  async componentDidMount() {
    try {
      const response = await axios.get(
        `http://192.168.1.71:5000/api/data?id=${this.state.id}`
      );
      const data = response.data;

      this.setState({ posts: data });
    } catch (error) {
      console.error("Error in componentDidMount:", error);
    }
  }
  async componentDidUpdate(prevProps) {
    if (prevProps.id !== this.props.id) {
      try {
        const response = await axios.get(
          `http://192.168.1.71:5000/api/data?id=${this.props.id}`
        );
        const data = response.data;
        this.setState({ posts: data });
        this.setState({ id: this.props.id });
      } catch (error) {
        console.error("Error in componentDidMount:", error);
      }
    }
  }

  render() {
    const { humid, temp } = this.state.posts;
    if (temp.length === 0) {
      // If temp is empty, return a loading message or some other UI indication.
      return <div>Loading...</div>; // or return null; or any loading UI you prefer
    }
    return (
      <React.Fragment>
        <h1>showing {this.state.id}</h1>
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
                <MyChart data={transformData(humid)} />
              </td>
              <td>
                <MyChart data={transformData(temp)} />
              </td>
            </tr>
            <tr>
              <td>
                <TestChart data={testTransformData(temp)} />
              </td>
            </tr>
          </tbody>
        </table>
      </React.Fragment>
    );
  }
}
