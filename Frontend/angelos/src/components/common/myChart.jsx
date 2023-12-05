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

import { transformRangeSelectorData } from "../../utils/transformRangeSelectorData";

class MyChart extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      visualRange: { startValue: undefined, endValue: undefined },
      rangeSelectorData: transformRangeSelectorData(props.data, props.dataMin),
    };
    this.chartRef = React.createRef();
    this.falseVisibility = { visible: false };
  }

  componentDidUpdate(prevProps) {
    //refresh chart
    const { current } = this.chartRef;
    if (current && current.instance) {
      current.instance.render({
        force: true, // forces redrawing
        animate: false, // redraws the UI component without animation
      });
    }

    if (prevProps.data !== this.props.data) {
      this.setState({
        rangeSelectorData: transformRangeSelectorData(
          this.props.data,
          this.props.dataMin
        ),
      });
    }
  }

  render() {
    const { data } = this.props;
    const { rangeSelectorData } = this.state;
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
            <Tooltip
              enabled={true}
              customizeTooltip={(pointInfo) => {
                return {
                  text: `Date: ${pointInfo.argumentText}\nValue: ${pointInfo.valueText}`,
                };
              }}
            />
          </Chart>
          <RangeSelector
            dataSource={rangeSelectorData}
            onValueChanged={(e) => {
              this.setState({ visualRange: e.value });
            }}>
            <Size height={80} />
            <Margin left={10} />
            <Scale
              startValue={rangeSelectorData[0].date}
              endValue={rangeSelectorData[rangeSelectorData.length - 1].date}
              label={this.falseVisibility}
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

export default MyChart;
