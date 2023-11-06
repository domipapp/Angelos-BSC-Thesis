export function transformData(array) {
  console.log("transoformData");
  return array.map((item) => {
    return { date: item[1], value: item[0] };
  });
}
