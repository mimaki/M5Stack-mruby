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

#define ENABLE_MIRB   /* mirb */

#ifdef ENABLE_MIRB
extern "C" int mirb(mrb_state*);
#else
static const uint8_t appbin[] = {
0x45,0x54,0x49,0x52,0x30,0x30,0x30,0x34,0x6c,0x9f,0x00,0x00,0x00,0x58,0x4d,0x41,
0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x3a,0x30,0x30,
0x30,0x30,0x00,0x00,0x00,0x32,0x00,0x01,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x02,
0x3d,0x00,0x80,0x00,0x4a,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x0d,0x48,
0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x6d,0x72,0x75,0x62,0x79,0x21,0x00,0x00,0x00,0x00,
0x45,0x4e,0x44,0x00,0x00,0x00,0x00,0x08
};
#endif

static mrb_value
run_app_mrb(mrb_state *mrb, const char *fname)
{
  mrb_value val = mrb_nil_value();
  size_t size;
  uint8_t *buf;

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
  // if (mrb->exc) {
  //   val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
  //   M5.lcd.printf("%s\n", mrb_str_to_cstr(mrb, val));
  // }
  /* free mrb buffer and close mrb file */
  delete [] buf;
  file.close();

  return val;
}

void mrubyTask(void *pvParameters)
{
  mrb_state *mrb = NULL;
// #ifndef ENABLE_MIRB
  mrb_value val;
// #endif

  M5.begin();

  mrb = mrb_open();
  if (!mrb) {
    M5.lcd.print("mrb_open() failed.\n");
    goto ERROR;
  }

  // if (file = SD.open(APP_MRB, FILE_READ)) {
  //   size_t size = file.size();
  //   M5.lcd.printf("file size = %d\n", size);
  //   uint8_t *buf = new uint8_t[size];
  //   file.read(buf, size);
  //   mrb_load_irep(mrb, buf);
  //   if (mrb->exc) {
  //     mrb_value val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
  //     M5.lcd.printf("%s\n", mrb_str_to_cstr(mrb, val));
  //   }
  //   delete [] buf;
  //   file.close();
  // }
  // else {
  //   M5.lcd.print(APP_MRB " not found.\n");
  // }
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

#ifdef ENABLE_MIRB
  mirb(mrb);
#else
  val = mrb_load_irep(mrb, appbin);
  if (mrb->exc) {
    mrb_print_error(mrb);
    mrb->exc = 0;
  }
  else {
    Serial.printf(" => ");
    mrb_p(mrb, val);
  }
#endif

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
