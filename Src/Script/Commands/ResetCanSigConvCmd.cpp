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
#include "Message.h"
#include "EquationParser.h"
#include "ExecutionStack.h"
#include "ReadCanCfg.h"
#include "InterfaceToScript.h"
}
#include "Parser.h"
#include "BaseCmd.h"

DEFINE_CMD_CLASS(cResetCanSigConvCmd)


int cResetCanSigConvCmd::SyntaxCheck (cParser *par_Parser)
{
    UNUSED(par_Parser);
    return 0;
}

int cResetCanSigConvCmd::Execute (cParser *par_Parser, cExecutor *par_Executor)
{
    UNUSED(par_Executor);
    int Channel = -1;
    int Id = -1;
    char *SignalName = nullptr;

    if (par_Parser->GetParameterCounter () >= 1) {
        //Channel
        if (par_Parser->SolveEquationForParameter (0, &Channel, -1)) {
            par_Parser->Error (SCRIPT_PARSER_FATAL_ERROR, "equation '%s' contains error ! Perhaps a variable does not exist...", 
                               par_Parser->GetParameter (0));
            return -1;
        }
        if (par_Parser->GetParameterCounter () >= 2) {
            // Id
            if (par_Parser->SolveEquationForParameter (1, &Id, -1)) {
                par_Parser->Error (SCRIPT_PARSER_FATAL_ERROR, "equation '%s' contains error ! Perhaps a variable does not exist...", 
                                   par_Parser->GetParameter (1));
                return -1;
            }
            if (par_Parser->GetParameterCounter () >= 3) {
                SignalName = par_Parser->GetParameter (2);
            }
        }
    }
    if (SendAddOrRemoveReplaceCanSigConvReq (Channel, Id, SignalName, nullptr, 0)) {
        par_Parser->Error (SCRIPT_PARSER_FATAL_ERROR, "cannot reset CAN signal conversion! Perhaps a variable does not exist..."); 
        return -1;
    }
    return 0;
}

int cResetCanSigConvCmd::Wait (cParser *par_Parser, cExecutor *par_Executor, int Cycles)
{
    UNUSED(Cycles);
    MESSAGE_HEAD mhead;
    if (test_message (&mhead)) {
        if (mhead.mid == NEW_CANSERVER_SET_SIG_CONV) {
            int Ret;
            read_message (&mhead, reinterpret_cast<char*>(&Ret), sizeof (Ret));

            if (par_Parser->GenDebugFile ()) par_Parser->PrintDebugFile ("\t\t/* RESET_CAN_SIG_CONV() needed %d cycles */\n", par_Executor->GetWaitCyclesNeeded ());
            if (Ret != 0) {
                par_Parser->Error (SCRIPT_PARSER_FATAL_ERROR, "cannot reset conversion");
            }
            return 0;
        }
    }
    return 1;

}

static cResetCanSigConvCmd ResetCanSigConvCmd ("RESET_CAN_SIG_CONV",                        
                                               0, 
                                               3,  
                                               nullptr,
                                               FALSE, 
                                               TRUE,  
                                               1000,
                                               0,
                                               CMD_INSIDE_ATOMIC_NOT_ALLOWED);
