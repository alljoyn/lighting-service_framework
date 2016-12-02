/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#include <Alarm.h>
#include <qcc/Debug.h>

using namespace lsf;

#define QCC_MODULE "LSF_ALARM"

Alarm::Alarm(AlarmListener* alarmListener) :
    isRunning(true),
    alarmListener(alarmListener),
    progressInTime(0),
    timeToTrack(0),
    alarmReset(false)
{
    QCC_DbgPrintf(("%s", __func__));
    Thread::Start();
}

Alarm::~Alarm()
{
    QCC_DbgPrintf(("%s", __func__));
}

void Alarm::Run()
{
    QCC_DbgPrintf(("%s", __func__));

    while (isRunning) {
        QCC_DbgPrintf(("%s", __func__));
        usleep(1000000);
        if (alarmReset) {
            QCC_DbgPrintf(("%s: Alarm Reloaded", __func__));
            progressInTime = 0;
            alarmReset = false;
        } else {
            if (timeToTrack) {
                progressInTime++;
                if (progressInTime == timeToTrack) {
                    QCC_DbgPrintf(("%s: Calling AlarmTriggered", __func__));
                    progressInTime = 0;
                    timeToTrack = 0;
                    alarmListener->AlarmTriggered();
                }
            }
        }
    }
}

void Alarm::Join()
{
    QCC_DbgPrintf(("%s", __func__));
    Thread::Join();
}

void Alarm::Stop()
{
    isRunning = false;
    progressInTime = 0;
    timeToTrack = 0;
    alarmReset = false;
}

void Alarm::SetAlarm(uint8_t timeInSecs)
{
    timeToTrack = timeInSecs;
    alarmReset = true;
}