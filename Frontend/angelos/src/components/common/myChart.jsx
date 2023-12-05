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

import { calculateDataMin } from "../../utils/calculateDataMin";
import { transformRangeSelectorData } from "../../utils/transformRangeSelectorData";

class App extends React.Component {
  constructor(props) {
    super(props);
    const rangeSelectorData = transformRangeSelectorData(
      props.data,
      props.dataMin
    );
    this.state = {
      visualRange: { startValue: 10, endValue: 880 },
      rangeSelectorData: rangeSelectorData,
      startValue: rangeSelectorData[0].date,
      endValue: rangeSelectorData[rangeSelectorData.length - 1].date,
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
      console.log("componentDidUpdate");
      const dataMin = calculateDataMin(this.props.data);
      const rangeSelectorData = transformRangeSelectorData(
        this.props.data,
        dataMin
      );
      const startValue = this.props.data[0].date;
      const endValue = this.props.data[this.props.data.length - 1].date;

      this.setState({
        dataMin: dataMin,
        rangeSelectorData: rangeSelectorData,
        startValue: startValue,
        endValue: endValue,
      });
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

  render() {
    const { data } = this.props;
    const { rangeSelectorData, startValue, endValue } = this.state;
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
