#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <nvml.h>	

//Buffer size guaranteed to be large enough for nvmlDeviceGetInforomVersion and nvmlDeviceGetInforomImageVersion
#define NVML_DEVICE_INFOROM_VERSION_BUFFER_SIZE 16

//Buffer size guaranteed to be large enough for nvmlDeviceGetName
#define NVML_DEVICE_NAME_BUFFER_SIZE 64

//Buffer size guaranteed to be large enough for nvmlDeviceGetBoardPartNumber
#define NVML_DEVICE_PART_NUMBER_BUFFER_SIZE 80

//Buffer size guaranteed to be large enough for nvmlDeviceGetSerial
#define NVML_DEVICE_SERIAL_BUFFER_SIZE 30

//Buffer size guaranteed to be large enough for storing GPU identifiers.
#define NVML_DEVICE_UUID_BUFFER_SIZE 80

//Buffer size guaranteed to be large enough for nvmlDeviceGetUUID
#define NVML_DEVICE_UUID_V2_BUFFER_SIZE 96

//Buffer size guaranteed to be large enough for nvmlDeviceGetVbiosVersion
#define NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE 32

//Buffer size guaranteed to be large enough for nvmlSystemGetDriverVersion
#define NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE 80

//Buffer size guaranteed to be large enough for nvmlSystemGetNVMLVersion
#define NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE 80

//Macros for converting the CUDA driver version number to Major and Minor version numbers
//#define NVML_CUDA_DRIVER_VERSION_MAJOR ( v ) ((v)/1000)

//Define clock refrence
//Graphics clock domain.
#define NVML_CLOCK_GRAPHICS  0

//SM clock domain.
#define NVML_CLOCK_SM  1

//Memory clock domain.
#define NVML_CLOCK_MEM  2

//Video encoder/decoder clock domain.
#define NVML_CLOCK_VIDEO  3

//Count of clock types.
#define NVML_CLOCK_COUNT

//enum nvmlClockId_t
//Current actual clock value.
#define NVML_CLOCK_ID_CURRENT  0

//Target application clock.
#define NVML_CLOCK_ID_APP_CLOCK_TARGET  1

//Default application clock target.
#define NVML_CLOCK_ID_APP_CLOCK_DEFAULT  2

//OEM-defined maximum clock rate.
#define NVML_CLOCK_ID_CUSTOMER_BOOST_MAX  3

//Represents the queryable PCIe utilization counters
#define NVML_PCIE_UTIL_TX_BYTES  0
#define NVML_PCIE_UTIL_RX_BYTES  1

//Temperature sensor for the GPU die
#define NVML_TEMPERATURE_GPU  0

void main(int argc, char **argv )
{

// NVML Initialization	
nvmlReturn_t nvmlReturn;
nvmlReturn_t nvmlReturn_1;
nvmlReturn = nvmlInit_v2 ();

if(nvmlReturn  == NVML_SUCCESS)
	printf("\nNVML has been properly initialized");

if(nvmlReturn  == NVML_ERROR_DRIVER_NOT_LOADED)
	printf("\nNVIDIA driver is not running");

if(nvmlReturn  == NVML_ERROR_NO_PERMISSION)
	printf("\nNVML does not have permission to talk to the driver");

if(nvmlReturn  == NVML_ERROR_UNKNOWN)
	printf("\nunexpected error");




//NVML error report
/* const DECLDIR char* nvmlErrorstring;
nvmlErrorstring = nvmlErrorString ( nvmlReturn_t result ); */

//Retrieves the version of the CUDA driver.
char value[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE] = "430.40";

nvmlReturn = nvmlSystemGetDriverVersion(value, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE);

if(nvmlReturn  == NVML_SUCCESS)
	printf("\ncudaDriverVersion has been set");

if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
	printf("\ncudaDriverVersion is NULL");

//Acquire the handle for a particular device, based on its globally unique immutable UUID associated with each device.
unsigned int deviceCount;
nvmlReturn = nvmlDeviceGetCount_v2 (&deviceCount);
nvmlDevice_t device ;
nvmlClockType_t clockType;
unsigned int clockMHz;
unsigned int utilization;
unsigned int samplingPeriodU;
unsigned int* limit;
unsigned int speed;
nvmlMemory_t* memory =malloc(sizeof(nvmlMemory_t));
nvmlPcieUtilCounter_t *counter = malloc(sizeof(nvmlPcieUtilCounter_t));
unsigned int Value ;
nvmlPstates_t* pState = malloc(sizeof(nvmlPstates_t));
unsigned int power;
unsigned int Counter;
unsigned int count = 141;
int cudaDriverVersion;
unsigned int *GPU_clocksMHz = malloc(count*sizeof(unsigned int));
unsigned int* mem_spp_clk;
unsigned int  memoryClockMHz;
nvmlTemperatureSensors_t sensorType;
unsigned int* temp = malloc(sizeof(unsigned int));
nvmlUtilization_t* Utilization = malloc(sizeof(nvmlUtilization_t));
unsigned int  memClockMHz;
unsigned int  graphicsClockMHz ;
unsigned int infoCount;
nvmlProcessInfo_t * infos = malloc(sizeof(nvmlProcessInfo_t));
unsigned int  pid;
unsigned long long usedGpuMemory ;

//Supported memory and GPU clocks for device 0
nvmlDeviceGetHandleByIndex_v2 (0, &device );
printf("\n\n*******Device number is: %d *******",0);
Counter = 10;
nvmlReturn = nvmlDeviceGetSupportedMemoryClocks (device,&Counter ,&clockMHz );
if(nvmlReturn == NVML_SUCCESS)
{
	count =1000;
	mem_spp_clk = malloc(Counter*sizeof(unsigned int));
	nvmlReturn = nvmlDeviceGetSupportedMemoryClocks (device,&Counter ,mem_spp_clk );
	for(int i=0;i<Counter ;i++)
	{
		printf("\n\n\n*************** supported memory clocks");
		printf("\t%u****************",mem_spp_clk[i]); 
		//Retrieves the list of possible graphics clocks that can be used as an argument for nvmlDeviceSetApplicationsClocks
		memoryClockMHz = mem_spp_clk[i];
		nvmlReturn_1 = nvmlDeviceGetSupportedGraphicsClocks(device,memoryClockMHz,&count,GPU_clocksMHz);
		if(nvmlReturn_1 == NVML_SUCCESS)
			{
				unsigned int* gpu_spp_clk = malloc(count*sizeof(unsigned int));
				nvmlReturn_1 = nvmlDeviceGetSupportedGraphicsClocks(device,mem_spp_clk[i],&count,gpu_spp_clk );
				for(int j=0;j<count;j++)
				{
					if(nvmlReturn_1 == NVML_SUCCESS)
						printf("\n                supported gpu clock: %u",gpu_spp_clk[j]); 

					if(nvmlReturn_1  == NVML_ERROR_UNINITIALIZED)
						printf("\n the library has not been successfully initialized\n");
						if(nvmlReturn_1  == NVML_ERROR_NOT_FOUND)
						printf("\n the specified memoryClockMHz is not a supported frequency\n");
					if(nvmlReturn_1  == NVML_ERROR_INVALID_ARGUMENT)
						printf("\ndevice is invalid or clock is NULL\n");
					if(nvmlReturn_1  == NVML_ERROR_NOT_SUPPORTED)
						printf("\nthe device does not support this feature\n");
						if(nvmlReturn_1  == NVML_ERROR_INSUFFICIENT_SIZE)
						printf("\ncount is too small\n");
					if(nvmlReturn_1 == NVML_ERROR_GPU_IS_LOST)
						printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
					if(nvmlReturn_1 == NVML_ERROR_UNKNOWN)
						printf("\nunexpected error\n");
				}
			} 
	}
}


if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
	printf("\n the library has not been successfully initialized\n");
if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
	printf("\ndevice is invalid or clock is NULL\n");
if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
	printf("\nthe device does not support this feature\n");
	if(nvmlReturn  == NVML_ERROR_INSUFFICIENT_SIZE)
	printf("\ncount is too small\n");
if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
	printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
if(nvmlReturn  == NVML_ERROR_UNKNOWN)
	printf("\nunexpected error\n");



for(unsigned int index = 0;index<deviceCount;index++)
{
	nvmlReturn = nvmlDeviceGetHandleByIndex_v2 (index, &device );
	if(nvmlReturn  == NVML_SUCCESS)
	{
		printf("\n\n                      *********Device id is: %u***********",index);
		printf("\nRetrieves the current setting of a clock that applications will use");
		printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------");
		//Query Graphics clock domain

		clockType = NVML_CLOCK_GRAPHICS;
		nvmlReturn =  nvmlDeviceGetApplicationsClock (device,clockType,&clockMHz );
		if(nvmlReturn  == NVML_SUCCESS)
			printf("\n|Graphics clock                         |     %u",clockMHz);
		
		//Query SM clock domain.
		clockType = NVML_CLOCK_SM;
		nvmlReturn =  nvmlDeviceGetApplicationsClock ( device,clockType,&clockMHz );
		if(nvmlReturn  == NVML_SUCCESS)	
			printf("\n|SM clock                               |     %u",clockMHz);
		
		//Query Memory clock domain.
		clockType = NVML_CLOCK_MEM;
		nvmlReturn =  nvmlDeviceGetApplicationsClock ( device,clockType,&clockMHz );
		if(nvmlReturn  == NVML_SUCCESS)
			printf("\n|Memory clock                           |     %u",clockMHz);
		
		//Query Video encoder/decoder clock domain.
		clockType = NVML_CLOCK_VIDEO;
		nvmlReturn =  nvmlDeviceGetApplicationsClock ( device,clockType,&clockMHz );
		if(nvmlReturn  == NVML_SUCCESS)
			printf("\n|Video encoder/decoder clock            |     %u",clockMHz);
		
		 nvmlClockId_t clockId;
		for(int j=0;j<4;j++)
		{
			clockId = NVML_CLOCK_ID_CURRENT ;
			clockType = j;
			nvmlReturn = nvmlDeviceGetClock(device,clockType,clockId,&clockMHz );
			if(nvmlReturn  == NVML_SUCCESS)
			if(clockType == NVML_CLOCK_GRAPHICS)
				printf("\n|Current graphic clock                  |     %u",clockMHz);
			if(clockType == NVML_CLOCK_SM)
				printf("\n|Current SM clock                       |     %u",clockMHz);
			if(clockType == NVML_CLOCK_MEM)
				printf("\n|Current memory clock                   |     %u",clockMHz);
			if(clockType == NVML_CLOCK_VIDEO)
				printf("\n|Current video encoder/decoder clock    |     %u",clockMHz);
		}
	
	//Get information about processes with a compute context on a device

		infoCount = 1000;
		nvmlReturn = nvmlDeviceGetComputeRunningProcesses (device, &infoCount, NULL );
		infos = malloc(sizeof(nvmlProcessInfo_t)*(infoCount));
		nvmlReturn = nvmlDeviceGetComputeRunningProcesses (device, &infoCount, infos);
			printf("\n|Number of processes using the GPU      |     %u",infoCount);
		for(int i=0;i<infoCount;i++)
		{
			pid = infos[i].pid;
			usedGpuMemory = infos[i].usedGpuMemory;
			if(nvmlReturn == NVML_SUCCESS)
				printf("\n|usedGpuMemory by process with pid %u is |     %llu",pid,usedGpuMemory ); 
		}
//Retrieves the current utilization and sampling size in microseconds for the Decoder	
		nvmlReturn = nvmlDeviceGetDecoderUtilization (device,&utilization,&samplingPeriodU);

		if(nvmlReturn == NVML_SUCCESS)
			printf("\n|Decoder utilization is                 |     %u \n|Sampling period in US is               |     %u",utilization,samplingPeriodU); 

		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, utilization is NULL, or samplingPeriodUs is NULL\n");
		if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
			printf("\nthe device cannot report  current utilization and sampling size in microseconds for the Decoder\n");
		if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
			printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
		if(nvmlReturn  == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error\n");
		
		//Retrieves the current utilization and sampling size in microseconds for the Encoder
		nvmlReturn = nvmlDeviceGetEncoderUtilization(device,&utilization,&samplingPeriodU);

		if(nvmlReturn == NVML_SUCCESS)
			printf("\n|Encoder utilization is                 |     %u \n|sampling period in US is               |     %u",utilization,samplingPeriodU); 

		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, utilization is NULL, or samplingPeriodUs is NULL\n");
		if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
			printf("\nthe device cannot report  current utilization and sampling size in microseconds for the Decoder\n");
		if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
			printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
		if(nvmlReturn  == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error\n");
		
		
		//Retrieves the intended operating speed of the device's fan
		nvmlReturn = nvmlDeviceGetFanSpeed(device,&speed);

		if(nvmlReturn == NVML_SUCCESS)
			printf("\n|fan speed percentage                   |     %u",speed); 

		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, utilization is NULL, or samplingPeriodUs is NULL\n");
		if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
			printf("\nthe device cannot report  current utilization and sampling size in microseconds for the Decoder\n");
		if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
			printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
		if(nvmlReturn  == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error\n");
		
		//Memory information
		nvmlReturn = nvmlDeviceGetMemoryInfo(device,memory);
		if(nvmlReturn == NVML_SUCCESS)
			printf("\n|Unallocated FB memory                  |     %lluB\n|Total installed FB memory              |     %lluB\n|Allocated FB memory                    |     %lluB",memory->free,memory->total,memory->used); 
		
		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn == NVML_ERROR_NO_PERMISSION)
			printf("\nthe user doesn't have permission to perform this operation");
		if(nvmlReturn == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, either of infoCount or infos is NULL");
		if(nvmlReturn == NVML_ERROR_GPU_IS_LOST)
			printf("\nthe target GPU has fallen off the bus or is otherwise inaccessible");
		if(nvmlReturn == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error");
		
		//Retrieve PCIe utilization information
		for(int i=0;i<2;i++)
		{
			if(i==0)
			{
				*counter = NVML_PCIE_UTIL_TX_BYTES;
				nvmlReturn = nvmlDeviceGetPcieThroughput(device,*counter,&Value);
				if(nvmlReturn == NVML_SUCCESS)
					printf("\n|PCIe utilization TX bytes              |     %u",Value); 
			}
			if(i==1)
			{
				*counter = NVML_PCIE_UTIL_RX_BYTES;
				nvmlReturn = nvmlDeviceGetPcieThroughput(device,*counter,&Value);
				if(nvmlReturn == NVML_SUCCESS)
					printf("\n|PCIe utilization TX bytes              |     %u",Value); 
			}

			if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
				printf("\n the library has not been successfully initialized\n");
			if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
				printf("\ndevice is invalid, utilization is NULL, or samplingPeriodUs is NULL\n");
			if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
				printf("\nthe device cannot report  current utilization and sampling size in microseconds for the Decoder\n");
			if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
				printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
			if(nvmlReturn  == NVML_ERROR_UNKNOWN)
				printf("\nunexpected error\n");
		}
		
		
		//Performance state
		nvmlReturn = nvmlDeviceGetPerformanceState(device,pState);
		if(nvmlReturn == NVML_SUCCESS)
		{
			if(*pState == 32)
					printf("\n|Unknown performance state              |");
			else
			{
				for(int i=0;i<16;i++)
				{
					if(*pState == i)
					{
						printf("\n|Performance state value                |     %d",i);
						continue;
					}
				}
			}
		}

		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, utilization is NULL, or samplingPeriodUs is NULL\n");
		if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
			printf("\nthe device cannot report  current utilization and sampling size in microseconds for the Decoder\n");
		if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
			printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
		if(nvmlReturn  == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error\n");
		
		
	
		//Retrieves power usage for this GPU in milliwatts and its associated circuitry (e.g. memory)	
		nvmlReturn = nvmlDeviceGetPowerUsage(device,&power);
		if(nvmlReturn == NVML_SUCCESS)
			printf("\n|power usage                            |     %u mw",power); 
		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, utilization is NULL, or samplingPeriodUs is NULL\n");
		if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED)
			printf("\nthe device cannot report  current utilization and sampling size in microseconds for the Decoder\n");
		if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
			printf("\n the target GPU has fallen off the bus or is otherwise inaccessible\n");
		if(nvmlReturn  == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error\n"); 
		
		//Retrieves the current temperature readings for the device, in degrees C
		sensorType = NVML_TEMPERATURE_GPU;
		nvmlReturn = nvmlDeviceGetTemperature(device,sensorType,temp );
		if(nvmlReturn == NVML_SUCCESS)
			printf("\n|Devie tempreture is                    |     %uC",*temp); 
		
		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn == NVML_ERROR_NO_PERMISSION)
			printf("\nthe user doesn't have permission to perform this operation");
		if(nvmlReturn == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, either of infoCount or infos is NULL");
		if(nvmlReturn == NVML_ERROR_GPU_IS_LOST)
			printf("\nthe target GPU has fallen off the bus or is otherwise inaccessible");
		if(nvmlReturn == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error");
		
		//Device utilization rate
		nvmlReturn = nvmlDeviceGetUtilizationRates(device,Utilization );
		if(nvmlReturn == NVML_SUCCESS)
		{
			printf("\n|GPU Utilization                        |     %u",Utilization->gpu); 
			printf("\n|Memory Utilization                     |     %u",Utilization->memory); 
		}
		
		if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
			printf("\n the library has not been successfully initialized\n");
		if(nvmlReturn == NVML_ERROR_NO_PERMISSION)
			printf("\nthe user doesn't have permission to perform this operation");
		if(nvmlReturn == NVML_ERROR_INVALID_ARGUMENT)
			printf("\ndevice is invalid, either of infoCount or infos is NULL");
		if(nvmlReturn == NVML_ERROR_GPU_IS_LOST)
			printf("\nthe target GPU has fallen off the bus or is otherwise inaccessible");
		if(nvmlReturn == NVML_ERROR_UNKNOWN)
			printf("\nunexpected error");
		
		printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------");
	}
}
return;
}