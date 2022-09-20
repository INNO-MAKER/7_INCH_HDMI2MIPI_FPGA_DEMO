`timescale 1ns / 1ps
/* -------------------------------------------------------------------------
** This confidential and proprietary software may be only used as authorized
** by a licensing agreement from Tim.Yep (samssmarm@gmail.com). If you are 
** not an authorized user, please destroy this source code file and notify 
** Tim.Yep (samssmarm@gmail.com) immediately that you inadvertently received 
** an unauthorized copy.
** -------------------------------------------------------------------------
** Copyright (c) 2017 Tim.Yep ALL RIGHTS RESERVED
** Filename				:		disp_clk_crm.v
** Author				:		Tim.Yep
** Create Date&Time		:		
** Version				:		1.0
** Description			:		
** Modification History	:
** Date			By			Version			Change Description
** ==========================================================================
** 04/25/2020 		          1.0				First Release
**	
** --------------------------------------------------------------------------
*/

module disp_clk_crm
(
input           clk_in      ,
input           rst_in      ,

output          disp_clk    ,
output          disp_rst    ,
output          disp_clk_ok 
);


wire LOCKED;

disp_clk_wizard disp_clk_wizard_i
(
.CLK_IN1        (clk_in     ),      // IN
.CLK_OUT1       (disp_clk   ),     // OUT
.RESET          (rst_in     ),// IN
.LOCKED         (LOCKED     )   
); 

//hold-off logic for clocks ready
    reg [15:0] clocks_ready_count;
    reg clocks_ready;
    always @(posedge disp_clk or posedge rst_in or negedge LOCKED) begin
        if (rst_in | !LOCKED) begin
            clocks_ready_count <= 16'b0;
            clocks_ready <= 1'b0;
        end
        else if (!clocks_ready) begin
            clocks_ready_count <= clocks_ready_count + 1'b1;
            clocks_ready <= (clocks_ready_count == 16'hffff);
        end
    end

reset_sync bus_sync(.clk(disp_clk), .reset_in(!clocks_ready), .reset_out(disp_rst));	

assign disp_clk_ok = clocks_ready;

endmodule

