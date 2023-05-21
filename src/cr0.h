#ifndef CR0_H_
#define CR0_H_

#define DISABLE_WP_CR0(x)                                   \
  asm volatile ("pushq %%rax\n\t"                           \
                "movq  %%cr0, %%rax\n\t"                    \
                "andq  $0xFFFFFFFFFFFEFFFF, %%rax\n\t"      \
                "movq  %%rax, %%cr0\n\t"                    \
                "popq  %%rax"                               \
                :                                           \
                :                                           \
                : "%rax");

#define ENABLE_WP_CR0(x)                                    \
  asm volatile ("pushq %%rax\n\t"                           \
                "movq  %%cr0, %%rax\n\t"                    \
                "orq   $0x10000, %%rax\n\t"                 \
                "movq  %%rax, %%cr0\n\t"                    \
                "popq  %%rax"                               \
                :                                           \
                :                                           \
                : "%rax");

#endif // CR0_H_
