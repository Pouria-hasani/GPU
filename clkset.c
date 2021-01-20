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
//Set the clock
printf("\nEnter memory frequency in MHz\t");
scanf("%u",&memClockMHz);
printf("\nEnter GPU frequency in MHz\t");
scanf("\n%u",&graphicsClockMHz);

for(unsigned int index = 0;index<deviceCount;index++)
{
	nvmlReturn = nvmlDeviceSetApplicationsClocks (device,memClockMHz,graphicsClockMHz );
	if(nvmlReturn  == NVML_SUCCESS)
		printf("\nnew settings were successfully set");
	if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
		printf("\nthe library has not been successfully initialized");
	if(nvmlReturn  == NVML_ERROR_INVALID_ARGUMENT)
		printf("\nindex is invalid or device is NULL");
	if(nvmlReturn  == NVML_ERROR_NO_PERMISSION)
		printf("\nthe user doesn't have permission to talk to this device");
	if(nvmlReturn  == NVML_ERROR_NOT_SUPPORTED )
		printf("\nhe device doesn't support this feature");
	if(nvmlReturn  == NVML_ERROR_GPU_IS_LOST)
		printf("\nthe target GPU has fallen off the bus or is otherwise inaccessible");
	if(nvmlReturn  == NVML_ERROR_UNKNOWN)
		printf("\nunexpected error");
}
//Get driver version
nvmlSystemGetCudaDriverVersion (&cudaDriverVersion);
printf("\ndriver version: %d",cudaDriverVersion);
//NVML Shutdown
nvmlReturn =  nvmlShutdown ();
printf("\n\nNVML shutdown");
if(nvmlReturn  == NVML_SUCCESS)
	printf("\nNVML has been properly initialized\n");

if(nvmlReturn  == NVML_ERROR_UNINITIALIZED)
	printf("\nthe library has not been successfully initialized");

if(nvmlReturn  == NVML_ERROR_UNKNOWN )
	printf("\nu nexpected error");

return;
}