Here is the details about our progress in developing the responsive adative regenerative braking system:

We faced challenges in calibrating the MPU6050 sensor, particularly with the Z-axis, and accurately identifying states like downhill, coasting, and cornering.
To address these, we smoothed the sensor data, implemented count-based checks, and added conditions to filter out false data and noise.
This helped us achieve reliable and responsive state detection for adaptive braking

We’ve successfully developed functional code for adaptive regenerative braking, including state detection and relay latching.
