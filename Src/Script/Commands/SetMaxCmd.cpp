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
#include "Blackboard.h"
#include "InterfaceToScript.h"
}
#include "Parser.h"
#include "BaseCmd.h"

DEFINE_CMD_CLASS(cSetMaxCmd)


int cSetMaxCmd::SyntaxCheck (cParser *par_Parser)
{
    UNUSED(par_Parser);
    return 0;
}

int cSetMaxCmd::Execute (cParser *par_Parser, cExecutor *par_Executor)
{
    UNUSED(par_Executor);
    int Vid;
    double Value;
    if (par_Parser->SolveEquationForParameter (1, &Value, -1)) {
        par_Parser->Error (SCRIPT_PARSER_FATAL_ERROR, "equation '%s' contains error ! Perhaps a variable does not exist...", par_Parser->GetParameter (1));
        return -1;
    }
    if ((Vid = get_bbvarivid_by_name (par_Parser->GetParameter (0))) <= 0) {
        par_Parser->Error (SCRIPT_PARSER_FATAL_ERROR, "Variable '%s' does not exist", par_Parser->GetParameter (0));
        return -1;
    }
    set_bbvari_max (Vid, Value);
    return 0;
}

int cSetMaxCmd::Wait (cParser *par_Parser, cExecutor *par_Executor, int Cycles)
{
    UNUSED(par_Parser);
    UNUSED(par_Executor);
    UNUSED(Cycles);
    return 0;
}

static cSetMaxCmd SetMaxCmd ("SET_MAX",                        
                             2, 
                             2,  
                             nullptr,
                             FALSE, 
                             FALSE, 
                             0,
                             0,
                             CMD_INSIDE_ATOMIC_ALLOWED);

