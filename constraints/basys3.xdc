set_property PACKAGE_PIN W5 [get_ports clk100MHz]							
set_property IOSTANDARD LVCMOS33 [get_ports clk100MHz]
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports clk100MHz]

##Sch name = JA4
set_property PACKAGE_PIN G2    [get_ports {pin}]
set_property IOSTANDARD  LVTTL [get_ports {pin}]
set_property DRIVE       24     [get_ports {pin}]

