/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH                          *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2020  SEGGER Microcontroller GmbH                 *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       Current version number will be inserted here                 *
*       when shipment is built.                                      *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : Disassembly_RI5CY.js
Purpose     : Ozone disassembly javascript plugin for the RI5CY core
Literature  : [1] RI5CY: User Manual
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       init
*
*  Function Description
*    Called by Ozone when the script was loaded 
*    (i.e. when command "Project.SetDisassemblyPlugin" was executed).
*
*    Typical usage: executes one or multiple "Debug.enableOverrideInst"
*    commands which define the instructions whose default disassembly 
*    is to be overridden by this plugin.
*
*  Return Value
*     0:  OK
*    -1:  error
*/
function init() {
  return 0;
}

/*********************************************************************
*
*       printInstAsm
*
*  Function Description
*    Prints the assembly code of an instruction.
*
*  Function Parameters
*    Addr     instruction address (type: integer)
*    aInst    instruction data bytes (type: byte array)
*    Flags    basic information about the instruction required for analysis.
*
*  Return Value
*    undefined:   if the input instruction is not supported by this plugin or an error occurred
*    string:      assembly code string containing a single tab after the instruction mnemonic and a single tab before a possible trailing comment
*/ 
function printInstAsm(Addr, aInst, Flags) {
  
  if (aInst.length != 4) {
    return undefined; // the RI5CY ISA-extension has a fixed instruction length of 32 bit
  }
  var Encoding = (aInst[3] << 24) | (aInst[2] << 16) | (aInst[1] << 8) | aInst[0];
  //
  // Instruction bitfields
  //
  var Opcode   = (Encoding >> 0)  & 0x7F;
  var Rd       = (Encoding >> 7)  & 0x1F;
  var Rs1      = (Encoding >> 15) & 0x1F;
  var Rs2      = (Encoding >> 20) & 0x1F;
  var Funct3   = (Encoding >> 12) & 0x7;
  var Funct7   = (Encoding >> 25) & 0x7F;
  var Imm5     = (Encoding >> 20) & 0x1F;
  var Imm6     = (Encoding >> 20) & 0x3F;
  var Imm7     = (Encoding >> 25) & 0x7F;
  var Imm12    = (Encoding >> 20) & 0xFFF;
  var Ls3      = (Encoding >> 25) & 0x1F;
  var Ls2      = (Encoding >> 20) & 0x1F;
  var F2       = (Encoding >> 30) & 0x3;
  var L        = (Encoding >> 7)  & 0x1;
  var F        = (Encoding >> 26) & 0x1;
 
  var sOpcode  = "";
  var sInst    = "";
  var sSymbol  = "";

  switch (Opcode) {

  case 11: // Register-Immediate Loads with Post-Increment

    if (Funct3 == 7) { // Register-Register Loads with Post-Increment

      switch (Funct7) {
        case 0:   sOpcode = "P.LB";  break;
        case 8:   sOpcode = "P.LH";  break;
        case 16:  sOpcode = "P.LW";  break;
        case 32:  sOpcode = "P.LBU"; break;
        case 40:  sOpcode = "P.LHU"; break;
        default:  sOpcode = "";      break;
      }
      if (Funct3 != 7) {
        sOpcode = ""; // invalid encoding
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs2) + "(" + regName(Rs1) + "!)";
      }
      break;

    } else { // Register-Immediate Loads with Post-Increment

      switch (Funct3) {
        case 0:  sOpcode = "P.LB";  break;
        case 1:  sOpcode = "P.LH";  break;
        case 2:  sOpcode = "P.LW";  break;
        case 4:  sOpcode = "P.LBU"; break;
        case 5:  sOpcode = "P.LHU"; break;
        default: sOpcode = "";      break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + immName(Imm12) + "(" + regName(Rs1) + "!)";
      }
    }
    break;
  
  case 3: // Register-Register Loads

    switch (Funct7) {
      case 0:   sOpcode = "P.LB";  break;
      case 8:   sOpcode = "P.LH";  break;
      case 16:  sOpcode = "P.LW";  break;
      case 32:  sOpcode = "P.LBU"; break;
      case 40:  sOpcode = "P.LHU"; break;
      default:  sOpcode = "";      break;
    }
    if (Funct3 != 7) {
      sOpcode = ""; // invalid encoding
    }
    if (sOpcode.length) {
      sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs2) + "(" + regName(Rs1) + ")";
    }
    break;

  case 43: 

    if (Funct3 <= 2) { // Register-Immediate Stores with Post-Increment

      switch (Funct3) {
        case 0:  sOpcode = "P.SB";  break;
        case 1:  sOpcode = "P.SH";  break;
        case 2:  sOpcode = "P.SW";  break;
        default: sOpcode = "";      break;
      }
      Imm12 = (Imm7 << 5) | Imm5;

      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rs2) + ", " + immName(Imm12) + "(" + regName(Rs1) + "!)";
      }
    } else { // Register-Register Stores with Post-Increment

      switch (Funct3) {
        case 4:  sOpcode = "P.SB";  break;
        case 5:  sOpcode = "P.SH";  break;
        case 6:  sOpcode = "P.SW";  break;
        default: sOpcode = "";      break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rs2) + ", " + regName(Rd) + "(" + regName(Rs1) + "!)";
      }
    }
    break;

  case 35: // Register-Register Stores

    switch (Funct3) {
      case 4:  sOpcode = "P.SB";  break;
      case 5:  sOpcode = "P.SH";  break;
      case 6:  sOpcode = "P.SW";  break;
      default: sOpcode = "";      break;
    }
    if (Funct7 != 0) {
      sOpcode = ""; // invalid encoding
    }
    if (sOpcode.length) {
      sInst = sOpcode + "\t" + regName(Rs2) + ", " + regName(Rd) + "(" + regName(Rs1) + ")";
    }
    break;

  case 123: // Hardware Loops

    switch (Funct3) {
      case 0:  sInst = "LP.STARTI" + "\t" + L + ", " + immName(Imm12); break;
      case 1:  sInst = "LP.ENDI"   + "\t" + L + ", " + immName(Imm12); break;  
      case 2:  sInst = "LP.COUNT"  + "\t" + L + ", " + regName(Rs1);   break; 
      case 3:  sInst = "LP.COUNTI" + "\t" + L + ", " + immName(Imm12); break;
      case 4:  sInst = "LP.SETUP"  + "\t" + L + ", " + regName(Rs1)  + ", " + immName(Imm12); break; 
      case 5:  sInst = "LP.SETUPI" + "\t" + L + ", " + immName(Imm5) + ", " + immName(Rs1);   break;
      default: sInst = ""; break;   
    }
    if (((Encoding >> 8) & 0xF) != 0) {
      sInst = ""; // invalid encoding
    }
    break;

  case 51: //  Bit Manipulation Operations

    if (F2 == 3) { // Bit Manipulation, one register and two immediates

      switch (Funct3) {
        case 0:  sOpcode = "P.EXTRACT";  break;
        case 1:  sOpcode = "P.EXTRACTU"; break;
        case 2:  sOpcode = "P.INSERT";   break;
        case 3:  sOpcode = "P.BCLR";     break;
        case 4:  sOpcode = "P.BSET";     break;
        default: sOpcode = "";           break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + immName(Ls3) + ", " + immName(Ls2);
      }

    } else if (Funct7 == 64) { // Bit Manipulation, 2 registers

      switch (Funct3) {
        case 0:  sOpcode = "P.EXTRACTR";  break;
        case 1:  sOpcode = "P.EXTRACTUR"; break;
        case 2:  sOpcode = "P.INSERTR";   break;
        case 3:  sOpcode = "P.BCLRR";     break;
        case 4:  sOpcode = "P.BSETR";     break;
        default: sOpcode = "";            break;
      }
      if (Ls3 != 0) {
         sOpcode = ""; // invalid encoding
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      }
    
    } else if (Funct7 == 4) { // Bit Manipulation, ROR
      
      if (Funct3 == 5) {
        sInst = "P.ROR" + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      }

    } else if (Funct7 == 8) { // Bit Manipulation, two registers & General ALU
    
      switch (Funct3) {
        case 0:  sOpcode = "P.FF1";    break; // Bit Manipulation
        case 1:  sOpcode = "P.FL1";    break;
        case 2:  sOpcode = "P.CLB";    break;
        case 3:  sOpcode = "P.CNT";    break;
        case 4:  sOpcode = "P.EXTHS";  break; // General ALU 
        case 5:  sOpcode = "P.EXTHZ";  break;
        case 6:  sOpcode = "P.EXTBS";  break;
        case 7:  sOpcode = "P.EXTBZ";  break;
        default: sOpcode = "";         break;
      }
      if (Rs2 != 0) {
        sOpcode = ""; // invalid encoding
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1);
      }

    } else if ((Funct7 == 2) && (Funct3 == 0)) { // General ALU ops, ABS

      if (Rs2 == 0) {
        sInst = "P.ABS" + "\t" + regName(Rd) + ", " + regName(Rs1);   
      }

    } else if (Funct7 == 2) { // General ALU ops, two registers

      switch (Funct3) {
        case 2:  sOpcode = "P.SLET";  break;
        case 3:  sOpcode = "P.SLETU"; break;
        case 4:  sOpcode = "P.MIN";   break;
        case 5:  sOpcode = "P.MINU";  break;
        case 6:  sOpcode = "P.MAX";   break;
        case 7:  sOpcode = "P.MAXU";  break;
        default: sOpcode = "";        break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      }
    } else if (Funct7 == 10) { // General ALU ops, 1 register and 1 immediate

      switch (Funct3) {
        case 1:  sInst = "P.CLIP"   + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + immName(Ls2); break;
        case 2:  sInst = "P.CLIPU"  + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + immName(Ls2); break;
        case 3:  sInst = "P.CLIR"   + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2); break;
        case 4:  sInst = "P.CLIPUR" + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2); break;
        default: sInst = ""; break;
      }
    } else if (Funct7 == 33) { // Multiply Accumulate ops

      if (Funct3 == 0) {
        sInst = "P.MAC" + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      } else if (Funct3 == 1) {
        sInst = "P.MSU" + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      }
    }
    break;

  case 91: // General ALU ops
   
    if (((F2 == 0) || (F2 == 2)) && ((Funct3 == 2) || (Funct3 == 3) || (Funct3 == 6) || (Funct3 == 7))) { // General ALU Operations, two registers and 1 immediate

      switch ((F2 << 3) | Funct3) {
        case 2:  sOpcode = "P.ADDN";    break;
        case 18: sOpcode = "P.ADDUN";   break;
        case 6:  sOpcode = "P.ADDRN";   break;
        case 22: sOpcode = "P.ADDURN";  break;
        case 3:  sOpcode = "P.SUBN";    break;
        case 19: sOpcode = "P.SUBUN";   break;
        case 7:  sOpcode = "P.SUBRN";   break;
        case 23: sOpcode = "P.SUBURN";  break;
        default: sOpcode = "";          break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2) + ", " + immName(Ls3);
      }
    } else if (((F2 == 1) || (F2 == 3)) && ((Funct3 == 2) || (Funct3 == 3) || (Funct3 == 6) || (Funct3 == 7))) { // General ALU Operations, two registers
      
      switch ((F2 << 3) | Funct3) {
        case 10: sOpcode = "P.ADDNR";   break;
        case 26: sOpcode = "P.ADDUNR";  break;
        case 14: sOpcode = "P.ADDRNR";  break;
        case 30: sOpcode = "P.ADDURNR"; break;
        case 11: sOpcode = "P.SUBNR";   break;
        case 27: sOpcode = "P.SUBUNR";  break;
        case 15: sOpcode = "P.SUBRNR";  break;
        case 31: sOpcode = "P.SUBURNR"; break;
        default: sOpcode = "";          break;
      }
      if (Ls3 != 0) {
        sOpcode = ""; // invalid encoding
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      }
    } else if ((Ls3 == 0) && (Funct3 == 0)) { // Multiply Accumulate, 2 registers

      switch (F2) {
        case 0:  sOpcode = "P.MULU";    break;
        case 1:  sOpcode = "P.MULHHU";  break;
        case 2:  sOpcode = "P.MULS";    break;
        case 3:  sOpcode = "P.MULHHS";  break;
        default: sOpcode = "";          break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2);
      }

    } else { // Multiply Accumulate, 2 registers and 1 immediate

      switch ((F2 << 3) | Funct3) {
        case 16: sOpcode = "P.MULSN";     break; // 10000 
        case 24: sOpcode = "P.MULHHSN";   break; // 11000 
        case 20: sOpcode = "P.MULSRN";    break; // 10100 
        case 28: sOpcode = "P.MULHHSRN";  break; // 11100 
        case 0:  sOpcode = "P.MULUN";     break; // 00000 
        case 8:  sOpcode = "P.MULHHUN";   break; // 01000 
        case 4:  sOpcode = "P.MULURN";    break; // 00100 
        case 12: sOpcode = "P.MULHHURN";  break; // 01100 
        case 17: sOpcode = "P.MACSN";     break; // 10001 
        case 25: sOpcode = "P.MACHHSN";   break; // 11001 
        case 21: sOpcode = "P.MACSRN";    break; // 10101 
        case 29: sOpcode = "P.MACHHSRN";  break; // 11101 
        case 1:  sOpcode = "P.MACUN";     break; // 00001 
        case 9:  sOpcode = "P.MACHHUN";   break; // 01001 
        case 5:  sOpcode = "P.MACURN";    break; // 00101 
        case 13: sOpcode = "P.MACHHURN";  break; // 01101
        default: sOpcode = "";            break;
      }
      if (sOpcode.length) {
        sInst = sOpcode + "\t" + regName(Rd) + ", " + regName(Rs1) + ", " + regName(Rs2) + ", " + immName(Ls3);
      }
    }
    break;

  case 99: // Immediate Branching Operations

    var a   =  (Encoding >> 7)  & 1;
    var b   =  (Encoding >> 8)  & 0xF;
    var c   =  (Encoding >> 25) & 0x3F;
    var d   =  (Encoding >> 31) & 1;
    Imm12   =  (b | (c << 4) | (a << 11) | (d << 12)) << 1;

    sSymbol =  Debug.getSymbol(Addr + Imm12);

    if (Funct3 == 2) {
      sOpcode = "P.BEQIMM";
    } else if (Funct3 == 3) {
      sOpcode = "P.BNEQIMM";
    } else {
      sOpcode = "";
    }
    if (sOpcode.length) {
      sInst = sOpcode + "\t" + regName(Rs1) + ", " + Imm5 + ", " + Imm12;
      if (sSymbol != "") {
        sInst = sInst + "\t; " + sSymbol;
      }
    }
  } // end switch(Opcode)

  if (sInst.length) {
    return sInst;
  } else {
    return undefined;
  }
}

/*********************************************************************
*
*       getInstInfo
*
*  Function Description
*    Returns information about an instruction.
*
*    Used by Ozone to generate timeline stacks and call-graphs,
*    among other applications.
*
*  Function Parameters
*    Addr     instruction address (type: integer)
*    aInst    instruction data bytes (type: byte array)
*    Flags    basic information about the instruction required for analysis.
*
*  Return Value
*    undefined:   if the input instruction is not supported by this plugin or an error occurred
*    InstInfo:    an object corresponding to the following C structure:
*          
*    struct INST_INFO {
*      U32 Mode;                      // instruction execution mode (for ex. THUMB or ARM)
*      U32 Size;                      // instruction byte size
*      U64 AccessAddress;             // branch address or memory access address
*      int StackAdjust;               // Difference of SP before and after instruction execution
*      struct {  
*        U32 IsValid           : 1;   // all fields initialized                     
*        U32 IsControlTransfer : 1;   // Instruction possibly alters the PC (synchronously or asynchronously) (WFI, SVC, POP PC, LDR PC, ...)
*        U32 IsSoftIRQ         : 1;   // Instruction is a software interrupt request
*        U32 IsBranch          : 1;   // Instruction is a simple branch (B, JMP, ...)
*        U32 IsCall            : 1;   // Instruction is a function call (Branch with Link, BL, CALL, ...)
*        U32 IsReturn          : 1;   // Dedicated return instruction or return-style branch (e.g. POP PC)
*        U32 IsMemAccess       : 1;   // Instruction reads from or writes to memory
*        U32 IsFixedAddress    : 1;   // Branch or access address is fixed (absolute or PC-relative)
*        U32 IsBP              : 1;   // Instruction is a SW Breakpoint
*        U32 IsSemiHosting     : 1;   // Instruction could be a semihosting instruction (BKPT 0xAB or SVC 0xAB or SVC 0x123456)
*        U32 IsNOP             : 1;   // Instruction is a NOP
*        U32 IsConditional     : 1;   // Instruction is conditionally executed
*        U32 Condition         : 4;   // Condition if conditionally executed
*      } Flags;
*    }
*/
function getInstInfo(Addr, aInst, Flags) {

  if (aInst.length != 4) {
    return undefined; // the RI5CY ISA extension has a fixed instruction length of 32 bit
  }
  var Encoding  =  (aInst[3] << 24) | (aInst[2] << 16) | (aInst[1] << 8) | aInst[0];
  var Opcode    =  (Encoding >> 0) & 0x7F;

  var InstInfo           = new Object();
  InstInfo.Size          = 4;
  InstInfo.Mode          = 0;
  InstInfo.AccessAddress = 0;
  InstInfo.StackAdjust   = 0;
  InstInfo.Flags         = 0;

  switch (Opcode) {

  case 11:   // Register-Immediate Loads with Post-Increment

    InstInfo.IsValid     = 1;
    InstInfo.IsMemAccess = 1;
    break;
  
  case 3:   // Register-Register Loads

    InstInfo.IsValid     = 1;
    InstInfo.IsMemAccess = 1;
    break;

  case 43:  // Register Stores with Post-Increment 

    InstInfo.IsValid     = 1;
    InstInfo.IsMemAccess = 1;   
    break;

  case 35:  // Register-Register Stores

    InstInfo.IsValid     = 1;
    InstInfo.IsMemAccess = 1;
    break;

  case 123: // Hardware Loops

    InstInfo.IsValid = 1;
    break;

  case 51:  //  Bit Manipulation Operations

    InstInfo.IsValid = 1;
    break;

  case 91:  // General ALU ops
   
    InstInfo.IsValid = 1;
    break;

  case 99:  // Immediate Branching Operations

    var a      =  (Encoding >> 7)  & 1;
    var b      =  (Encoding >> 8)  & 0xF;
    var c      =  (Encoding >> 25) & 0x3F;
    var d      =  (Encoding >> 31) & 1;
    var Imm12  =  (b | (c << 4) | (a << 11) | (d << 12)) << 1;

    InstInfo.AccessAddr    = Addr + Imm12;
    InstInfo.IsValid       = 1;
    InstInfo.IsBranch      = 1;
    InstInfo.IsFixedAddr   = 1;
    InstInfo.IsConditional = 1;
    
  } // end switch(Opcode)

  if (InstInfo.Flags.IsValid) {
    return InstInfo;
  } else {
    return undefined;
  }
}

/*********************************************************************
*
*       regName
*
*  Function Description
*    Helper function. Returns the ABI name of a RISCV-register.
*/
function regName(r) {

  var sReg;

  switch(r) {
    case 0:  sReg = "zero"; break; 
    case 1:  sReg = "ra";   break;
    case 2:  sReg = "sp";   break;
    case 3:  sReg = "gp";   break;
    case 4:  sReg = "tp";   break;
    case 5:  sReg = "t0";   break;
    case 6:  sReg = "t1";   break;
    case 7:  sReg = "t2";   break;
    case 8:  sReg = "fp";   break; 
    case 9:  sReg = "s1";   break;
    case 10: sReg = "a0";   break;
    case 11: sReg = "a1";   break;
    case 12: sReg = "a2";   break;
    case 13: sReg = "a3";   break;
    case 14: sReg = "a4";   break;
    case 15: sReg = "a5";   break;
    case 16: sReg = "a6";   break; 
    case 17: sReg = "a7";   break;
    case 18: sReg = "s2";   break;
    case 19: sReg = "s3";   break;
    case 20: sReg = "s4";   break;
    case 21: sReg = "s5";   break;
    case 22: sReg = "s6";   break;
    case 23: sReg = "s7";   break;
    case 24: sReg = "s8";   break; 
    case 25: sReg = "s9";   break;
    case 26: sReg = "s10";  break;
    case 27: sReg = "s11";  break;
    case 28: sReg = "t3";   break;
    case 29: sReg = "t4";   break;
    case 30: sReg = "t5";   break;
    case 31: sReg = "t6";   break;
    default: sReg = "?";    break;
  }
  return sReg;
}

/*********************************************************************
*
*       immName
*
*  Function Description
*    Helper function. Returns an unsigned immediate value as string
*/
function immName(Imm) {

  var sImm;

  if (Imm < 0x20) { // print small immediates in decimal
    sImm = Imm.toString(10);
  } else { // print large immediates in hexadecimal
    sImm = Imm.toString(16);
  }
  return sImm;
}

/*************************** End of file ****************************/