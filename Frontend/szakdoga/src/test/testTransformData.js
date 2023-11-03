export function testTransformData(array) {
  return array
    .map((item) => {
      return { date: item[1], value: Math.round(item[0]) };
    })
    .slice(0, 5);
}
