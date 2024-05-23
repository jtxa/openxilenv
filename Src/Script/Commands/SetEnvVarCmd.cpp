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
#include "MyMemory.h"
#include "EnvironmentVariables.h"
}
#include "Parser.h"
#include "BaseCmd.h"

DEFINE_CMD_CLASS(cSetEnvVarCmd)


int cSetEnvVarCmd::SyntaxCheck (cParser *par_Parser)
{
    UNUSED(par_Parser);
    return 0;
}

int cSetEnvVarCmd::Execute (cParser *par_Parser, cExecutor *par_Executor)
{
    UNUSED(par_Executor);
    if (!strcmp(GetName(), "SET_ENV_SYS_VAR")) {
        SetEnvironmentVariable(par_Parser->GetParameter (0), par_Parser->GetParameter (1));
    } else {
        SetUserEnvironmentVariable (par_Parser->GetParameter (0), par_Parser->GetParameter (1));
    }
    return 0;
}

int cSetEnvVarCmd::Wait (cParser *par_Parser, cExecutor *par_Executor, int Cycles)
{
    UNUSED(par_Parser);
    UNUSED(par_Executor);
    UNUSED(Cycles);
    return 0;
}

static cSetEnvVarCmd SetEnvVarCmd ("SET_ENV_VAR",                        
                                   2, 
                                   2,  
                                   nullptr,
                                   FALSE, 
                                   FALSE,  
                                   0,
                                   0,
                                   CMD_INSIDE_ATOMIC_ALLOWED);


static cSetEnvVarCmd SetEnvSysVarCmd ("SET_ENV_SYS_VAR",
                                   2,
                                   2,
                                   nullptr,
                                   FALSE,
                                   FALSE,
                                   0,
                                   0,
                                   CMD_INSIDE_ATOMIC_ALLOWED);
