/*
 * Copyright 2023 ZF Friedrichshafen AG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "Platform.h"
#include <stdio.h>

extern "C" {
#include "Config.h"
#include "MyMemory.h"
#include "ConfigurablePrefix.h"
#include "Message.h"
#include "EquationParser.h"
}

#include "Parser.h"
#include "BaseCmd.h"
#include "Script.h"


DEFINE_CMD_CLASS(cStopPlayerCmd)


int cStopPlayerCmd::SyntaxCheck (cParser *par_Parser)
{
    UNUSED(par_Parser);
    return 0;
}

int cStopPlayerCmd::Execute (cParser *par_Parser, cExecutor *par_Executor)
{
    UNUSED(par_Executor);
    if (ReadVariableFromBlackboard (GetConfigurablePrefix(CONFIGURABLE_PREFIX_TYPE_STIMULUS_PLAYER)) > 0.0) {
        par_Parser->SendMessageToProcess (nullptr, 0, PN_STIMULI_PLAYER, HDPLAY_STOP_MESSAGE);
    }
    return 0;
}

int cStopPlayerCmd::Wait (cParser *par_Parser, cExecutor *par_Executor, int Cycles)
{
    UNUSED(par_Parser);
    UNUSED(par_Executor);
    UNUSED(Cycles);
    return 0;
}

static cStopPlayerCmd StopPlayerCmd ("STOP_PLAYER",                        
                                     0, 
                                     0,  
                                     nullptr,
                                     FALSE, 
                                     FALSE,  
                                     0,
                                     0,
                                     CMD_INSIDE_ATOMIC_ALLOWED);
