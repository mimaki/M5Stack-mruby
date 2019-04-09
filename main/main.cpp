#include <M5Stack.h>

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

static mrb_value
run_app_mrb(mrb_state *mrb, const char *fname)
{
  mrb_value val = mrb_nil_value();
  size_t size;
  uint8_t *buf;

  /* open mrb file */
  fs::File file = SD.open(fname, FILE_READ);
  if (!file) {
    M5.lcd.printf("(%s) load error.\n", fname);
    return val;
  }

  /* load mrb file */
  size = file.size();
  buf = new uint8_t[size];
  file.read(buf, size);
  /* launch application */
  val = mrb_load_irep(mrb, buf);
  /* free buffer and close mrb file */
  delete [] buf;
  file.close();

  return val;
}

void mrubyTask(void *pvParameters)
{
  mrb_state *mrb = NULL;
  mrb_value val;

  /* Initialize M5Stack */
  M5.begin();

  mrb = mrb_open();
  if (!mrb) {
    M5.lcd.print("mrb_open() failed.\n");
    goto ERROR;
  }

  if (M5.BtnA.isReleased()) {
    val = run_app_mrb(mrb, APP_MRB);
    if (mrb->exc) {
      val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
      mrb->exc = 0;
    }
    else {
      M5.lcd.print(" => ");
      if (!mrb_string_p(val)) {
        val = mrb_obj_as_string(mrb, val);
      }
    }
    M5.lcd.printf("%s\n", mrb_str_to_cstr(mrb, val));
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
  xTaskCreatePinnedToCore(mrubyTask, "mrubyTask", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}
