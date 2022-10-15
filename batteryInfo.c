/*
Copyright (c) 2022, Vlad Mesco

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// cc -o batteryInfo batteryInfo.c
#include <sys/types.h>
#include <machine/apmvar.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
/*
man 4 apm
     APM_IOC_GETPOWER
             (struct apm_power_info) Request the current power state.  The
             argument structure is as follows:

                   struct apm_power_info {
                           u_char battery_state;
                           u_char ac_state;
                           u_char battery_life;
                           u_char spare1;
                           u_int minutes_left;
                           u_int spare2[6];
                   };
            The following values are defined for battery_state:

             APM_BATT_HIGH
                     Battery has a high state of charge.

             APM_BATT_LOW
                     Battery has a low state of charge.


             APM_BATT_CRITICAL
                     Battery has a critical state of charge.

             APM_BATT_CHARGING
                     Battery is not high, low, or critical and is currently
                     charging.

             APM_BATT_UNKNOWN
                     Cannot read the current battery state.

             APM_BATTERY_ABSENT
                     No battery installed.

             The following values are defined for ac_state:

             APM_AC_OFF
                     External power not detected.

             APM_AC_ON
                     External power detected.

             APM_AC_BACKUP
:                     Backup power in use.

             APM_AC_UNKNOWN
                     External power state unknown.

             The battery_life value contains the estimated percentage of
             battery life available.  100% indicates a full charge.

             The minutes_left value contains the estimated number of minutes
             of battery life remaining.

*/

int main(int argc, char* argv[])
{
    struct apm_power_info pi;
    int d = open("/dev/apm", O_RDONLY);
    if(d < 0)
        err(1, "Failed to open /dev/apm");


    memset(&pi, 0, sizeof(struct apm_power_info));
    if(0 > ioctl(d, APM_IOC_GETPOWER, &pi))
        err(1, "Failed to ioctl APM_IOC_GETPOWER");

    printf("(%s) %d%%\n",
            (pi.ac_state == 1) ? "+" : "-",
            pi.battery_life);

    close(d);
}
