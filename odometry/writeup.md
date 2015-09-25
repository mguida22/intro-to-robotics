# Odometry

 1. Inspect the line following code provided by ArcBotics. What happens during the “delay(100)” statement and where would you want to introduce your odometry code?
 2. Think about what would happen if calculating a position update would take longer than 100ms. Think about ways to measure the time the execution of a command takes and making sure every loop takes exactly 100ms. Hint: check out the millis library for Arduino.
 3. Use the caliper to measure the robot’s wheel radius and axle length. Use the millis library to measure how long it takes the robot to move 30cm and calculate its speed from there.
 4. Display the robot’s pose on its display. (Make sure this operation does not destroy your timing!). What do you expect the display to show when the robot arrives at the start line at the second (the third, the fourth) time? What actually happens?
 5. Use the robots sensors to identify the start line. How could you exploit this information to reduce your error?
