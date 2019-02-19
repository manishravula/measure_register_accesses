/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2018 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
#include <iostream>
#include <fstream>
#include "pin.H"


#define C_DEBUG 0

ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 rread_count = 0;
static UINT64 rwrite_count = 0;
static UINT64 _rread_count = 0;
static UINT64 _rwrite_count = 0;

VOID regread_count(VOID *ip) { 
    rread_count++;
#if C_DEBUG
	OutFile << ip << ": R:  " << rread_count << endl;
#endif
}

VOID regwrite_count(VOID *ip){
    rwrite_count++;
#if C_DEBUG
	OutFile << ip << ": W:  " << rwrite_count << endl;
#endif
}

VOID Instruction(INS ins, VOID *v){


    int idx=0;

    for(idx=0;idx<4;idx++){
       if(INS_RegR(ins,idx)) {
			INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)regread_count, IARG_INST_PTR, IARG_END);
#if C_DEBUG
            OutFile << INS_Disassemble(ins) <<  " -- (READ):" <<_rread_count<< " -- " << REG_StringShort(INS_RegR(ins,idx)) << " : " << idx << endl;
#endif
            _rread_count++;
        } 

       if(INS_RegW(ins,idx)){
			INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)regwrite_count, IARG_INST_PTR, IARG_END);
#if C_DEBUG
            OutFile << INS_Disassemble(ins) <<  " -- (WRITE):" <<_rwrite_count<< " -- " << REG_StringShort(INS_RegW(ins,idx)) << " : " << idx << endl;
#endif
           	_rwrite_count++;

       }
    }

	OutFile << endl;

}

    
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "regcount.txt", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Read Register Count (Analysis):" << rread_count << endl;
    OutFile << "Write Register Count (Analysis):" << rwrite_count << endl;
    OutFile << "Read Register Count (Instrumentation):" << _rread_count << endl;
    OutFile << "Write Register Count (Instrumentation):" << _rwrite_count << endl;
    OutFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);


    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
