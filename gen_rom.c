/**********************************************************************
 * gen_rom.c : Generate the VHDL for a ROM holding sigma-delta samples
 *             of a 1kHZ sine wave
 *
 * Author : Mike Field <hamster@snap.net.nz>
 *********************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define N_SAMPLES (100*1000) /* Bits for 1kHz at 100MHz pin rate */
int main(int argc, char *argv[]) {
   int i;
   uint32_t a = (1<<31)+100;
   double start_phase = 0, end_phase = 0;

   printf("  type a_memory is array (0 to %i) of std_logic_vector(31 downto 0);\n", N_SAMPLES/32-1);
   printf("  signal memory : a_memory := (\n");


   for(i = 0; i < N_SAMPLES; i++) {
     double rev_i = (i & ~0x1f) | (31-(i&0x1f)); // go from 31 to 0, not 0-31
     double phase = 2*M_PI*rev_i/N_SAMPLES;
     double f = (sin(phase)*0.336+0.5) * 65536 * 65536;
     uint32_t v = f;
     if(i%32 == 0) {
       printf("     \"");
       end_phase = phase;
     }
     if(a > 0xFFFFFFFFU - v) {
       putchar('1');
     } else {
       putchar('0');
     }
     if(i%32 == 31) {
        start_phase = phase;
        if(i == N_SAMPLES-1)
           printf("\"); -- %10.7f - %10.7f\n", start_phase, end_phase);
        else
           printf("\",  -- %10.7f - %10.7f\n", start_phase, end_phase);
     }
     a += v;
   }
   fprintf(stderr, "A is %08x\n",a);
}
