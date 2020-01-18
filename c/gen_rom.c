/**********************************************************************
 * gen_rom.c : Generate the VHDL for a ROM holding sigma-delta samples
 *             of a 1kHZ sine wave
 *
 * NOTE - EXPERIMENTAL VERSION!
 *
 * Author : Mike Field <hamster@snap.net.nz>
 *********************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define N_SAMPLES (100*1000) /* Bits for 1kHz at 100MHz pin rate */
int main(int argc, char *argv[]) {
   int i,last;
   int32_t a = 0;
   double start_phase = 0, end_phase = 0;

   printf("library IEEE;\n");
   printf("use IEEE.STD_LOGIC_1164.ALL;\n");
   printf("use IEEE.numeric_std.all;\n");
   printf("\n");
   printf("entity dsdsg is\n");
   printf("   port ( clk100MHz : in  std_logic;\n");
   printf("          pin       : out std_logic);\n");
   printf("end entity;\n");
   printf("\n");
   printf("architecture whatever of dsdsg is\n");
   printf("  signal addr : unsigned(11 downto 0) := (others => '0');\n");
   printf("  signal data : std_logic_vector(31 downto 0) := ( 0 => '1', others => '0');\n");
   printf("  signal mask : std_logic_vector(31 downto 0) := ( 0 => '1', others => '0');\n");
   printf("  type a_memory is array (0 to %i) of std_logic_vector(31 downto 0);\n", N_SAMPLES/32-1);
   printf("  signal memory : a_memory := (\n");


   for(i = 0; i < N_SAMPLES; i++) {
     double rev_i = (i & ~0x1f) | (31-(i&0x1f)); // go from 31 to 0, not 0-31
     double phase = 2*M_PI*rev_i/N_SAMPLES;
     double f = (sin(phase)*0.166) * 32768 * 65536;
     int32_t v = f;

     if(i%32 == 0) {
       printf("     \"");
       end_phase = phase;
     }

     a += v;
     if(a > 0) {
       putchar('1');
       a -= 0x3FFFFFFF;
//     if( last == 0) a += 0x0003FFFF;
       last = 1; 
     } else {
       putchar('0');
       a += 0x3FFFFFFF;
//     if( last == 1) a -= 0x0003FFFF;
       last = 0; 
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

   printf("begin\n");
   printf("\n");
   printf("process(clk100MHz)\n");
   printf("    begin\n");
   printf("        if rising_edge(clK100MHz) then\n");
   printf("            if (mask and data) = x\"00000000\" then\n");
   printf("                pin <= '0';\n");
   printf("            else\n");
   printf("                pin <= '1';\n");
   printf("            end if;\n");
   printf("\n");
   printf("            data <= memory(to_integer(addr));\n");
   printf("\n");
   printf("            mask <= mask(30 downto 0) & mask(31); \n");
   printf("            if mask(30) = '1' then\n");
   printf("                if addr = memory'high then\n");
   printf("                    addr <= (others => '0');\n");
   printf("                else\n");
   printf("                    addr <= addr + 1;\n");
   printf("                end if;\n");
   printf("            end if;\n");
   printf("        end if;\n");
   printf("    end process;\n");
   printf("end whatever;\n");
}
