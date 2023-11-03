export function transformData(array) {
  const resultArray = [];
  // Iterate through the humidArray and transform each pair
  for (let i = 0; i < array.length; i++) {
    const pair = array[i];
    const x = new Date(pair[1]); // Convert the date-time string to a Date object
    const y = pair[0]; // Use the first value as 'y'

    // Create an object with 'x' and 'y' properties and push it to the resultArray
    resultArray.push({ x, y });
  }
  return resultArray;
}
