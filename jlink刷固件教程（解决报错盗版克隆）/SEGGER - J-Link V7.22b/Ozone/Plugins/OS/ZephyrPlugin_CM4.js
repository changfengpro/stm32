/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : ZephyrPlugin_CM4.js
Purpose : Script for thread windows for Zephyr and SEGGER Embedded Studio/Ozone
          Includes FPU awareness.
          TODO: Restore FPU context
*/

/**** Zephyr list names, constant ************************************/
//var Mutexes             = "Mutexes";
//var Semaphores          = "Semaphores";
//var Queues              = "Queues";
//var Mailboxes           = "Mailboxes";
//var Timers              = "Timers";
//var MemoryPools         = "Memory Pools";
//var SystemInformation   = "System Information";
//@todo ...


function isValid( task ) {
    return !( ( task == undefined ) || (task == 0) );
}


function get_tcb(hTask) {
    return Debug.evaluate("*(k_thread*)" + hTask);
}

function get_thread_tid(hTask) {
    return "0x" + hTask.toString(16);
}

function get_thread_entry(hTask) {
    var task = get_tcb(hTask);
    
    if ( isValid(task) ) {
        if (Debug.getSymbol)   return Debug.getSymbol(   task.entry.pEntry );
        if (Debug.getfunction) return Debug.getfunction( task.entry.pEntry );
        return "0x" + task.entry.pEntry.toString(16);
    }
    return "undefined";
}

function get_thread_prio(hTask) {
    var task = get_tcb(hTask);
    if ( isValid(task) ) {
        if ( task.base.prio != undefined )                                       return task.base.prio;
        if ( task.base["<<anonymous>_1>"]["<<anonymous>_0>"].prio != undefined ) return task.base["<<anonymous>_1>"]["<<anonymous>_0>"].prio;
    }
    return "undefined";
}

function get_thread_state(hTask) {
    var task = get_tcb(hTask);
    if (task == undefined ||  task == 0) {
        return "invalid";
    } else {
        // Not a real thread _THREAD_DUMMY (BIT(0))
        // Thread is waiting on an object _THREAD_PENDING (BIT(1))
        // Thread has not yet started  _THREAD_PRESTART (BIT(2))
        // Thread has terminated _THREAD_DEAD (BIT(3))
        // Thread is suspended _THREAD_SUSPENDED (BIT(4))
        // Thread is being aborted (SMP only) _THREAD_ABORTING (BIT(5))
        // Thread was aborted in interrupt context (SMP only) _THREAD_ABORTED_IN_ISR (BIT(6))
        // Thread is present in the ready queue _THREAD_QUEUED (BIT(7))

        var state = task.base.thread_state;

        if (state & (1 << 3)) {
            return "Dead";
        }

        if (state & (1 << 2)) {
            return "Prestart";
        }

        if (state & (1 << 5)) {
            return "Aborting";
        }

        if (state & (1 << 6)) {
            return "Aborting";
        }

        if (state & (1 << 1)) {
            return "Blocked";
        }

        if (state & (1 << 4)) {
            return "Waiting";
        }

        if (state & (1 << 7)) {
            return "Ready";
        }

        return "Invalid";
    }
}

function get_thread_name(hTask) {
    var task = get_tcb(hTask);
    if ( isValid(task) ) {
        var name = "";
        if ( isValid(task.name) ) {
            var name_len = task.name.length;

            for (var i = 0; i < name_len; i++) {
                var c = task.name[i];
                if (c != 0) {
                    name += String.fromCharCode(c);
                }
            }

            return name;
        }
        return "T0x"+hTask.toString(16)+"@"+get_thread_entry(hTask);
    }
    return "invalid";
}


function updateThreads() {
    if (Threads.newqueue2 == undefined) {
      Threads.newqueue( "Task List" )
    }
    var idle_ptask = Debug.evaluate("&z_idle_threads[0]");
    //idle_ptask.toString(16);

    var main_ptask = Debug.evaluate("&z_main_thread");
    //main_ptask.toString(16);

    var current_ptask = Debug.evaluate("_kernel.ready_q.cache");
    //current_ptask.toString(16);

    var ptask = Debug.evaluate("_kernel.threads");
    //ptask.toString(16);
    if( false == isValid(ptask) ) Threads.add( "Enable THREAD_MONITOR for information", undefined );

    while ( isValid(ptask) ) {
        var task = get_tcb(ptask);

        var task_name = get_thread_name(ptask);
        var tid       = get_thread_tid(ptask);
        var entry     = get_thread_entry(ptask);
        var prio      = get_thread_prio(ptask).toString();

        var status;
        if (current_ptask == ptask)    status = "Executing";
        else if (idle_ptask == ptask)  status = "Ready";
        else                           status = get_thread_state(ptask);

        var SP = task.callee_saved.psp;

        Threads.add( task_name, tid, entry, prio, status, "0x" + SP.toString(16), (current_ptask == ptask) ? undefined : ptask);

        ptask = task.next_thread;
    }
}

function UpdateMutexes( Window ) {
    var pMutex = undefined; // add here a valid address to get all OS objects of interest
    while (pMutex) {
        //
        // Get necessary information about OS object and populate them to table.
        //
        
        // Show values in table
        Window.add2( Mutextes,                                  // Table Mutextes
                    "0x" + pMutex.toString(16).toUpperCase(),   // Address of mutex
                    "..."                                       // everything else
        );                                    
    }
}

function UpdateTodo( Window ) {
    Window.add2("...", "Todo: Add more information" );
}


function init() {
    Threads.clear();
    
    // Init the theads table
    Threads.newqueue("Task List");
    Threads.setColumns("Threads", "TID", "Entry", "Priority", "Status", "Stack" );
    
    Threads.setSortByNumber("TID");
    Threads.setSortByNumber("Priority");
    Threads.setSortByNumber("Stack");

    if (Threads.setColor) {
        Threads.setColor("Status", "Ready", "Executing", "Waiting");
    }

    if (Threads.setColumns2) {
        if(Mutexes          ) Threads.setColumns2(Mutexes,            "Mutexes" /*,     "Name", "Owner",        "Use Counter",    "Waiting Tasks"                                   */);
        if(Semaphores       ) Threads.setColumns2(Queues,             "Queues"/*,       "Name", "Messages",     "Buffer Address", "Buffer Size",    "Waiting Tasks"                  */);
        if(Queues           ) Threads.setColumns2(Semaphores,         "Semaphores"/*,   "Name", "Count",        "Waiting Tasks"                                                      */);
        if(Mailboxes        ) Threads.setColumns2(Mailboxes,          "Mailboxes"/*,    "Name", "Messages",     "Message Size",   "Buffer Address", "Waiting Tasks",  "In Use"       */);
        if(Timers           ) Threads.setColumns2(Timers,             "Timers"/*,       "Name", "Hook",         "Timeout",        "Period"                                           */);
        if(MemoryPools      ) Threads.setColumns2(MemoryPools,        "MemoryPools"/*,  "Name", "Total Blocks", "Block Size",     "Max. Usage",     "Buffer Address", "Waiting Tasks"*/);
        if(SystemInformation) Threads.setColumns2(SystemInformation,  SystemInformation, "Value"                                                                                       );
        Threads.setColumns2(                      "...", "..." );
    }
    TargetInterface.message( "Loaded" );
}

function update() {
    //
    // Clear entire threads window
    //
    Threads.clear();
    //
    // Update plug in lists
    //
    if (Threads.newqueue2 != undefined) {
        if (Mutexes           && Threads.shown(Mutexes))            UpdateMutexes(Threads);
        if (Semaphores        && Threads.shown(Semaphores))         UpdateSemaphores(Threads);
        if (Queues            && Threads.shown(Queues))             UpdateQueues(Threads);
        if (Mailboxes         && Threads.shown(Mailboxes))          UpdateMailboxes(Threads);
        if (Timers            && Threads.shown(Timers))             UpdateTimers(Threads);
        if (MemoryPools       && Threads.shown(MemoryPools))        UpdateMemoryPools(Threads);
        if (SystemInformation && Threads.shown(SystemInformation))  UpdateSystemInformation(Threads);
        // @todo Add more Lists
        if (                     Threads.shown("..."))              UpdateTodo(Threads);
    }
    updateThreads();
}

function getname(hTask) {
    return get_thread_name(hTask);
}

function getregs(hTask) {
    TargetInterface.message( "refresh registers" );
    FPSCR  = 39;
    S0ofs  = 64;
    S16ofs = S0ofs + 16;
    
    var regs = new Array(100);

    var task  =  get_tcb(hTask);
    //
    // Restore task stack (SP) from thread
    //
    var SP = task.callee_saved.psp;
    //
    // Restore R4-R11 from thread
    //
    regs[ 4] = task.callee_saved.v1;
    regs[ 5] = task.callee_saved.v2;
    regs[ 6] = task.callee_saved.v3;
    regs[ 7] = task.callee_saved.v4;
    regs[ 8] = task.callee_saved.v5;
    regs[ 9] = task.callee_saved.v6;
    regs[10] = task.callee_saved.v7;
    regs[11] = task.callee_saved.v8;
    //
    // Restore automatically saved R0-R3, and R12, LR, PC, xPSR from task stack
    //
    for (var i = 0; i < 4; i++){                    // R0..R3
        regs[i] = TargetInterface.peekWord(SP);
        SP += 4;
    }
    regs[12] = TargetInterface.peekWord(SP);        // IP
    SP += 4;
    regs[14] = TargetInterface.peekWord(SP);        // LR
    SP += 4;
    regs[15] = TargetInterface.peekWord(SP);        // PC = return address
    SP += 4;
    regs[16] = TargetInterface.peekWord(SP);        // xPSR
    SP += 4;
    //
    // Restore saved FPU registers.
    //
    var pFPRegs = Debug.evaluate( "thread->arch.preempt_float" );
    
    //if ( isValid(pFPRegs) && (regs[14] & 0x10) != 0x10 ) { // test EXC_RETURN.F_Type_Msk
        // FPU was active for this thread, so restore the FP-registers
        // restore S0..S15, FPSCR from stack
        for( var i=0; i<16; ++i ) {
            regs[S0ofs + i]  = TargetInterface.peekWord(SP);             // S0..15
            SP += 4;
        }
        regs[FPSCR]          = TargetInterface.peekWord(SP);             // FPSCR
        SP += 4;
        SP += 4; // Dummy read
        
        // restore S16..S31 form thread->arch.preempt_float
        for( var i=0; i<16; ++i ) {
            regs[S16ofs + i] = TargetInterface.peekWord(pFPRegs + 4*i);  // S16..31
        }
    //}
    //
    // 8 byte aligned SP in interrupt entry enabled? (Set in xPSR[9])
    // Increment SP after reading registers before accessing further variables from stack
    //
    if (regs[16] & (1<<9)) { 
        if ((SP % 8) != 0) { // Check is stack is already aligned
            // enforce SP 8 byte alignement
            SP += 4;
        }
    }
    //
    // Restore SP after adjusting for saved registers. 
    // Now points to function stack frame.
    //
    regs[13] = SP;

    return regs;
}

function getContextSwitchAddrs() {
    var aAddrs;
    var Addr;

    aFuncts = [
        Debug.evaluate("&__swap")
     ];

    for( a in aFuncts ) {
        if (a != undefined ) aAddrs.push( a );
    }
    
    return aAddrs;
}

function getOSName() {
    return "Zephyr";
}
