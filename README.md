# CS4348-OperatingSystems-Project02
This project simulates a bank using pthreads and POSIX semaphores for the teller and customers

# Directory Overview

1. include
   1. Houses all the include files for the programs. Includes a modified version of the log program from Project 1
2. Log
   1. Houses the log file created through the [Log Program]()
3. src
   1. Houses all the source .cpp files for the programs. The Driver Program main resides in the root of /src

# File List

1. **Driver Program**
   1. `main.cpp`: the only file for the driver program. Responsible for creating the Teller and Customer threads. Also houses the logic for the respective threads.
2. **Log Program**
   1. `logger.cpp`: the goal of this file is to define the logger class such that it is thread safe. The main logic of the log program resides in *print*. In this method, you write to the file if the queue has data to be read. Otherwise, you set the thread to periodically check the queue and allow the thread to sleep for a set duration.
   2. `log_stream.cpp`: this file simply houses the creation of the output string that will be sent to the file. It combines the time, state, and input to the stream.
   3. `logger.h`: this file defines the logger class as a singleton and also includes the *log_stream* link to the actual logger object.
   4. `log_stream.h`: this file defines the stream class. It also inherits ostream so that the logger can be utilized like cout with overloaded operators.
   5. `timeutil.h`: a simple util file that grabs and formats the time as per the logger requirements.
   6. `log_states.h`: a file that defines enums with the possible states that the logger must handle.

# Compile and Run

NOTE: All files are included for run in the zip. To skip compile, go to /src and run ./main

The program was run and tested on **Ubuntu 20.04 using g++ version 9.3**. The program should compile on any UNIX based system. In order to compile the program, `cd` to the `/src` directory. Run the following command:
>g++ main.cpp ../include/Log/log_stream.cpp -pthread -o main

If the compile is successful, an output file should be created within the same directory. To run the exe, run `./[exe]` (**assuming UNIX system**).

The output of the file will show within `Log/run.log` rather than standard output. This is simply done to format the output neatly utilizing existing development. When running, ensure that *run.log* is not open as the program can't write to the file when another process is in control. The file does overwrite and ensures unique run via random seed.

***Note: If there is an issue with segmentation faults at runtime, simply go to `Line 25 in main.cpp`. Reduce the max number of Customer Threads and compile once more. Recommended to reduce ~20 threads at a time. Mileage may vary based on hardware.***
