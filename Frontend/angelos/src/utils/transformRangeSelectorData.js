// Custom query function to transform data for rangeSelectorData
export function transformRangeSelectorData(data, dataMin) {
  console.log("transformRangeSelectorData");
  return data.map((item) => ({
    date: item.date,
    value: item.value - dataMin,
  }));
}
