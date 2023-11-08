export function transformData(array) {
  console.log("transoformData");
  return array.map((item) => {
    return { date: item[0], value: item[1] };
  });
}
