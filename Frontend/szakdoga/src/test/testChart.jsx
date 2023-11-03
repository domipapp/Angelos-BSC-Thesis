import React from "react";
import Chart, {
  CommonSeriesSettings,
  Series,
  ArgumentAxis,
  ValueAxis,
  Title,
} from "devextreme-react/chart";

const dataa = [
  { date: "2023-10-19 20:39:13", value: 10 },
  { date: "2023-10-19 21:45:25", value: 15 },
  { date: "2023-10-21 22:30:50", value: 20 },
  { date: "2023-10-22 23:15:10", value: 25 },
  { date: "2023-10-23 00:00:00", value: 30 },
];

const data = [
  { date: "2023-10-30 20:13:51", value: 23 },
  { date: "2023-10-30 20:13:56", value: 23 },
  { date: "2023-10-30 20:14:01", value: 23 },
  { date: "2023-10-30 20:15:14", value: 24 },
  { date: "2023-10-30 20:15:44", value: 24 },
];

const TestChart = ({ data }) => {
  console.log(data);
  return (
    <Chart id="chart" dataSource={data} title="Date vs. Number">
      <CommonSeriesSettings argumentField="date" valueField="value" />
      <Series
        argumentField="date"
        valueField="value"
        name="Number"
        type="line"
      />
      <ArgumentAxis
        type="datetime"
        label={{
          format: "yyyy-MM-dd HH:mm:ss",
          customizeText: (arg) => {
            return new Date(arg.value);
          },
        }}
        title="Date"
      />
      <ValueAxis title="Number" />
      <Title text="Date vs. Number" />
    </Chart>
  );
};

export default TestChart;
