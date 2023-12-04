export function transformData(array) {
  return array.map((item) => {
    return { date: item[0], value: item[1] };
  });
}
