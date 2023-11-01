USE weather;
select * from measurements;
TRUNCATE TABLE measurements;
INSERT INTO measurements (sensor_id, temperature, humidity, time_of_measurement) 
VALUES (2, 21.22, 65.77, '2023-10-18 10:11:11');
SELECT temperature, time_of_measurement FROM measurements WHERE sensor_id = 1;

SELECT DISTINCT sensor_id FROM measurements as unique_sensor_id_array;


