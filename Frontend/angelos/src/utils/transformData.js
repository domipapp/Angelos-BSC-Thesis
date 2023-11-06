export function transformData(array) {
  return array.map((item) => {
    return { date: item[1], value: item[0] };
  });
}
