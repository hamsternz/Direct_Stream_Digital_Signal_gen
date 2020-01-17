open_hw_manager
connect_hw_server 
current_hw_target [get_hw_targets */Digilent/210183625087A ]
open_hw_target
set_property PROGRAM.FILE {../bitstreams/basys3_dsdsg.bit} [lindex [get_hw_devices] 0]
program_hw_devices [lindex [get_hw_devices] 0]
get_property REGISTER.IR.BIT5_DONE [lindex [get_hw_devices] 0]
close_hw_target
close_hw_manager
