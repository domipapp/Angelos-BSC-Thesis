// Custom query function to calculate dataMin
export function calculateDataMin(data) {
  console.log("calculateDataMin");
  return Math.min(...data.map((item) => item.value));
}
