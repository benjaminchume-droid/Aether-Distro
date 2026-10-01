#include <assert.h>
#include <stdio.h>
#include <aether/device_manager.h>
#include <aether/cpu.h>
#include <aether/gpu.h>
#include <aether/display.h>
#include <aether/input.h>
#include <aether/audio.h>
#include <aether/camera.h>
#include <aether/storage.h>
#include <aether/network.h>
#include <aether/bluetooth.h>
#include <aether/thermal.h>
#include <aether/battery.h>
#include <aether/firmware.h>
int main(void){
 assert(aether_device_manager_init()==AETHER_OK);
 assert(aether_device_scan()==AETHER_OK);
 assert(aether_cpu_scan()==AETHER_OK);
 aether_status_t gpu=aether_gpu_scan(); assert(gpu==AETHER_OK || gpu==AETHER_ERR_IO);
 aether_status_t display=aether_display_scan(); assert(display==AETHER_OK || display==AETHER_ERR_IO);
 assert(aether_input_scan()==AETHER_OK); assert(aether_audio_scan()==AETHER_OK || aether_audio_scan()==AETHER_ERR_IO); assert(aether_camera_scan()==AETHER_OK || aether_camera_scan()==AETHER_ERR_IO); assert(aether_storage_scan()==AETHER_OK); assert(aether_network_scan()==AETHER_OK); { aether_status_t bt=aether_bluetooth_scan(); assert(bt==AETHER_OK || bt==AETHER_ERR_UNAVAILABLE); } assert(aether_thermal_scan()==AETHER_OK || aether_thermal_scan()==AETHER_ERR_IO); assert(aether_battery_scan()==AETHER_OK); assert(aether_firmware_scan()==AETHER_OK);
 printf("hardware discovery tests passed: devices=%zu cpus=%zu gpus=%zu displays=%zu inputs=%zu\n",
  aether_device_count(),aether_cpu_count(),aether_gpu_count(),aether_display_count(),aether_input_count());
 return 0;
}
