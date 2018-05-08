#include "kernel.h"
#include "tests.h"

#ifdef TEST00
#define TEST_NUM 0

#elif TEST01
#define TEST_NUM 1

#elif TEST02
#define TEST_NUM 2

#elif TEST03
#define TEST_NUM 3

#elif TEST04
#define TEST_NUM 4

#elif TEST05
#define TEST_NUM 5

#elif TEST06
#define TEST_NUM 6

#elif TEST07
#define TEST_NUM 7

#elif TEST08
#define TEST_NUM 8

#elif TEST09
#define TEST_NUM 9

#elif TEST10
#define TEST_NUM 10

#elif TEST11
#define TEST_NUM 11

#elif TEST12
#define TEST_NUM 12

#elif TEST13
#define TEST_NUM 13

#elif TEST14
#define TEST_NUM 14

#elif TEST15
#define TEST_NUM 15

#elif TEST16
#define TEST_NUM 16

#elif TEST17
#define TEST_NUM 17

#elif TEST18
#define TEST_NUM 18

#elif TEST19
#define TEST_NUM 19

#elif TEST20
#define TEST_NUM 20

#elif TEST21
#define TEST_NUM 21

#elif TEST22
#define TEST_NUM 22

#elif TEST23
#define TEST_NUM 23

#elif TEST24
#define TEST_NUM 24

#elif TEST25
#define TEST_NUM 25

#else
#define TEST_NUM 255

#endif

struct bootparams *bootparams;
slock _malloc_lock = 0;
slock* malloc_lock = &_malloc_lock;

int debug = 0; // change to 0 to stop seeing so many messages

void shutdown() {
  puts("Shutting down...");
  // this is really the "wait" instruction, which gcc doesn't seem to know about
  __asm__ __volatile__ ( ".word 0x42000020\n\t");
  while (1);
}


/* Trap handling.
 *
 * Only core 0 will get interrupts, but any core can get an exception (or
 * syscall).  So we allocate enough space for every core i to have its own place
 * to store register state (trap_save_state[i]), its own stack to use in the
 * interrupt handler (trap_stack_top[i]), and a valid $gp to use during
 * interrupt handling (trap_gp[i]).
 */

struct mips_core_data *trap_save_state[MAX_CORES]; /* one save-state per core */
void *trap_stack_top[MAX_CORES]; /* one trap stack per core */
unsigned int trap_gp[MAX_CORES]; /* one trap $gp per core */

void trap_init()
{
  int id = current_cpu_id();

  /* trap should use the same $gp as the current code */
  trap_gp[id] = current_cpu_gp();

  /* trap should use a fresh stack */
  void *bottom = alloc_pages(4);
  trap_stack_top[id] = bottom + 4*PAGE_SIZE - 4;

  /* put the trap save-state at the top of the corresponding trap stack */
  trap_stack_top[id] -= sizeof(struct mips_core_data);
  trap_save_state[id] = trap_stack_top[id];

  /* it is now safe to take interrupts on this core */
  intr_restore(1);
}

void interrupt_handler(int cause)
{
  // note: interrupts will only happen on core 0
  // diagnose the source(s) of the interrupt trap
  int pending_interrupts = (cause >> 8) & 0xff;
  int unhandled_interrupts = pending_interrupts;

  if (pending_interrupts & (1 << INTR_KEYBOARD)) {
    if (debug) printf("interrupt_handler: got a keyboard interrupt, IGNORING it\n");
    //keyboard_trap();
    unhandled_interrupts &= ~(1 << INTR_KEYBOARD);
  }

  if (pending_interrupts & (1 << INTR_TIMER)) {
    printf("interrupt_handler: got a spurious timer interrupt, ignoring it and hoping it doesn't happen again\n");
    unhandled_interrupts &= ~(1 << INTR_TIMER);
  }

  if (unhandled_interrupts != 0) {
    printf("got interrupt_handler: one or more other interrupts (0x%08x)...\n", unhandled_interrupts);
  }

}

void trap_handler(struct mips_core_data *state, unsigned int status, unsigned int cause)
{
  if (debug) printf("trap_handler: status=0x%08x cause=0x%08x on core %d\n", status, cause, current_cpu_id());
  // diagnose the cause of the trap
  int ecode = (cause & 0x7c) >> 2;
  switch (ecode) {
    case ECODE_INT:	  /* external interrupt */
      interrupt_handler(cause);
      return; /* this is the only exception we currently handle; all others cause a shutdown() */
    case ECODE_MOD:	  /* attempt to write to a non-writable page */
      printf("trap_handler: some code is trying to write to a non-writable page!\n");
      break;
    case ECODE_TLBL:	  /* page fault during load or instruction fetch */
    case ECODE_TLBS:	  /* page fault during store */
      printf("trap_handler: some code is trying to access a bad virtual address!\n");
      break;
    case ECODE_ADDRL:	  /* unaligned address during load or instruction fetch */
    case ECODE_ADDRS:	  /* unaligned address during store */
      printf("trap_handler: some code is trying to access a mis-aligned address!\n");
      break;
    case ECODE_IBUS:	  /* instruction fetch bus error */
      printf("trap_handler: some code is trying to execute non-RAM physical addresses!\n");
      break;
    case ECODE_DBUS:	  /* data load/store bus error */
      printf("trap_handler: some code is read or write physical address that can't be!\n");
      break;
    case ECODE_SYSCALL:	  /* system call */
      printf("trap_handler: who is doing a syscall? not in this project...\n");
      break;
    case ECODE_BKPT:	  /* breakpoint */
      printf("trap_handler: reached breakpoint, or maybe did a divide by zero!\n");
      break;
    case ECODE_RI:	  /* reserved opcode */
      printf("trap_handler: trying to execute something that isn't a valid instruction!\n");
      break;
    case ECODE_OVF:	  /* arithmetic overflow */
      printf("trap_handler: some code had an arithmetic overflow!\n");
      break;
    case ECODE_NOEX:	  /* attempt to execute to a non-executable page */
      printf("trap_handler: some code attempted to execute a non-executable virtual address!\n");
      break;
    default:
      printf("trap_handler: unknown error code 0x%x\n", ecode);
      break;
  }
  shutdown();
}


/* kernel entry point called at the end of the boot sequence */
void __boot() {

  if (current_cpu_id() == 0) {
    /* core 0 boots first, and does all of the initialization */

    // boot parameters are on physical page 0
    bootparams = physical_to_virtual(0x00000000);

    // initialize console early, so output works
    console_init();

    // output should now work
    printf("Welcome to CS3410 final project --MALLOC--!\n");
    printf("Running on a %d-way multi-core machine\n", current_cpu_exists());

    // initialize memory allocators
    mem_init();

    // prepare to handle interrupts, exceptions, etc.
    trap_init();

    // initialize keyboard late, since it isn't really used by anything else
    //keyboard_init();

#ifdef USE_DISPLAY
    display_init();
#endif

    //Test_zone
    /*************************INSERT_CODE_BELOW_FOR_MALLOC_PROJECT**************************/

    int (*tests[])(void) = { test00, test01, test02, test03, test04, test05,  /* 0-15 are SPEC TESTS */
                             test06, test07, test08, test09, test10,
                             test11, test12, test13, test14, test15,
                             test16, test17, test18, test19,                  /* 16-23 are STRESS TESTS */
                             test20, test21, test22, test23,
			     test24, test25 };				      /* 24-25 are UTILIZATION TESTS */                

    int succ = 0;
    int failed = 0;

    printf("<-CS3410 malloc tester-> Starting SPEC tests...\n");
    if(TEST_NUM == 255)
    {
    for(int test_num=0;test_num<16;test_num++) {
	printf("<-CS3410 malloc tester-> Running test: %d: %s\n", test_num, test_descriptions[test_num]);
        if(tests[test_num]()==1) {
            printf("<-CS3410 malloc tester-> Test %d passed!\n", test_num);
	    succ++;
	}
	else {
	    failed++;
	}
    }

    if(succ!=10) {
	printf("<-CS3410 malloc tester-> Will not go to STRESS test since there are failed SPEC tests!\n");
	shutdown();
    }

    printf("<-CS3410 malloc tester-> Starting STRESS tests...\n");

    for(int test_num=16;test_num<24;test_num++) {
	printf("<-CS3410 malloc tester-> Running test: %d: %s\n", test_num, test_descriptions[test_num]);
        if(tests[test_num]()==1) {
            printf("<-CS3410 malloc tester-> Test %d passed!\n", test_num);
            succ++;
	}
	else {
	    failed++;
	}
    }

    printf("<-CS3410 malloc tester-> All done! %d PASSED and %d FAILED\n", succ,failed);

    if(succ==17) {
	printf("<-CS3410 malloc tester-> Your code is eligible for utilization testing.\n");
	for(int test_num=24;test_num<26;test_num++) {
	    printf("<-CS3410 malloc tester-> Running test: %d: %s\n", test_num, test_descriptions[test_num]);
	    tests[test_num]();
	}
    } 
    }
    else
    {
        printf("<-CS3410 malloc tester-> ONLY run TEST CASE %d\n",TEST_NUM);
        if(tests[TEST_NUM]()==1) {
            printf("<-CS3410 malloc tester-> Test %d passed!\n", TEST_NUM);
	    }
        else
        {
            printf("<-CS3410 malloc tester-> Test %d FAILED!\n", TEST_NUM);
        }

    }
    /*************************INSERT_CODE_ABOVE_FOR_MALLOC_PROJECT**************************/

    // turn on the other cores
    //set_cpu_enable(0xFFFFFFFF);
    busy_wait(0.1);

  }
  else if (current_cpu_id() == 1) { /* distribution core */
  }
  else { /* processing cores */
  }

  shutdown();
}

