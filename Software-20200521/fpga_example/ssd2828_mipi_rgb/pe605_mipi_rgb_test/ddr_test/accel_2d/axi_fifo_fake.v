//
// Copyright 2012-2013 Ettus Research LLC
//

 

// Block RAM AXI fifo

// Special case SIZE <= 5 uses a short fifo

module axi_fifo_fake
  #(parameter WIDTH=32, SIZE=9)
   (input clk, input reset, input clear,
    input [WIDTH-1:0] i_tdata,
    input i_tvalid,
    output i_tready,
    output [WIDTH-1:0] o_tdata,
    output o_tvalid,
    input o_tready,
    
    output  [17:0] space,
    output  [17:0] occupied);

assign o_tdata = i_tdata;
assign o_tvalid = i_tvalid;
assign i_tready = o_tready;

assign space = 128;
assign occupied = 0;

endmodule // fifo_long
