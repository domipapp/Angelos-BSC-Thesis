import React from "react";

import Chart, {
  Series,
  Legend,
  CommonSeriesSettings,
  Point,
  ArgumentAxis,
  Tooltip,
  Label,
  ValueAxis,
} from "devextreme-react/chart";

import RangeSelector, {
  Size,
  Chart as ChartOptions,
  Margin,
  Scale,
  Behavior,
} from "devextreme-react/range-selector";

class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      visualRange: { startValue: 10, endValue: 880 },
    };

    this.updateVisualRange = this.updateVisualRange.bind(this);
    this.chartRef = React.createRef();
  }
  handleRefreshClick = () => {
    const { current } = this.chartRef;
    if (current && current.instance) {
      current.instance.render({
        force: true, // forces redrawing
        animate: false, // redraws the UI component without animation
      });
    }
  };
  componentDidUpdate(prevProps) {
    this.handleRefreshClick();
    if (prevProps.data !== this.props.data) {
      this.dataMin = Math.min(...this.props.data.map((item) => item.value));
      this.rangeSelectorData = this.props.data.map((item) => ({
        date: item.date,
        value: item.value - this.dataMin,
      }));
      this.startValue = this.props.data[0].date;
      this.endValue = this.props.data[this.props.data.length - 1].date;
    }
  }

  updateVisualRange(e) {
    this.setState({ visualRange: e.value });
  }
  customizeTooltip(pointInfo) {
    return {
      text: `Date: ${pointInfo.argumentText}\nValue: ${pointInfo.valueText}`,
    };
  }
  // Define label visibility here, outside of the render method
  rangeSelectorLabelConfig = { visible: false };
  dataMin = Math.min(...this.props.data.map((item) => item.value));
  rangeSelectorData = this.props.data.map((item) => ({
    date: item.date,
    value: item.value - this.dataMin,
  }));
  startValue = this.props.data[0].date;
  endValue = this.props.data[this.props.data.length - 1].date;

  render() {
    const { data } = this.props;
    const { rangeSelectorData, startValue, endValue } = this;
    return (
      <React.Fragment>
        <div className="container">
          <Chart
            ref={this.chartRef}
            id="zoomedChart"
            palette="Harmony Light"
            dataSource={data}>
            <Series argumentField="date" valueField="value" />
            <ArgumentAxis visualRange={this.state.visualRange}></ArgumentAxis>
            <Legend visible={false} />
            <CommonSeriesSettings>
              <Point size={4} />
            </CommonSeriesSettings>
            <ValueAxis>
              <Label format="decimal" />
            </ValueAxis>
            <Tooltip enabled={true} customizeTooltip={this.customizeTooltip} />
          </Chart>
          <RangeSelector
            dataSource={rangeSelectorData}
            onValueChanged={this.updateVisualRange}>
            <Size height={80} />
            <Margin left={10} />
            <Scale
              minorTickCount={1}
              startValue={startValue}
              endValue={endValue}
              label={this.rangeSelectorLabelConfig}
            />

            <ChartOptions palette="Harmony Light">
              <Behavior valueChangeMode="onHandleMove" />
              <Legend visible={false} />
              <Series argumentField="date" valueField="value" />
            </ChartOptions>
          </RangeSelector>
        </div>
      </React.Fragment>
    );
  }
}

export default App;
