module cnt_frm
(
input  wire          disp_clk      ,
input  wire          disp_rst      ,
       
input  wire          vsync         ,
output reg  [8:0]    frm_cnt 
);

reg [1:0] vsync_ff;

always @(posedge disp_clk)
  vsync_ff <= {vsync_ff[0], vsync};
  
wire vsync_ff_rising = vsync_ff == 2'b01;  

always @(posedge disp_clk)
if(disp_rst)
   frm_cnt <= 0;
else if(vsync_ff_rising)   
   frm_cnt <= frm_cnt + 1;
   
   

endmodule