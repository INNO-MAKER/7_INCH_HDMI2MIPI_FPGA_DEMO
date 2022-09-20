`timescale 1ns / 1ps
/* -------------------------------------------------------------------------
** This confidential and proprietary software may be only used as authorized
** by a licensing agreement from Tim.Yep (samssmarm@gmail.com). If you are 
** not an authorized user, please destroy this source code file and notify 
** Tim.Yep (samssmarm@gmail.com) immediately that you inadvertently received 
** an unauthorized copy.
** -------------------------------------------------------------------------
** Copyright (c) 2017 Tim.Yep ALL RIGHTS RESERVED
** Filename				:		sp605_ddr_test.v
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

module sp605_ddr_test #
(
parameter C3_P0_MASK_SIZE           = 4         ,
parameter C3_P0_DATA_PORT_SIZE      = 32        ,
parameter C3_P1_MASK_SIZE           = 4         ,
parameter C3_P1_DATA_PORT_SIZE      = 32        ,
parameter DEBUG_EN                  = 0         ,       
                                                          // # = 1, Enable debug signals/controls,
                                                          //   = 0, Disable debug signals/controls.
parameter C3_MEMCLK_PERIOD          = 2500      ,                    
                                                          // Memory data transfer clock period
parameter C3_CALIB_SOFT_IP          = "TRUE"    ,                    
                                                          // # = TRUE, Enables the soft calibration logic,
                                                          // # = FALSE, Disables the soft calibration logic.
parameter C3_SIMULATION             = "FALSE"   ,                 
                                                          // # = TRUE, Simulating the design. Useful to reduce the simulation time,
                                                          // # = FALSE, Implementing the design.
parameter C3_RST_ACT_LOW            = 0         ,                 
                                                          // # = 1 for active low reset,
                                                          // # = 0 for active high reset.
parameter C3_INPUT_CLK_TYPE         = "SINGLE_ENDED",       
                                                          // input clock type DIFFERENTIAL or SINGLE_ENDED
parameter C3_MEM_ADDR_ORDER         = "ROW_BANK_COLUMN",       
                                                          // The order in which user address is provided to the memory controller,
                                                          // ROW_BANK_COLUMN or BANK_ROW_COLUMN
parameter C3_NUM_DQ_PINS            = 16        ,       
                                                          // External memory data width
parameter C3_MEM_ADDR_WIDTH         = 13        ,       
                                                          // External memory address width
parameter  C3_MEM_BANKADDR_WIDTH    =  3        ,
                                                          //  External  memory  bank  address  width
parameter C3_S0_AXI_STRICT_COHERENCY       =  0 ,
parameter C3_S0_AXI_ENABLE_AP              =  1 ,
parameter C3_S0_AXI_DATA_WIDTH             =  32,
parameter C3_S0_AXI_SUPPORTS_NARROW_BURST  =  0 ,
parameter C3_S0_AXI_ADDR_WIDTH             =  32,
parameter C3_S0_AXI_ID_WIDTH               =  4 ,
parameter C3_S0_AXI_SUPPORTS_READ	       =  1 ,
parameter C3_S0_AXI_SUPPORTS_WRITE	       =  1 ,
parameter C3_S0_AXI_ENABLE                 =  1
)	

(

inout  [C3_NUM_DQ_PINS-1:0]                         mcb3_dram_dq     ,
output [C3_MEM_ADDR_WIDTH-1:0]                      mcb3_dram_a      ,
output [C3_MEM_BANKADDR_WIDTH-1:0]                  mcb3_dram_ba     ,
output                                              mcb3_dram_ras_n  ,
output                                              mcb3_dram_cas_n  ,
output                                              mcb3_dram_we_n   ,
output                                              mcb3_dram_odt    ,
output                                              mcb3_dram_reset_n,
output                                              mcb3_dram_cke    ,
output                                              mcb3_dram_dm     ,
inout                                               mcb3_dram_udqs   ,
inout                                               mcb3_dram_udqs_n ,
inout                                               mcb3_rzq         ,
inout                                               mcb3_zio         ,
output                                              mcb3_dram_udm    ,
input                                               c3_sys_clk       ,
input                                               c3_sys_rst_i     ,
//output                                            c3_calib_done    ,
inout                                               mcb3_dram_dqs    ,
inout                                               mcb3_dram_dqs_n  ,
output                                              mcb3_dram_ck     ,
output                                              mcb3_dram_ck_n   ,

output  reg                                         mcu_irq          ,
                                                                     
output  reg [23:0]                                  rgb_pdata		 ,
output  reg                                         rgb_hsync		 ,
output  reg                                         rgb_vsync		 ,
output  reg                                         rgb_de			 ,
output		                                        rgb_pclk	     ,
                                                                     
input                                               rxa_clk_p        ,
input                                               rxa_clk_n        ,
input                                               rxa_d0_p	     ,
input                                               rxa_d0_n	     ,
input                                               rxa_d1_p	     ,
input                                               rxa_d1_n	     ,
input                                               rxa_d2_p	     ,
input                                               rxa_d2_n	     ,
input                                               rxa_d3_p	     ,
input                                               rxa_d3_n	     ,
input                                               rxb_d0_p	     ,
input                                               rxb_d0_n	     ,
input                                               rxb_d1_p	     ,
input                                               rxb_d1_n	     ,
input                                               rxb_d2_p	     ,
input                                               rxb_d2_n	     ,
input                                               rxb_d3_p	     ,
input                                               rxb_d3_n	     ,
input                                               SPI_CS1          ,
input                                               SPI_CS0          ,
input                                               SPI_MOSI         ,
output                                              SPI_MISO         ,
input                                               SPI_CLK          ,

input                                               sf_miso          ,
output                                              sf_mosi          ,
output                                              sf_ce            ,
output                                              sf_sclk                
//output											heartbeat
);

localparam x_res = 1920, y_res = 1200       ;	
localparam WIDTH = 64                       ;  

wire                    ipu_clk             ;
wire                    ipu_rst             ;
wire                    disp_clk            ;
wire                    disp_rst            ;
wire                    disp_clk_int        ;
wire                    disp_rst_int        ;
wire                    disp_clk_ok         ;
   
wire    [63:0]          gpio_0_tri_i        ;
wire    [63:0]          gpio_0_tri_o        ;
wire    [63:0]          gpio_0_tri_t        ;
                                            
wire                    mem_clk             ;
wire                    mem_rst             ;
wire                    bus_clk             ;
wire                    bus_rst             ;
wire                    pclk                ;
wire                    pclk_rst            ;

wire    [31:0]          S_AXI_HP0_araddr    ;
wire    [1:0]           S_AXI_HP0_arburst   ;
wire    [3:0]           S_AXI_HP0_arcache   ;
wire    [5:0]           S_AXI_HP0_arid      ;
wire    [7:0]           S_AXI_HP0_arlen     ;
wire    [1:0]           S_AXI_HP0_arlock    ;
wire    [2:0]           S_AXI_HP0_arprot    ;
wire    [3:0]           S_AXI_HP0_arqos     ;
wire                    S_AXI_HP0_arready   ;
wire    [2:0]           S_AXI_HP0_arsize    ;
wire                    S_AXI_HP0_arvalid   ;
wire    [31:0]          S_AXI_HP0_awaddr    ;
wire    [1:0]           S_AXI_HP0_awburst   ;
wire    [3:0]           S_AXI_HP0_awcache   ;
wire    [5:0]           S_AXI_HP0_awid      ;
wire    [3:0]           S_AXI_HP0_awlen     ;
wire    [1:0]           S_AXI_HP0_awlock    ;
wire    [2:0]           S_AXI_HP0_awprot    ;
wire    [3:0]           S_AXI_HP0_awqos     ;
wire                    S_AXI_HP0_awready   ;
wire    [2:0]           S_AXI_HP0_awsize    ;
wire                    S_AXI_HP0_awvalid   ;
wire    [5:0]           S_AXI_HP0_bid       ;
wire                    S_AXI_HP0_bready    ;
wire    [1:0]           S_AXI_HP0_bresp     ;
wire                    S_AXI_HP0_bvalid    ;
wire    [WIDTH-1:0]     S_AXI_HP0_rdata     ;
wire    [5:0]           S_AXI_HP0_rid       ;
wire                    S_AXI_HP0_rlast     ;
wire                    S_AXI_HP0_rready    ;
wire    [1:0]           S_AXI_HP0_rresp     ;
wire                    S_AXI_HP0_rvalid    ;
wire    [WIDTH-1:0]     S_AXI_HP0_wdata     ;
wire    [5:0]           S_AXI_HP0_wid       ;
wire                    S_AXI_HP0_wlast     ;
wire                    S_AXI_HP0_wready    ;
wire    [WIDTH/8-1:0]   S_AXI_HP0_wstrb     ;
wire                    S_AXI_HP0_wvalid    ;

wire    [31:0]          mem_araddr          ;
wire    [1:0]           mem_arburst         ;
wire    [3:0]           mem_arcache         ;
wire    [5:0]           mem_arid            ;
wire    [7:0]           mem_arlen           ;
wire    [1:0]           mem_arlock          ;
wire    [2:0]           mem_arprot          ;
wire    [3:0]           mem_arqos           ;
wire                    mem_arready         ;
wire    [2:0]           mem_arsize          ;
wire                    mem_arvalid         ;
wire    [31:0]          mem_awaddr          ;
wire    [1:0]           mem_awburst         ;
wire    [3:0]           mem_awcache         ;
wire    [5:0]           mem_awid            ;
wire    [7:0]           mem_awlen           ;
wire    [1:0]           mem_awlock          ;
wire    [2:0]           mem_awprot          ;
wire    [3:0]           mem_awqos           ;
wire                    mem_awready         ;
wire    [2:0]           mem_awsize          ;
wire                    mem_awvalid         ;
wire    [5:0]           mem_bid             ;
wire                    mem_bready          ;
wire    [1:0]           mem_bresp           ;
wire                    mem_bvalid          ;
wire    [WIDTH-1:0]     mem_rdata           ;
wire    [5:0]           mem_rid             ;
wire                    mem_rlast           ;
wire                    mem_rready          ;
wire    [1:0]           mem_rresp           ;
wire                    mem_rvalid          ;
wire    [WIDTH-1:0]     mem_wdata           ;
wire    [5:0]           mem_wid             ;
wire                    mem_wlast           ;
wire                    mem_wready          ;
wire    [WIDTH/8-1:0]   mem_wstrb           ;
wire                    mem_wvalid          ;

wire                    sync_err            ;
wire                    rd_underrun         ;
wire                    wr_ovfl             ;	

wire                    Init_Done           ;
wire                    xfer_en             ;

wire    [23:0]          i_pdata            ;
wire                    i_hsync            ;
wire                    i_vsync            ;
wire                    i_de               ;

wire    [23:0]          o_pdata            ;
wire                    o_hsync            ;
wire                    o_vsync            ;
wire                    o_de               ;

wire                    c3_clk0            ;
wire                    c3_rst0            ;
wire                    c3_calib_done      ;
reg     [2:0]           calib_done_r       ;

wire                    vld                ;
wire    [55:0]          dna_id             ;

wire                    deser_clk          ;
wire    [55:0]          deser_data         ;
reg     [7:0]           dser_cnt           ;

wire    [23:0]          lvds_fdata         ;
wire    [23:0]          lvds_sdata         ;
wire                    lvds_vs            ;
wire                    lvds_hs            ;
wire                    lvds_de            ;
wire                    lvds_rst           ;

wire                    rx_bufpll_lock     ;
wire                    mon_bitslip        ;
wire                    rx_bufg_x1         ;
top_nto1_pll_diff_rx top_nto1_pll_diff_rx_i
(
.reset                 (ipu_rst        ),				// reset (active high)
.datain_p              ({rxa_d3_p, rxa_d2_p, rxa_d1_p, rxa_d0_p, rxb_d3_p, rxb_d2_p, rxb_d1_p, rxb_d0_p}), 
.datain_n              ({rxa_d3_n, rxa_d2_n, rxa_d1_n, rxa_d0_n, rxb_d3_n, rxb_d2_n, rxb_d1_n, rxb_d0_n}),		// lvds data inputs
.clkin_p               (rxa_clk_p      ),  
.clkin_n               (rxa_clk_n      ),		// lvds clock input
.rx_bufg_x1            (deser_clk      ),
.dummy_out             (deser_data     ),
.rx_bufpll_lock        (rx_bufpll_lock ),
.mon_bitslip           (mon_bitslip    )
) ;	

//hold-off logic for clocks ready
    reg [15:0] clocks_ready_count;
    reg clocks_ready;
    always @(posedge deser_clk or posedge c3_sys_rst_i or negedge rx_bufpll_lock) begin
        if (c3_sys_rst_i | !rx_bufpll_lock) begin
            clocks_ready_count <= 16'b0;
            clocks_ready <= 1'b0;
        end
        else if (!clocks_ready) begin
            clocks_ready_count <= clocks_ready_count + 1'b1;
            clocks_ready <= (clocks_ready_count == 16'hffff);
        end
    end

reset_sync bus_sync(.clk(deser_clk), .reset_in(!clocks_ready), .reset_out(lvds_rst));	

always @(posedge deser_clk)
  dser_cnt <= dser_cnt + 1'b1;

assign lvds_de = ~deser_data[20];
assign lvds_hs = ~deser_data[18];
assign lvds_vs = ~deser_data[19];
//assign lvds_fdata = {deser_data[17:14], deser_data[13:12], deser_data[26:25], deser_data[11:7], deser_data[6], deser_data[24:23], deser_data[5:0], deser_data[22:21]};
//                  //<-------------------------------------------------------> <------------------------------------------------>  <-------------------------------->
//                  //                        B                                                          G                                           R
//assign lvds_sdata = {deser_data[45:42], deser_data[41:40], deser_data[54:53], deser_data[39:35], deser_data[34], deser_data[52:51], deser_data[33:28], deser_data[50:49]};

assign lvds_fdata = {deser_data[26:25], deser_data[17:14], deser_data[13:12], deser_data[24:23], deser_data[11:7], deser_data[6], deser_data[22:21], deser_data[5:0]};
                  //<-------------------------------------------------------> <------------------------------------------------>  <-------------------------------->
                  //                        B                                                          G                                           R
assign lvds_sdata = {deser_data[54:53], deser_data[45:42], deser_data[41:40], deser_data[52:51], deser_data[39:35], deser_data[34], deser_data[50:49], deser_data[33:28]};


wire [35:0] CONTROL0;
	chipscope_ila_64 chipscope_ila_i0 // udp tx
     (
      .CONTROL(CONTROL0), // INOUT BUS [35:0]
      .CLK(deser_clk), // IN
      .TRIG0(
         {
			//rx_bufpll_lock, mon_bitslip, dser_cnt[5:0], deser_data[55:0]
			2'b0, wr_ovfl, rd_underrun, sync_err, rx_bufpll_lock, mon_bitslip, lvds_de, lvds_hs, lvds_vs, dser_cnt[5:0], lvds_sdata[23:0], lvds_fdata[23:0]
          }
         ) // IN BUS [191:0]
      );
	 chipscope_icon chipscope_icon_i0
     (
      .CONTROL0(CONTROL0) // INOUT BUS [35:0]
      ); 

assign  mem_clk  = c3_clk0      ;
assign  mem_rst  = c3_rst0      ;
assign  bus_clk  = c3_clk0      ;
assign  bus_rst  = c3_rst0      ;
assign  pclk     = ipu_clk      ;
assign  pclk_rst = ipu_rst      ;

localparam WAIT_DDR_OK  = 1'b0  ;
localparam EN_XFER      = 1'b1  ;
reg        ctrl_sm              ;
reg [15:0] ddr_ok_cnt           ;

always @(posedge ipu_clk)
if(ipu_rst) begin
	ctrl_sm <= WAIT_DDR_OK;
	ddr_ok_cnt <= 0;
end
else begin
	calib_done_r <= {calib_done_r[1:0], c3_calib_done};
	case(ctrl_sm)
	WAIT_DDR_OK : begin
		if(calib_done_r[2])
			ddr_ok_cnt <= ddr_ok_cnt + 1'b1;
		else
			ddr_ok_cnt <= 0;
			
		if(ddr_ok_cnt[15])
			ctrl_sm <= EN_XFER;
	end
	EN_XFER : begin
		ctrl_sm <= EN_XFER;
	end
	endcase
end
assign Init_Done = ctrl_sm == EN_XFER;		 
assign xfer_en	 = ctrl_sm == EN_XFER; 		

mem_rw_test mem_rw_test_i
(
.clk                (mem_clk            ),
.rst                (mem_rst            ),
.Init_done          (Init_Done          ),

.M_AXI_araddr       (mem_araddr         ),
.M_AXI_arburst      (mem_arburst        ),
.M_AXI_arcache      (mem_arcache        ),
.M_AXI_arid         (mem_arid           ),
.M_AXI_arlen        (mem_arlen          ),
.M_AXI_arlock       (mem_arlock         ),
.M_AXI_arprot       (mem_arprot         ),
.M_AXI_arqos        (mem_arqos          ),
.M_AXI_arready      (mem_arready        ),
.M_AXI_arsize       (mem_arsize         ),
.M_AXI_arvalid      (mem_arvalid        ),
.M_AXI_awaddr       (mem_awaddr         ),
.M_AXI_awburst      (mem_awburst        ),
.M_AXI_awcache      (mem_awcache        ),
.M_AXI_awid         (mem_awid           ),
.M_AXI_awlen        (mem_awlen          ),
.M_AXI_awlock       (mem_awlock         ),
.M_AXI_awprot       (mem_awprot         ),
.M_AXI_awqos        (mem_awqos          ),
.M_AXI_awready      (mem_awready        ),
.M_AXI_awsize       (mem_awsize         ),
.M_AXI_awvalid      (mem_awvalid        ),
.M_AXI_bid          (mem_bid            ),
.M_AXI_bready       (mem_bready         ),
.M_AXI_bresp        (mem_bresp          ),
.M_AXI_bvalid       (mem_bvalid         ),
.M_AXI_rdata        (mem_rdata          ),
.M_AXI_rid          (mem_rid            ),
.M_AXI_rlast        (mem_rlast          ),
.M_AXI_rready       (mem_rready         ),
.M_AXI_rresp        (mem_rresp          ),
.M_AXI_rvalid       (mem_rvalid         ),
.M_AXI_wdata        (mem_wdata          ),
.M_AXI_wid          (mem_wid            ),
.M_AXI_wlast        (mem_wlast          ),
.M_AXI_wready       (mem_wready         ),
.M_AXI_wstrb        (mem_wstrb          ),
.M_AXI_wvalid       (mem_wvalid         )
);

 mig_39_2 # (
    .C3_P0_MASK_SIZE(8),
    .C3_P0_DATA_PORT_SIZE(64),
    .C3_P1_MASK_SIZE(8),
    .C3_P1_DATA_PORT_SIZE(64),
    .DEBUG_EN(0),
    .C3_MEMCLK_PERIOD(2500),
    .C3_CALIB_SOFT_IP("TRUE"),
    .C3_SIMULATION("FALSE"),
    .C3_RST_ACT_LOW(0),
    .C3_INPUT_CLK_TYPE("SINGLE_ENDED"),
    .C3_MEM_ADDR_ORDER("ROW_BANK_COLUMN"),
    .C3_NUM_DQ_PINS(16),
    .C3_MEM_ADDR_WIDTH(13),
    .C3_MEM_BANKADDR_WIDTH(3),
    .C3_S0_AXI_STRICT_COHERENCY(0),
    .C3_S0_AXI_ENABLE_AP(1),
    .C3_S0_AXI_DATA_WIDTH(64),
    .C3_S0_AXI_SUPPORTS_NARROW_BURST(0),
    .C3_S0_AXI_ADDR_WIDTH(32),
    .C3_S0_AXI_ID_WIDTH(4)	
)
u_mig_39_2 (

//.c3_sys_clk_p         (c3_sys_clk_p	     ),
//.c3_sys_clk_n         (c3_sys_clk_n	     ),
.c3_sys_clk             (c3_sys_clk		     ),
.c3_sys_rst_i           (c3_sys_rst_i	     ),                        
                                             
.mcb3_dram_dq           (mcb3_dram_dq        ),  
.mcb3_dram_a            (mcb3_dram_a         ),  
.mcb3_dram_ba           (mcb3_dram_ba        ),
.mcb3_dram_ras_n        (mcb3_dram_ras_n     ),                        
.mcb3_dram_cas_n        (mcb3_dram_cas_n     ),                        
.mcb3_dram_we_n         (mcb3_dram_we_n      ),                          
.mcb3_dram_odt          (mcb3_dram_odt       ),
.mcb3_dram_cke          (mcb3_dram_cke       ),                          
.mcb3_dram_ck           (mcb3_dram_ck        ),                          
.mcb3_dram_ck_n         (mcb3_dram_ck_n      ),       
.mcb3_dram_dqs          (mcb3_dram_dqs       ),                           
.mcb3_dram_dqs_n        (mcb3_dram_dqs_n     ),
.mcb3_dram_udqs         (mcb3_dram_udqs      ),  // for X16 parts                        
.mcb3_dram_udqs_n       (mcb3_dram_udqs_n    ),  // for X16 parts
.mcb3_dram_udm          (mcb3_dram_udm       ),  // for X16 parts
.mcb3_dram_dm           (mcb3_dram_dm        ),
.mcb3_dram_reset_n      (mcb3_dram_reset_n   ),
.c3_clk0                (c3_clk0             ),
.c3_rst0                (c3_rst0             ),
.ipu_clk                (ipu_clk             ),
.ipu_rst                (ipu_rst             ),
.disp_clk               (disp_clk_int        ),  
.disp_rst               (disp_rst_int        ),  	
 

.c3_calib_done          (c3_calib_done       ),
.mcb3_rzq               (mcb3_rzq            ),     
.mcb3_zio               (mcb3_zio            ),
                                             
.c3_s0_axi_aclk         (mem_clk             ),
.c3_s0_axi_aresetn      (~mem_rst            ),
.c3_s0_axi_awid         (mem_awid[3:0]       ),
.c3_s0_axi_awaddr       (mem_awaddr          ),
.c3_s0_axi_awlen        (mem_awlen           ),
.c3_s0_axi_awsize       (mem_awsize          ),
.c3_s0_axi_awburst      (mem_awburst         ),
.c3_s0_axi_awlock       (mem_awlock          ),
.c3_s0_axi_awcache      (mem_awcache         ),
.c3_s0_axi_awprot       (mem_awprot          ),
.c3_s0_axi_awqos        (mem_awqos           ),
.c3_s0_axi_awvalid      (mem_awvalid         ),
.c3_s0_axi_awready      (mem_awready         ),
.c3_s0_axi_wdata        (mem_wdata           ),
.c3_s0_axi_wstrb        (mem_wstrb           ),
.c3_s0_axi_wlast        (mem_wlast           ),
.c3_s0_axi_wvalid       (mem_wvalid          ),
.c3_s0_axi_wready       (mem_wready          ),
.c3_s0_axi_bid          (mem_bid             ),
.c3_s0_axi_wid          (mem_wid[3:0]        ),	
.c3_s0_axi_bresp        (mem_bresp           ),
.c3_s0_axi_bvalid       (mem_bvalid          ),
.c3_s0_axi_bready       (mem_bready          ),
.c3_s0_axi_arid         (mem_arid[3:0]       ),
.c3_s0_axi_araddr       (mem_araddr          ),
.c3_s0_axi_arlen        (mem_arlen           ),
.c3_s0_axi_arsize       (mem_arsize          ),
.c3_s0_axi_arburst      (mem_arburst         ),
.c3_s0_axi_arlock       (mem_arlock          ),
.c3_s0_axi_arcache      (mem_arcache         ),
.c3_s0_axi_arprot       (mem_arprot          ),
.c3_s0_axi_arqos        (mem_arqos           ),
.c3_s0_axi_arvalid      (mem_arvalid         ),
.c3_s0_axi_arready      (mem_arready         ),
.c3_s0_axi_rid          (mem_rid[3:0]        ),
.c3_s0_axi_rdata        (mem_rdata           ),
.c3_s0_axi_rresp        (mem_rresp           ),
.c3_s0_axi_rlast        (mem_rlast           ),
.c3_s0_axi_rvalid       (mem_rvalid          ),
.c3_s0_axi_rready       (mem_rready          )
);

video_ctrl_1080x1920 video_ctrl_o_i
(
.clk                    (disp_clk            ),
.rst_n                  (~disp_rst           ),
.hsync_n                (o_hsync             ),
.hsync_n_ahead          (                    ),
.vsync_n                (o_vsync             ),
.de                     (o_de                ),
.is_next_pixel_active   (                    ),
.y_valid                (                    ),                                            
.next_x                 (                    ),
.next_y                 (                    ),
.hcnt                   (                    ),
.vcnt                   (                    )
);

wire  [8:0]             frm_cnt              ;
cnt_frm cnt_frm_i
(
.disp_clk               (disp_clk            ),
.disp_rst               (disp_rst            ),
       
.vsync                  (o_vsync             ),
.frm_cnt                (frm_cnt             )
);

disp_clk_crm disp_clk_crm_i
(
.clk_in                 (disp_clk_int        ),
.rst_in                 (disp_rst_int        ),
                                             
.disp_clk               (disp_clk            ),
.disp_rst               (disp_rst            ),
.disp_clk_ok            (disp_clk_ok         )
);
	
always @(posedge disp_clk) begin
	//rgb_pdata <= o_pdata;
	rgb_pdata <= frm_cnt[8:7] == (2'b00) ? 24'h0000ff :
                 frm_cnt[8:7] == (2'b01) ? 24'h00ff00 :
                 frm_cnt[8:7] == (2'b10) ? 24'hff0000 : 24'h00ffff;
	rgb_hsync <= ~o_hsync;
	rgb_vsync <= ~o_vsync;
	rgb_de    <= o_de;
end

//ODDR2 #(.DDR_ALIGNMENT("C0"),.SRTYPE("ASYNC")) oddr2_rgb_pclk
//     (.Q(rgb_pclk), .C0(pclk), .C1(~pclk),
//      .CE(1'b1), .D0(1'b1), .D1(1'b0), .R(1'b0), .S(1'b0));
ODDR2 #(.DDR_ALIGNMENT("C0"),.SRTYPE("ASYNC")) oddr2_rgb_pclk
     (.Q(rgb_pclk), .C0(disp_clk), .C1(~disp_clk),
      .CE(1'b1), .D0(1'b0), .D1(1'b1), .R(1'b0), .S(1'b0));	

always @(posedge ipu_clk)
    //mcu_irq <= wr_ovfl || rd_underrun || sync_err; 	
    mcu_irq <= 0;  

reg [31:0]	led_cnt	;
reg			led_st	;
always @(posedge mem_clk)
if(mem_rst) begin
  led_cnt <= 0;
  led_st <= 0;
end
else begin
  if(led_cnt < 160000000) //160MHz
    led_cnt <= led_cnt + 1'b1;
  else
    led_cnt <= 0;
  	
  if(led_cnt == 160000000)
    led_st <= ~led_st;
  
end
//assign heartbeat = led_st;


/*
wire [35:0] CONTROL0;
	chipscope_ila_64 chipscope_ila_i0 // udp tx
     (
      .CONTROL(CONTROL0), // INOUT BUS [35:0]
      .CLK(pclk), // IN
      .TRIG0(
         {
			//10'b0, i_vsync, i_hsync, i_de, o_vsync, o_hsync, o_de, Init_Done, xfer_en, chk_rslt, sync_err, rd_underrun, wr_ovfl, o_de_r, exp_val_r[23:0], o_pdata_r[23:0]
			10'b0, lvds_vs, lvds_hs, lvds_de, o_vsync, o_hsync, o_de, Init_Done, xfer_en, chk_rslt, sync_err, rd_underrun, wr_ovfl, o_de_r, exp_val_r[23:0], o_pdata_r[23:0]
			}
         ) // IN BUS [191:0]
      );
	 chipscope_icon chipscope_icon_i0
     (
      .CONTROL0(CONTROL0) // INOUT BUS [35:0]
      ); 
*/
wire   dna_miso = 1'b0;

assign SPI_MISO = SPI_CS0 ? dna_miso : sf_miso;
assign sf_mosi  = SPI_MOSI  ;
assign sf_ce    = SPI_CS0   ;
assign sf_sclk  = SPI_CLK   ;

endmodule
