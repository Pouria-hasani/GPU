# GPU
**NVIDIA_SMI**
Monitor is using Nvidia-smi tool to profile :
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

**nvprof** 
To monitor matrixMul using nvprof use the following commad:
```
 sudo /usr/local/cuda-10.1/bin/nvprof --unified-memory-profiling off --cpu-profiling on --metrics all --events all --timeout 20  --csv --log-file %p ./matrixMul
```

To monitor mnist.py using nvprof use the following commad:
```
 sudo /usr/local/cuda-10.1/bin/nvprof --unified-memory-profiling off --cpu-profiling on --metrics all --events all --timeout 20  --csv --log-file %p python3 mnist.py
```
