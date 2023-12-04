import React from "react";
import { useParams } from "react-router-dom";
import axios from "axios";
import MyChart from "./common/myChart";
import { transformData } from "../utils/transformData";
import { useQuery } from "react-query";

const fetchData = async (id, ADDR) => {
  const response = await axios.get(`${ADDR}?id=${id}`);
  return response.data;
};

const Charts = ({ SERVER_API_ADDR }) => {
  const { id } = useParams();
  // Define a unique query key for each ID to cache data separately
  const queryKey = ["data", id];
  // Use React Query's useQuery hook with queryKey
  const { data, error, isLoading } = useQuery(
    queryKey,
    () => fetchData(id, SERVER_API_ADDR),
    {
      // Set staleTime to control when data is considered stale (e.g., re-fetch after 5 minutes)
      staleTime: 5 * 60 * 1000, // 5 minutes in milliseconds
    }
  );

  if (isLoading) {
    return <p>Loading data...</p>;
  }

  if (error) {
    return <p>Error: {error.message}</p>;
  }

  const { humid, temp } = data;
  return (
    <React.Fragment>
      <h1>showing {id}</h1>
      <table className="table">
        <thead>
          <tr>
            <th scope="col">Humidity</th>
            <th scope="col">Temperature</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>
              <MyChart data={transformData(humid)} />
            </td>
            <td>
              <MyChart data={transformData(temp)} />
            </td>
          </tr>
        </tbody>
      </table>
    </React.Fragment>
  );
};
export default Charts;
