// EnergyPlus, Copyright (c) 1996-2020, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <stdlib.h>

#include <EnergyPlus/api/datatransfer.h>
#include <EnergyPlus/api/runtime.h>

int outdoorDewPointActuator = -1;
int outdoorTempSensor = -1;
int outdoorDewPointSensor = -1;

void afterZoneTimeStepHandler()
{
    printf("STARTING A NEW TIME STEP\n");
    if (outdoorDewPointActuator == -1) {
        outdoorDewPointActuator = getActuatorHandle("Environment", "Weather Data", "Outdoor Dew Point");
        outdoorTempSensor = getVariableHandle("SITE OUTDOOR AIR DRYBULB TEMPERATURE", "ENVIRONMENT");
        outdoorDewPointSensor = getVariableHandle("SITE OUTDOOR AIR DEWPOINT TEMPERATURE", "ENVIRONMENT");
        if (outdoorDewPointActuator == -1 || outdoorTempSensor == -1 || outdoorDewPointSensor == -1) {
            exit(1);
        }
//        FILE *apiAvailFile;
//        apiAvailFile = fopen("/tmp/api_stuff_available.csv", "w");
//        const char * apiStuff = listAllAPIDataCSV();
//        fputs(apiStuff, apiAvailFile);
//        fclose(apiAvailFile);
    }
    setActuatorValue(outdoorDewPointActuator, -25.0);
    Real64 oa_temp = getVariableValue(outdoorTempSensor);
    printf("Reading outdoor temp via getVariable, value is: %8.4f \n", oa_temp);
    Real64 dp_temp = getVariableValue(outdoorDewPointSensor);
    printf("Actuated Dew Point temp value is: %8.4f \n", dp_temp);
}

int main(int argc, const char * argv[]) {
    callbackEndOfZoneTimeStepAfterZoneReporting(afterZoneTimeStepHandler);
    requestVariable("SITE OUTDOOR AIR DRYBULB TEMPERATURE", "ENVIRONMENT");
    requestVariable("SITE OUTDOOR AIR DEWPOINT TEMPERATURE", "ENVIRONMENT");
    energyplus(argc, argv);
    return 0;
}
