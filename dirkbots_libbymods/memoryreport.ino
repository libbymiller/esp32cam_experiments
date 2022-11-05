#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();


void setup_report_memory() {
}

void loop_report_memory() {
  static unsigned t = millis(), lc = 0;
  lc++;
  if (millis() - t < 10 * 1000)
    return;
  t = millis();

  time_t now = time(NULL);
  Log.printf("Heap: Size: %.1f kB, Free: %.1f kB,, Min free: %.1f kB, Max Alloc: %.1f kB, cpu: %.1f˚C, looprate %.1f/sec -- %s",
             ESP.getHeapSize() / 1024.,
             ESP.getFreeHeap() / 1024.,
             ESP.getMinFreeHeap() / 1024.,
             ESP.getMaxAllocHeap() / 1024.,
             (temprature_sens_read() - 32) / 1.8,
             1000. * lc / millis(),
             ctime(&now)
            );
}

// Failing with cam_dma_config(306): frame buffer malloc failed 
// 11:03:39.609 -> Heap: Size: 291.6 kB, Free: 205.4 kB,, Min free: 187.4 kB, Max Alloc: 108.0 kB

// Working:
// 11:11:06.432 -> Heap: Size: 297.4 kB, Free: 272.9 kB, Min free: 267.5 kB, Max Alloc: 108.0 kB
// 11:07:50.612 -> Heap: Size: 293.3 kB, Free:  76.9 kB, Min free:  65.2 kB, Max Alloc:  45.0 kB, 
