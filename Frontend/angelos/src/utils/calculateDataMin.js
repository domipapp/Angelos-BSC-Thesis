// Custom query function to calculate dataMin
export function calculateDataMin(data) {
  return Math.min(...data.map((item) => item.value));
}
