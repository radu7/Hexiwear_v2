/*
 * Sensor.c
 *
 *  Created on: 02.06.2017
 *      Author: Erich Styger
 */

#include "Platform.h"
#include "Sensor.h"
#include "FRTOS1.h"
#include "Bluetooth.h"
#if PL_CONFIG_HAS_ACCELEROMETER
  #include "FX1.h"
#endif
#include "HostComm.h"
#if PL_CONFIG_HAS_TSL2561
  #include "TSL1.h"
  #include "Vcc3V3B_EN.h" /* power for ambient, temperature/humidity */
#endif
#include "CLS1.h"
#if PL_CONFIG_HAS_HTU21D
  #include "HTU21d.h"
#endif
#include "LDO_EN.h" /* power for heart rate monitor */
#include "TmDt1.h"

static void SensorTask(void *param) {
  uint8_t res;

  vTaskDelay(pdMS_TO_TICKS(2000)); /* delay this task so other tasks can startup first */
#if 0
  /* enable power for heart rate monitor */
  LDO_EN_SetOutput();
  LDO_EN_ClrVal();
  vTaskDelay(pdMS_TO_TICKS(50));
#endif
#if PL_CONFIG_HAS_TSL2561
  CLS1_SendStr("Enabling TLS2561 sensor.\r\n", CLS1_GetStdio()->stdOut);
  /* 3V3B_EN:
   * HI-Z: Disabled
   * LOW: enabled (humidity, temperature, ambiLight
   */
  //Vcc3V3B_EN_SetInput(); /* disable */
  /* enable */
  Vcc3V3B_EN_SetOutput();
  Vcc3V3B_EN_ClrVal();
  vTaskDelay(pdMS_TO_TICKS(50));
  TSL1_Init();

  res = TSL1_Disable();
  if (res!=ERR_OK) {
    for(;;){}
  }
  vTaskDelay(pdMS_TO_TICKS(50));
  res = TSL1_Enable();
  if (res!=ERR_OK) {
    for(;;){}
  }

  res = TSL1_SetIntegrationTime(TSL2561_INTEGRATION_TIME_13MS);
  if (res!=ERR_OK) {
    for(;;){}
  }
  res = TSL1_SetGain(TSL2561_GAIN_16X);
  if (res!=ERR_OK) {
    for(;;){}
  }
#endif
#if PL_CONFIG_LOG_TO_RTT
  CLS1_SendStr("Logging Information to RTT\r\n", CLS1_GetStdio()->stdOut);
  /* write header */
  CLS1_SendStr("time", CLS1_GetStdio()->stdOut);
#if PL_CONFIG_HAS_ACCELEROMETER
  CLS1_SendStr(",ax,ay,az,mx,my,mz", CLS1_GetStdio()->stdOut);
#endif
#if PL_CONFIG_HAS_TSL2561
  CLS1_SendStr(",ambient", CLS1_GetStdio()->stdOut);
#endif
#if PL_CONFIG_HAS_HTU21D
  CLS1_SendStr(",T,H", CLS1_GetStdio()->stdOut);
#endif
  CLS1_SendStr("\r\n", CLS1_GetStdio()->stdOut);
#endif
  for(;;) {
    if (BLUETOOTH_GetCurrentAppMode()==GUI_CURRENT_APP_SENSOR_TAG || PL_CONFIG_LOG_TO_RTT) {
      int16_t x, y, z;
#if PL_CONFIG_LOG_TO_RTT
      {
        TIMEREC time;
        uint32_t ticks;

        TmDt1_GetTime(&time);
        TmDt1_TimeToTicks(&time, &ticks);
        CLS1_SendNum32u(ticks, CLS1_GetStdio()->stdOut);
      }
#endif
      /* accelerometer */
#if PL_CONFIG_HAS_ACCELEROMETER
      x = FX1_GetXmg();
      vTaskDelay(pdMS_TO_TICKS(10));
      y = FX1_GetYmg();
      vTaskDelay(pdMS_TO_TICKS(10));
      z = FX1_GetZmg();
      vTaskDelay(pdMS_TO_TICKS(10));
      /* transform milli-g values into centi-g (1234 mg are 123 which then are 1.23g */
#if PL_CONFIG_LOG_TO_RTT
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum16s(x, CLS1_GetStdio()->stdOut);
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum16s(y, CLS1_GetStdio()->stdOut);
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum16s(z, CLS1_GetStdio()->stdOut);
#endif
      x = x/10;
      y = y/10;
      z = z/10;
      if (BLUETOOTH_GetCurrentAppMode()==GUI_CURRENT_APP_SENSOR_TAG) {
        HostComm_SendAccel(x, y, z);
      }
      /* magnetometer */
      if (FX1_GetMagX(&x)==ERR_OK) {
        x = x/10;
      } else {
        x = 0;
      }
      vTaskDelay(pdMS_TO_TICKS(10));
      if (FX1_GetMagY(&y)==ERR_OK) {
        y = y/10;
      } else {
        y = 0;
      }
      vTaskDelay(pdMS_TO_TICKS(10));
      if (FX1_GetMagZ(&z)==ERR_OK) {
        z = z/10;
      } else {
        z = 0;
      }
      if (BLUETOOTH_GetCurrentAppMode()==GUI_CURRENT_APP_SENSOR_TAG) {
        HostComm_SendMag(x, y, z);
      }
#if PL_CONFIG_LOG_TO_RTT
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum16s(x, CLS1_GetStdio()->stdOut);
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum16s(y, CLS1_GetStdio()->stdOut);
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum16s(z, CLS1_GetStdio()->stdOut);
#endif
#endif
#if PL_CONFIG_HAS_TSL2561
      {
        uint16_t broad, ir;
        uint32_t lux;
        uint8_t ambient;

        TSL1_GetLuminosity(&broad, &ir);
        lux = TSL1_CalculateLux(broad, ir);
        TSL1_LuxToAmbientPercentage(lux, &ambient);
        if (BLUETOOTH_GetCurrentAppMode()==GUI_CURRENT_APP_SENSOR_TAG) {
          HostComm_SendAmbientLight(ambient);
        }
#if PL_CONFIG_LOG_TO_RTT
        CLS1_SendCh(',', CLS1_GetStdio()->stdOut);
        CLS1_SendNum8u(ambient, CLS1_GetStdio()->stdOut);
#endif
      }
#endif
#if PL_CONFIG_HAS_HTU21D
      {
        enum htu21_status status;
        uint8_t buf[16];
        float humidF, tempF;

        status = htu21_read_temperature_and_relative_humidity(&tempF, &humidF);
        if (status!=htu21_status_ok) {
          tempF = 0.0f;
          humidF = 0.0f;
        }
        buf[0] = '\0';
        UTIL1_strcatNumFloat(buf, sizeof(buf), tempF, 1);
        UTIL1_chcat(buf, sizeof(buf), (uint8_t*)",");
        UTIL1_strcatNumFloat(buf, sizeof(buf), humidF, 1);
        CLS1_SendStr(buf, CLS1_GetStdio()->stdOut);

      }
#endif
#if PL_CONFIG_LOG_TO_RTT
      CLS1_SendStr("\r\n", CLS1_GetStdio()->stdOut);
#endif
      vTaskDelay(pdMS_TO_TICKS(1000));
    } else {
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }
}

void SENSOR_Init(void) {
#if PL_CONFIG_HAS_TSL2561
  TSL1_Init();
#endif
  if (xTaskCreate(SensorTask, (uint8_t *)"Sensor", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
}
