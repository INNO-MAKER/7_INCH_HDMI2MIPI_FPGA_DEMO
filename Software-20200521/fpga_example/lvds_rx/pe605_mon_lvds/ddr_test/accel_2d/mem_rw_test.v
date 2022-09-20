module mem_rw_test
#(parameter USR_CLK = 32'd167000000, WIDTH=64, X_RES=1920, Y_RES=1080, DDR_BASE=32'h00000000)
(
input           clk             ,
input           rst             ,
input           Init_done       ,

output  [31:0]  M_AXI_araddr    ,
output  [1:0]   M_AXI_arburst   ,
output  [3:0]   M_AXI_arcache   ,
output  [5:0]   M_AXI_arid      ,
output  [7:0]   M_AXI_arlen     ,
output  [1:0]   M_AXI_arlock    ,
output  [2:0]   M_AXI_arprot    ,
output  [3:0]   M_AXI_arqos     ,
input           M_AXI_arready   ,
output  [2:0]   M_AXI_arsize    ,
output          M_AXI_arvalid   ,
output  [31:0]  M_AXI_awaddr    ,
output  [1:0]   M_AXI_awburst   ,
output  [3:0]   M_AXI_awcache   ,
output  [5:0]   M_AXI_awid      ,
output  [7:0]   M_AXI_awlen     ,
output  [1:0]   M_AXI_awlock    ,
output  [2:0]   M_AXI_awprot    ,
output  [3:0]   M_AXI_awqos     ,
input           M_AXI_awready   ,
output  [2:0]   M_AXI_awsize    ,
output          M_AXI_awvalid   ,
input   [5:0]   M_AXI_bid       ,
output          M_AXI_bready    ,
input   [1:0]   M_AXI_bresp     ,
input           M_AXI_bvalid    ,
input   [63:0]  M_AXI_rdata     ,
input   [5:0]   M_AXI_rid       ,
input           M_AXI_rlast     ,
output          M_AXI_rready    ,
input   [1:0]   M_AXI_rresp     ,
input           M_AXI_rvalid    ,
output  [63:0]  M_AXI_wdata     ,
output  [5:0]   M_AXI_wid       ,
output          M_AXI_wlast     ,
input           M_AXI_wready    ,
output  [7:0]   M_AXI_wstrb     ,
output          M_AXI_wvalid    
);

function integer funclog2; 
   input integer value; 
   begin 
     value = value-1; 
     for (funclog2=0; value>0; funclog2=funclog2+1) 
       value = value>>1; 
   end 
endfunction 

localparam XFER_SIZE     = funclog2(WIDTH/8)                 ;
localparam XFER_BYTE     = WIDTH/8                           ;
localparam WR_STRB       = {XFER_BYTE{1'b1}}                 ;
		                                                     
localparam COL_ITER      = 1920/(WIDTH/8) - 1                ;
localparam STRIDE_B      = 3*(WIDTH/8)                       ;
localparam WR_STRIDE     = STRIDE_B*Y_RES                    ;
localparam XRES_M1       = X_RES - 1                         ;
localparam YRES_M1       = Y_RES - 1                         ;
localparam LEFT_S        = (WIDTH == 64) ? 3'b0 : 2'b0       ;
localparam PAGE_LAST_B   = 13'h1000 - (WIDTH/8)              ;
localparam PAGE_LAST_2B  = 13'h1000 - 2*(WIDTH/8)            ;
                         
localparam RD_STRIDE_B 	 = 512							     ;
localparam RD_LASTB_ADDR = (X_RES * Y_RES * 3) - RD_STRIDE_B ;

localparam WR_STRIDE_B 	 = 128							     ;
localparam WR_LASTB_ADDR = (X_RES * Y_RES * 3) - WR_STRIDE_B ;
localparam WR_CNT_M1     = 128/8 - 1;
localparam WR_FRM_M1     = X_RES * Y_RES - 1;

wire            Init_done_r     ;
reg     [2:0]   Init_done_dl    ;

assign          M_AXI_arburst = 2'b01    ;
assign          M_AXI_arcache = 4'b0     ;
assign          M_AXI_arid    = 6'b0     ;
assign          M_AXI_arlen   = 8'd127   ;
assign          M_AXI_arlock  = 2'b0     ;
assign          M_AXI_arprot  = 3'b0     ;
assign          M_AXI_arqos   = 4'b0     ;
assign          M_AXI_arsize  = 3'b011   ;

assign          M_AXI_awburst = 2'b01    ;
assign          M_AXI_awcache = 4'b0     ;
assign          M_AXI_awid    = 6'b0     ;
assign          M_AXI_awlen   = WR_CNT_M1;
assign          M_AXI_awlock  = 2'b0     ;
assign          M_AXI_awprot  = 3'b0     ;
assign          M_AXI_awqos   = 4'b0     ;
assign          M_AXI_awsize  = 3'b011   ;
assign          M_AXI_wstrb   = 8'hff    ;
assign          M_AXI_wid     = 6'b0     ;
assign          M_AXI_bready  = 1'b1     ;

wire            M_AXI_wvalid_int         ;
wire            M_AXI_wready_int         ;
wire    [63:0]  M_AXI_wdata_int          ;
wire            M_AXI_wlast_int          ;
wire    [17:0]  wr_occu                  ;
wire            M_AXI_wvalid_int2        ;
wire            M_AXI_wready_int2        ;

assign          Init_done_r = Init_done_dl[2];
assign          M_AXI_wvalid_int = Init_done_r && (wr_vld_cn == 2'd3)	;
reg     [7:0]   wr_cn                    ;
reg     [63:0]  wr_dat                   ;
reg     [1:0]   wr_vld_cn                ;
always @(posedge clk)
if(rst) begin
	wr_cn <= 0;
	wr_dat <= 0;
	Init_done_dl <= 0;
	wr_vld_cn <= 0;
end
else begin
	Init_done_dl <= {Init_done_dl[1:0], Init_done};
	//if(wr_vld_cn < 2)
		wr_vld_cn <= wr_vld_cn + 1'b1;
	//else
		//wr_vld_cn <= 0;
	if(M_AXI_wvalid_int & M_AXI_wready_int & Init_done_r) begin
		if(wr_cn < WR_CNT_M1)
			wr_cn <= wr_cn + 1'b1;
		else
			wr_cn <= 0;
		
		if(wr_dat < WR_FRM_M1)
			wr_dat <= wr_dat + 1'b1;
		else
			wr_dat <= 0;
	end
end
assign M_AXI_wdata_int = wr_dat;
assign M_AXI_wlast_int = wr_cn == WR_CNT_M1;

axi_fifo #(.WIDTH(65), .SIZE(8)) wr_fifo_i
(
.clk        (clk                                ), 
.reset      (rst                                ), 
.clear      (1'b0                               ),
.i_tdata    ({M_AXI_wlast_int, M_AXI_wdata_int} ), 
.i_tvalid   (M_AXI_wvalid_int                   ), 
.i_tready   (M_AXI_wready_int                   ),
.o_tdata    ({M_AXI_wlast, M_AXI_wdata}         ), 
.o_tvalid   (M_AXI_wvalid_int2                  ), 
.o_tready   (M_AXI_wready_int2                  ),
.occupied   (wr_occu                            ),
.space      (                                   )
);

localparam 	WAIT_WR 	= 1'b0;
localparam 	ISSUE_WR 	= 1'b1;
reg			wr_st		;
reg	[8:0]	wr_issue_cn	;
always @(posedge clk)
if(rst) begin
	wr_st <= WAIT_WR;
	wr_issue_cn <= 0;
end
else begin
	case(wr_st)
	WAIT_WR : begin
		if(wr_occu > WR_CNT_M1) begin
			wr_st <= ISSUE_WR;
			wr_issue_cn <= 0;
		end
	end
	ISSUE_WR : begin
		if(M_AXI_wvalid && M_AXI_wready) begin
			if(wr_issue_cn < WR_CNT_M1)
				wr_issue_cn <= wr_issue_cn + 1'b1;
			else begin
				wr_issue_cn <= 0;
				wr_st <= WAIT_WR;
			end
		end
	end
	endcase
end

assign M_AXI_wvalid = M_AXI_wvalid_int2 && (wr_st == ISSUE_WR);
assign M_AXI_wready_int2 = M_AXI_wready && (wr_st == ISSUE_WR);

assign M_AXI_awvalid = Init_done_r;
reg		[31:0]	wr_addr		;

always @(posedge clk)
if(rst) begin
	wr_addr <= 0;
end
else begin
	if(M_AXI_awvalid & M_AXI_awready & Init_done_r) begin
		if(wr_addr >= WR_LASTB_ADDR)
			wr_addr <= 0;
		else
			wr_addr <= wr_addr + WR_STRIDE_B;
	end
end
assign M_AXI_awaddr = wr_addr;

//rd section
reg	[31:0]	rd_addr		;

always @(posedge clk)
if(rst) begin
	rd_addr <= 0;
end
else begin
	if(M_AXI_arvalid & M_AXI_arready) begin
		if(rd_addr >= RD_LASTB_ADDR) //64bit burst 512 bytes
			rd_addr <= 0;
		else
			rd_addr <= rd_addr + RD_STRIDE_B; 
	end 
end

assign M_AXI_araddr		= rd_addr;
assign M_AXI_arvalid	= Init_done_r && (wr_st == WAIT_WR);
//assign M_AXI_arvalid	= 1'b0;
assign M_AXI_rready 	= 1'b1;

reg [23:0] rd_cnt;
always @(posedge clk)
if(rst) begin
	rd_cnt <= 0;
end
else begin
	if(M_AXI_rvalid) begin
		if(rd_cnt < 2073599)
			rd_cnt <= rd_cnt + 1'b1;
		else
			rd_cnt <= 0;
	end
end

reg	[31:0] timer_cn      ;
reg	[31:0] wr_beat       ;
reg	[31:0] rd_beat       ;
reg	[31:0] wr_cnt_ps     ;
reg	[31:0] rd_cnt_ps     ;
always @(posedge clk)
if(rst) begin
	timer_cn <= 0;
	wr_beat <= 0;
	rd_beat <= 0;
	wr_cnt_ps <= 0;
	rd_cnt_ps <= 0;
end
else begin
	if(timer_cn < USR_CLK)
		timer_cn <= timer_cn + 1'b1;
	else
		timer_cn <= 0;
		
	if(timer_cn == USR_CLK)	begin
		wr_beat <= wr_cnt_ps;
		rd_beat <= rd_cnt_ps;
		wr_cnt_ps <= 0;
		rd_cnt_ps <= 0;
	end
	else begin
		if(M_AXI_wvalid & M_AXI_wready & Init_done_r)
			wr_cnt_ps <= wr_cnt_ps + 1'b1;
		if(M_AXI_rvalid)
			rd_cnt_ps <= rd_cnt_ps + 1'b1;
	end
end
/*
wire [35:0] CONTROL0;
	chipscope_ila_64 chipscope_ila_i0 // udp tx
     (
      .CONTROL(CONTROL0), // INOUT BUS [35:0]
      .CLK(clk), // IN
      .TRIG0(
         {
			//13'b0,  M_AXI_arvalid, M_AXI_arready, M_AXI_rvalid, rd_cnt[23:0], M_AXI_rdata[23:0]
			wr_st,  M_AXI_wvalid, M_AXI_wready, M_AXI_rvalid, rd_beat[29:0], wr_beat[29:0]
          }
         ) // IN BUS [191:0]
      );
	 chipscope_icon chipscope_icon_i0
     (
      .CONTROL0(CONTROL0) // INOUT BUS [35:0]
      );
*/
endmodule