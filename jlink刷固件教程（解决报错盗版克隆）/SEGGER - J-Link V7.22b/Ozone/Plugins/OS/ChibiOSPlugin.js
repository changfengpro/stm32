/*********************************************************************
*               (c) SEGGER Microcontroller GmbH                      *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : chibiOS.js
Purpose : RTOS Awareness Script for ChibiOS on Cortex-M devices.
          Display the system's threads in the ChibiOS Window,
          and enable selection and inspection of task states.
Date:     07. Jan 2019

Additional information:
  For ChibiOS 18.2 (included in ChibiStudio_Preview20)
  Tested with example projects 
  RT-STM32F722ZE-NUCLEO144 and RT-STM32L476RG-NUCLEO64, 
  configured for debug without optimization,
  with and without floating point use.
  
  Usage:
    (Requires Ozone V2.61b or newer.)
    - Create an Ozone project for the ChibiOS application.
    - Copy this file into Ozone project directory.
    - Add 
        Project.SetOSPlugin("$(ProjectDir)/chibiOS.js");
      at the end of OnProjectLoad().
    - Open the project in Ozone.
    - Select View -> ChibiOS.
    - Start debugging.
*/

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       GetTaskName()
*
*  Functions description:
*    Returns the task name of a task.
*/
function GetTaskName(pTask) {
  var TaskName;
  
  TaskName = Debug.evaluate("(char*)(*(thread_t*)" + pTask + ").name");
  if (TaskName == undefined) {
    TaskName = "undefined";
  } else if (TaskName == 0) {
    TaskName = "N/A";
  }
  
  return TaskName;
}

/*********************************************************************
*
*       GetTaskState()
*
*  Functions description:
*    Returns the description of a task state.
*/
function GetTaskState(State) {
  var sState;
  
  switch (State) {
  case 0:
    sState = "Ready";
    break;
  case 1:
    sState = "Currently running";
    break;
  case 2:
    sState = "Just created";
    break;
  case 3:
    sState = "Suspended";
    break;
  case 4:
    sState = "On an I/O queue";
    break;
  case 5:
    sState = "On a semaphore";
    break;
  case 6:
    sState = "On a mutex";
    break;
  case 7:
    sState = "On a cond.variable";
    break;
  case 8:
    sState = "Sleeping";
    break;
  case 9:
    sState = "Waiting for a thread";
    break;
  case 10:
    sState = "Waiting for one event";
    break;
  case 11:
    sState = "Waiting for several events";
    break;
  case 12:
    sState = "Sending a message, in queue";
    break;
  case 13:
    sState = "Sending a message, waiting answer";
    break;
  case 14:
    sState = "Waiting for a mesage";
    break;
  case 15:
    sState = "Thread terminated";
    break;
  default:
    sState = "unknown";
    break;
  }
  
  return sState;
}

/*********************************************************************
*
*       GetWorkingArea()
*
*  Functions description:
*    Returns the working area of a task.
*/
function GetWorkingArea(pTask) {
  var WABase;
  var StackEnd;
  var sWABase;
  
  
  WABase = Debug.evaluate("(*(thread_t*)" + pTask + ").wabase");
  if ((WABase == undefined) || (WABase == 0)) {
    sWABase = "N/A";
  } else {
    sWABase = "@0x" + WABase.toString(16).toUpperCase();
  }
  StackEnd = Debug.evaluate("(*(thread_t*)" + pTask + ").ctx.sp");
  if (StackEnd == undefined) {
    sStackEnd = "N/A";
  } else {
    sStackEnd = "0x" + StackEnd.toString(16).toUpperCase();
  }
  
  return sWABase + " (" + sStackEnd + ")";
}

/*********************************************************************
*
*       AddTask()
*
*  Functions description:
*    Get the task information and add it to the list.
*/
function AddTask(pTask, IsCurrent) {
  var Task;
  var sTaskName;
  var sPriority;
  var sState;
  var sStack;
  
  Task = Debug.evaluate("*(thread_t*)" + pTask);
    
  sTaskName = GetTaskName(pTask);
  sPriority = "" + Task.prio;
  sState = GetTaskState(Task.state);
  sStack = GetWorkingArea(pTask);
  sID = "0x" + pTask.toString(16).toUpperCase();
  
  if (IsCurrent == true) {
    Threads.add(sTaskName, sPriority, sState, sStack, sID, undefined);
  } else {
    Threads.add(sTaskName, sPriority, sState, sStack, sID, pTask);
  }
}

/*********************************************************************
*
*       API functions
*
**********************************************************************
*/

/*********************************************************************
*
*       getregs()
*
*  Functions description:
*    If a thread is selected in the threads window, this function returns
*    an array containing the registers r0-r12, sp, lr, and pc.
*
*  Parameters
*    pTask: Pointer to task control block
*/
function getregs(pTask) {
  var Task;
  var SavedRegs;
  var aRegs = new Array(16);
  //
  // Check if task is valid
  //
  Task = Debug.evaluate("*(thread_t*)" + pTask);
  if (Task == undefined) {
    return [];
  }
  //
  // Get the port context which holds the saved stack and registers
  //
  ctx = Debug.evaluate("((thread_t*)" + pTask + ")->ctx");
  if (ctx == undefined) {
    return [];
  }
  SavedRegs = Debug.evaluate("*(port_intctx*)" + ctx.sp);
  if (SavedRegs == undefined) {
    return [];
  }
  //
  // Restore the registers from saved stack
  //
  aRegs[0] = 0x00000000;        // Value not saved, unknown on context switch
  aRegs[1] = 0x00000000;        // Value not saved, unknown on context switch
  aRegs[2] = 0x00000000;        // Value not saved, unknown on context switch
  aRegs[3] = 0x00000000;        // Value not saved, unknown on context switch
  aRegs[4] = SavedRegs.r4;
  aRegs[5] = SavedRegs.r5;
  aRegs[6] = SavedRegs.r6;
  aRegs[7] = SavedRegs.r7;
  aRegs[8] = SavedRegs.r8;
  aRegs[9] = SavedRegs.r9;
  aRegs[10] = SavedRegs.r10;
  aRegs[11] = SavedRegs.r11;
  aRegs[15] = SavedRegs.lr;
  //
  // Restore stack pointer based on used stack register layout
  //
  if (SavedRegs.s16 != undefined) {
    aRegs[13] = ctx.sp + 100;  // r4-r11 + lr + s16-s31 saved
  } else {
    aRegs[13] = ctx.sp + 36;  // r4-r11 + lr saved
  }
  
  return aRegs;
}

/*********************************************************************
*
*       init()
*
*  Functions description:
*    This function describes the look of the threads window.
*/
function init() {
  Threads.clear();
  Threads.setColumns("Task", "Priority", "Status", "Working Area", "ID");
  Threads.setSortByNumber("Priority");
  if (Threads.setColor) {
    Threads.setColor("Status", "Ready", "Currently running", "Sleeping");
  }
}

/*********************************************************************
*
*       update()
*
*  Functions description:
*    This function is called to update the threads window and its
*    entries upon debug stop.
*/
function update() {
  var System;
  var ReadyList;
  var pTask;
  var pStart;
  var pNext;
  var pCurrent;
  var pfWakeup;
  //
  // Clear the threads window
  //
  Threads.clear();
  Threads.newqueue("Task List");
  //
  // Get main system variable
  //
  System = Debug.evaluate("ch");
  if (System == undefined) {
    return;
  }
  //
  // Get the address of wakeup()
  // to check if a timer handles a task
  //
  pfWakeup = Debug.evaluate("(int)&wakeup");
  pfWakeup = pfWakeup & 0xFFFFFFFE;
  //
  // Get current thread and add it if present
  //
  ReadyList = System.rlist;
  pStart = Debug.evaluate("(thread_t*)&(ch.rlist.queue)");
  pCurrent = Debug.evaluate("(thread_t*)" + ReadyList.current);
  if (pCurrent == undefined) {
    return;
  }
  if (pCurrent != 0) {
    AddTask(pCurrent, true);
    pNext = Debug.evaluate("(thread_t*)(*(thread_t*)" + pCurrent + ").newer");
  } else {
    pCurrent = Debug.evaluate("(thread_t*)" + ReadyList.queue.next);
    pNext = Debug.evaluate("(thread_t*)(*(thread_t*)" + ReadyList.queue.next + ").newer");
  }
  //
  // If the thread registry is used (.newer / .older is available), use it to get the full threads list.
  // Otherwise use the ready list and the timer list.
  //
  if (pNext != undefined) {
    //
    // Find end of list
    //
    if (pNext == pStart) {
      pTask = pCurrent;
    } else {
      pTask = pNext;
    }
    while (pTask != pStart) {
      pNext = Debug.evaluate("(thread_t*)(*(thread_t*)" + pTask + ").newer");
      if (pNext == undefined) {
        break;
      }
      if (pNext == pStart) {
        break;
      }
      pTask = pNext;
    }
    //
    // Go through list and add all tasks
    //
    while (pTask != pStart) {
      if (pTask != pCurrent) {
        AddTask(pTask, pTask == pCurrent);
      }
      pTask = Debug.evaluate("(thread_t*)(*(thread_t*)" + pTask + ").older");
      if (pTask == undefined) {
        break;
      }
    }
  } else {
    //
    // Get ready threads
    //
    pStart = Debug.evaluate("(thread_t*)&(ch.rlist.queue)");
    pTask = Debug.evaluate("(thread_t*)" + ReadyList.queue.next);
    if (pTask == undefined) {
      return;
    }
    while (pTask != 0) {
      if (pTask == pStart) {
        break;
      }
      AddTask(pTask, false);
      
      pNext = Debug.evaluate("(thread_t*)(*(thread_t*)" + pTask + ").queue.next");
      if (pNext == undefined) {
        break;
      }
      if (pNext == pTask) {
        break;
      }
      pTask = pNext;
    }
    //
    // Get waiting threads
    //
    TimerList = System.vtlist;
    pStart = Debug.evaluate("(virtual_timer_t*)&(ch.vtlist)");
    
    pTimer = Debug.evaluate("(virtual_timer_t*)" + TimerList.next);
    if (pTimer == undefined) {
      return;
    }
    while (pTimer != 0) {
      if (pTimer == pStart) {
        break;
      }
      //
      // Check if timer handles a task
      //
      pFunc = Debug.evaluate("((int*)(*(virtual_timer_t*)" + pTimer + ").func)");
      if (pFunc == undefined) {
        break;
      }
      pFunc = pFunc & 0xFFFFFFFE;
      if (pFunc == pfWakeup) {
        pTask = Debug.evaluate("(thread_t*)(*(virtual_timer_t*)" + pTimer + ").par");
        AddTask(pTask, false);
      } else {
        var s = "" + pFunc;
      }
      
      pNext = Debug.evaluate("(virtual_timer_t*)(*(virtual_timer_t*)" + pTimer + ").next");
      if (pNext == undefined) {
        break;
      }
      if (pNext == pTimer) {
        break;
      }
      pTimer = pNext;
    }
  }
}

/*********************************************************************
*
*       getContextSwitchAddrs()
*
*  Functions description:
*    Returns an unsigned integer array containing the base addresses 
*    of all functions that complete a task switch when executed.
*    For ChibiOS this is _port_switch (e.g. in chcore_v7m.S)
*/
function getContextSwitchAddrs() {
  var aAddrs;
  var Addr;
 
  Addr = Debug.evaluate("&_port_switch");
  if (Addr == undefined) {
    return [];
  }
  aAddrs = new Array(1);
  aAddrs[0] = Addr;
  return aAddrs;
}

/*********************************************************************
*
*       getOSName()
*
*  Functions description:
*    Returns the name of the OS this script is for.
*    Used by Ozone for display in the menu.
*/
function getOSName() {
  return "ChibiOS";
}

/****** End Of File *************************************************/
