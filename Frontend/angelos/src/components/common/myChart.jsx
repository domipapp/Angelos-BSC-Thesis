import React, { Component } from "react";
import Paper from "@mui/material/Paper";
import {
  Chart,
  ArgumentAxis,
  ValueAxis,
  LineSeries,
  ZoomAndPan,
} from "@devexpress/dx-react-chart-material-ui";
import { scaleTime } from "d3-scale";
import { ArgumentScale } from "@devexpress/dx-react-chart";

const getMode = (zoom, pan) => {
  if (zoom && pan) {
    return "both";
  }
  if (zoom && !pan) {
    return "zoom";
  }
  if (!zoom && pan) {
    return "pan";
  }
  return "none";
};

const chartRootStyle = { marginRight: "20px" };

const ChartRoot = (props) => <Chart.Root {...props} style={chartRootStyle} />;

class MyChart extends Component {
  state = {
    data: [],
    zoomArgument: true,
    panArgument: true,
    zoomValue: true,
    panValue: true,
  };

  componentDidUpdate(prevProps) {
    if (prevProps.data !== this.props.data) {
    }
  }

  render() {
    const { zoomValue, panValue, zoomArgument, panArgument } = this.state;
    const chartData = this.props.data;
    return (
      <Paper>
        <Chart data={chartData} rootComponent={ChartRoot}>
          <ArgumentScale factory={scaleTime} />
          <ArgumentAxis />
          <ValueAxis />

          <LineSeries valueField="y" argumentField="x" />
          <ZoomAndPan
            interactionWithArguments={getMode(zoomArgument, panArgument)}
            interactionWithValues={getMode(zoomValue, panValue)}
          />
        </Chart>
      </Paper>
    );
  }
}

export default MyChart;
