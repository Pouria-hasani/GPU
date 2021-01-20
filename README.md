# GPU
**Monitor**
Monitor is using NVML library to profile :
1. device_fan_speed
2. device_temperature
3. device_power_usage
4. device_utilization_rates

To run the file first do the following:

```
mkdir build
cd build
cmake ..
```

Then just run monitor executable:
```
./monitor 
```

**nvml.c**
Collects statistics and sets the GPU and memory frequency. The output is a bit messy but it almost has the whole nvml library's functions
To compile the code run following:
```
nvcc -o nvml nvml.c -lnvidia-ml
```
**clkset.c**
Sets the GPU and memory's clocks
To compile run the following:
```
nvcc -o clkset clkset.c -lnvidia-ml
```
Then run ```sudo ./clkset```. The program asks you to enter the desired clock value in MHz. 
The entered clock value should be supported by the device.
To check the supported clock by device use supp_clk.c code

**supp_clk.c**

This code provides the supported memory and GPU clock. For each memory clock there are different available GPU clock.
To compile the code run the following:
```
nvcc -o supp_clk supp_clk.c -lnvidia-ml
```

**nvprof** 
To monitor matrixMul using nvprof use the following commad:
```
 sudo /usr/local/cuda-10.1/bin/nvprof --unified-memory-profiling off --cpu-profiling on --metrics all --events all --timeout 20  --csv --log-file %p ./matrixMul
```

To monitor mnist.py using nvprof use the following commad:
```
 sudo /usr/local/cuda-10.1/bin/nvprof --unified-memory-profiling off --cpu-profiling on --metrics all --events all --timeout 20  --csv --log-file %p python3 mnist.py
```
