gui_open_window Wave
gui_sg_create disp_clk_wizard_group
gui_list_add_group -id Wave.1 {disp_clk_wizard_group}
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.test_phase}
gui_set_radix -radix {ascii} -signals {disp_clk_wizard_tb.test_phase}
gui_sg_addsignal -group disp_clk_wizard_group {{Input_clocks}} -divider
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.CLK_IN1}
gui_sg_addsignal -group disp_clk_wizard_group {{Output_clocks}} -divider
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.dut.clk}
gui_list_expand -id Wave.1 disp_clk_wizard_tb.dut.clk
gui_sg_addsignal -group disp_clk_wizard_group {{Status_control}} -divider
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.RESET}
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.LOCKED}
gui_sg_addsignal -group disp_clk_wizard_group {{Counters}} -divider
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.COUNT}
gui_sg_addsignal -group disp_clk_wizard_group {disp_clk_wizard_tb.dut.counter}
gui_list_expand -id Wave.1 disp_clk_wizard_tb.dut.counter
gui_zoom -window Wave.1 -full
