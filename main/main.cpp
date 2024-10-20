#include <M5Stack.h>
#include <esp_heap_caps.h>

#include <stdio.h>
#include <stdint.h>
#include "mruby.h"
#include "mruby/dump.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

/* Target application */
#define APP_MRB "/autorun.mrb"

extern "C" int mirb(mrb_state*);
extern "C" void* mrb_esp32_heap_allocf(mrb_state*, void*, size_t, void*);

static mrb_value
run_app_mrb(mrb_state *mrb, const char *fname)
{
  mrb_value val = mrb_nil_value();
  size_t size;
  uint8_t *buf;

  /* open mrb file */
  fs::File file = SD.open(fname, FILE_READ);
  if (!file) {
    // M5.lcd.printf("(%s) load error.\n", fname);
    return val;
  }

  /* load mrb file */
  size = file.size();
  buf = new uint8_t[size];
  if (buf) {
    file.read(buf, size);
    /* launch application */
    val = mrb_load_irep(mrb, buf);
    /* free buffer and close mrb file */
    delete [] buf;
  }
  else {
    M5.lcd.printf("No memory!!");
  }
  file.close();

  return val;
}

#define PTRBUFSIZE  32
#define ALLOCSIZE   1024 * 10
void mrubyTask(void *pvParameters)
{
  mrb_state *mrb = NULL;
  mrb_value val;
  size_t free8bit=0, freedef;
  // void *ptrbuf[PTRBUFSIZE];
  // int i;

  /* Initialize M5Stack */
  M5.begin();

  // M5.lcd.printf("Stack addr: %p\n", &pvParameters);

  free8bit = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
  freedef = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
  // M5.lcd.printf("Max block size: 8bit:%08x, def:%08x\n", free8bit, freedef);

  // free8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  // freedef = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  // M5.lcd.printf("Free size #0: 8bit:%08x, def:%08x\n", free8bit, freedef);

  // mrb = mrb_open();
  mrb = mrb_open_allocf(mrb_esp32_heap_allocf, NULL);
  if (!mrb) {
    M5.lcd.print("mrb_open() failed.\n");
    goto ERROR;
  }
  // free8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  // freedef = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  // M5.lcd.printf("Free size #1: 8bit:%08x, def:%08x\n", free8bit, freedef);
  // for (i=0; i<PTRBUFSIZE; i++) {
  //   // ptrbuf[i] = malloc(ALLOCSIZE);
  //   ptrbuf[i] = mrb_default_allocf(mrb, NULL, ALLOCSIZE, NULL);
  //   if (ptrbuf[i] == 0) {
  //     M5.lcd.printf("Alloc fail index=%d.\n", i);
  //     break;
  //   }
  // }
  // free8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  // freedef = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  // M5.lcd.printf("Free size #2: 8bit:%08x, def:%08x\n", free8bit, freedef);
  // for (i--; i>=0; i--) {
  //   // free(ptrbuf[i]);
  //   mrb_default_allocf(mrb, ptrbuf[i], 0, NULL);
  // }
  // free8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  // freedef = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  // M5.lcd.printf("Free size #3: 8bit:%08x, def:%08x\n", free8bit, freedef);

  if (M5.BtnA.isReleased()) {
    val = run_app_mrb(mrb, APP_MRB);
    if (mrb->exc) {
      val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
      mrb->exc = 0;
    }
    else {
      if (!mrb_nil_p(val)) {
        M5.lcd.print(" => ");
        if (!mrb_string_p(val)) {
          val = mrb_obj_as_string(mrb, val);
        }
      }
    }
    if (!mrb_nil_p(val)) {
      M5.lcd.printf("%s\n", mrb_str_to_cstr(mrb, val));
    }
  }

  /* Enter interactive mode */
  mirb(mrb);

  mrb_close(mrb);

ERROR:
  while (1) {
    micros();
  }
}

extern "C" void app_main()
{
  initArduino();
  xTaskCreatePinnedToCore(mrubyTask, "mrubyTask", 16384, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}
