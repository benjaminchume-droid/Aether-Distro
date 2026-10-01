#include <assert.h>
#include <stdio.h>
#include <aether/device_manager.h>
#include <aether/cpu.h>
#include <aether/gpu.h>
#include <aether/display.h>
#include <aether/input.h>
int main(void){
 assert(aether_device_manager_init()==AETHER_OK);
 assert(aether_device_scan()==AETHER_OK);
 assert(aether_cpu_scan()==AETHER_OK);
 assert(aether_gpu_scan()==AETHER_OK || aether_gpu_scan()==AETHER_ERR_IO);
 assert(aether_display_scan()==AETHER_OK || aether_display_scan()==AETHER_ERR_IO);
 assert(aether_input_scan()==AETHER_OK);
 printf("hardware discovery tests passed: devices=%zu cpus=%zu gpus=%zu displays=%zu inputs=%zu\n",
  aether_device_count(),aether_cpu_count(),aether_gpu_count(),aether_display_count(),aether_input_count());
 return 0;
}
