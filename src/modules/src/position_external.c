/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2012 BitCraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * position_external.c: Module to receive current position and yaw from external source
 */


// #include <string.h>
#include <errno.h>
// #include <stdint.h>
// #include <stdbool.h>

/* FreeRtos includes */
#include "FreeRTOS.h"
#include "task.h"
// #include "timers.h"
// #include "semphr.h"

// #include "config.h"
#include "crtp.h"
#include "position_external.h"
#include "debug.h"

//#include "console.h"
//#include "cfassert.h"

// Private types
struct data {
  float x;
  float y;
  float z;
  float yaw;
} __attribute__((packed));

// Global variables
static bool isInit = false;
static CRTPPacket p;
static uint64_t lastTime = 0;

//Private functions
static void positionExternalTask(void * prm);

void positionExternalInit(void)
{
  if(isInit) {
    return;
  }

  //Start the positionExternal task
  xTaskCreate(positionExternalTask, POSEXT_TASK_NAME,
              POSEXT_TASK_STACKSIZE, NULL, POSEXT_TASK_PRI, NULL);

  isInit = true;
  DEBUG_PRINT("posext. initialized.\n");
}

bool positionExternalTest(void)
{
  return isInit;
}

void positionExternalGetLastData(
  float* x,
  float* y,
  float* z,
  float* yaw,
  uint16_t* last_time_in_ms)
{
  struct data* d = (struct data*)&p.data[1];
  *x = d->x;
  *y = d->y;
  *z = d->z;
  *yaw = d->yaw;
  *last_time_in_ms = xTaskGetTickCount() - lastTime;
}

void positionExternalTask(void * prm)
{
  crtpInitTaskQueue(CRTP_PORT_POSEXT);

  while(1) {
    crtpReceivePacketBlock(CRTP_PORT_POSEXT, &p);
    lastTime = xTaskGetTickCount();
    DEBUG_PRINT("Recv. posext.\n");
  }
}