#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define the interrupt vector table.
uint32_t interrupt_vector_table[256];

// Define the interrupt service routine for system calls.
void system_call_interrupt_handler(uint32_t interrupt_number) {
  // Save the state of the current process.
  uint32_t regs[16];
  for (int i = 0; i < 16; i++) {
    regs[i] = __asm__ __volatile__("mov %0, %%eax" : "=r"(regs[i]));
  }

  // Perform the requested system call.
  uint32_t system_call = regs[0];
  switch (system_call) {
    case SYS_WRITE:
      printf("Write system call\n");
      break;
    case SYS_READ:
      printf("Read system call\n");
      break;
    case SYS_EXIT:
      printf("Exit system call\n");
      exit(0);
      break;
    default:
      printf("Unknown system call: %d\n", system_call);
      break;
  }

  // Restore the state of the current process.
  for (int i = 0; i < 16; i++) {
    __asm__ __volatile__("mov %%eax, %0" : "=m"(regs[i]));
  }

  // Return from the interrupt.
  __asm__ __volatile__("iret");
}

int main() {
  // Install the system call interrupt handler.
  interrupt_vector_table[0x80] = system_call_interrupt_handler;

  // Trigger a system call.
  __asm__ __volatile__("int $0x80");

  // The system call should return to this point after it has been serviced.
  printf("Returned from system call\n");

  return 0;
}
