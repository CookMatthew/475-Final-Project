# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\pr3de\workspace\RSA_system\_ide\scripts\debugger_rsa-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\pr3de\workspace\RSA_system\_ide\scripts\debugger_rsa-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -filter {jtag_cable_name =~ "Digilent Arty A7-35T 210319B0C4EFA" && level==0 && jtag_device_ctx=="jsn-Arty A7-35T-210319B0C4EFA-0362d093-0"}
fpga -file C:/Users/pr3de/workspace/RSA/_ide/bitstream/TopLevel_wrapper.bit
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
loadhw -hw C:/Users/pr3de/workspace/ECE475Project/export/ECE475Project/hw/TopLevel_wrapper.xsa -regs
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
rst -system
after 3000
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
dow C:/Users/pr3de/workspace/RSA/Debug/RSA.elf
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
con
