`define RES_1080p

module video_ctrl_1080x1920(
input             rst_n                ,
input             clk                  ,
input             fhd                  ,
input             hd,                  
input             sd480p               ,
input             sd576p               ,
output reg        hsync_n              ,
output reg        hsync_n_ahead        ,
output reg        vsync_n              ,
output reg        de                   ,
output reg        is_next_pixel_active ,
output reg        y_valid              ,
output reg [12:0] next_x               ,
output reg [12:0] next_y               ,
output reg [12:0] hcnt                 ,
output reg [12:0] vcnt                 
);

wire [7:0]        H_FRONT       = 8'd60                      ;
wire [7:0]        H_SYNC        = 8'd11                      ;
wire [7:0]        H_BACK        = 8'd60                      ;
wire [11:0]       H_ACT	        = 12'd1200                   ;
wire [12:0]       H_BLANK_END   = H_FRONT+H_SYNC+H_BACK      ;
wire [12:0]       H_PERIOD      = H_FRONT+H_SYNC+H_BACK+H_ACT;  //88+44+148+1920=2200 //(2200*1245)/(1920+45)~35+40+119+1200

wire [7:0]        V_FRONT       = 8'd10                      ; 
wire [7:0]        V_SYNC        = 8'd3                       ; 
wire [7:0]        V_BACK        = 8'd7                       ; 
wire [11:0]       V_ACT	        = 12'd1920                   ;
wire [12:0]       V_BLANK_END   = V_FRONT+V_SYNC+V_BACK      ;
wire [12:0]       V_PERIOD      = V_FRONT+V_SYNC+V_BACK+V_ACT; //4+5+36+1080=1125

//hsync_n
always @ (posedge clk or negedge rst_n)
begin
	if(!rst_n)
	begin
		hcnt <= 0;
		hsync_n <= 0;
		hsync_n_ahead <= 1'b0;
	end
	else
	begin
		if(hcnt<H_PERIOD-1)
			hcnt <= hcnt + 1;  
		else 
			hcnt <= 0;
		if (hcnt == H_FRONT-1)
			hsync_n <= 1'b1;
		else if (hcnt == H_FRONT+H_SYNC-1)
			hsync_n <= 1'b0;
			
		if(hcnt == 1)
			hsync_n_ahead <= 1'b1;
		else if (hcnt == H_FRONT+H_SYNC-1)
			hsync_n_ahead <= 1'b0;	
	end
end
  
 
//vsync_n
always @ (posedge clk or negedge rst_n)
begin
	if(!rst_n)
	begin
		vcnt <=  0;
		vsync_n <= 0;
	end
	else
	begin 
		if (hcnt == H_PERIOD-1)
		begin
			if(vcnt<V_PERIOD-1)
				vcnt <= vcnt + 1;  
			else 
				vcnt <= 0;
			if (vcnt == V_FRONT-1)
				vsync_n <= 1'b1;
			else if (vcnt == V_FRONT+V_SYNC-1)
				vsync_n <= 1'b0;
		end
	end
end
    
always @ (posedge clk or negedge rst_n)
if (!rst_n) begin
	is_next_pixel_active <= 0;
	de <= 0;
	y_valid <= 0;
end 
else begin
	if (vcnt >= V_BLANK_END) begin
		if (hcnt == H_BLANK_END - 2)
			is_next_pixel_active <= 1'b1;
		else if (hcnt == H_PERIOD - 2)
			is_next_pixel_active <= 1'b0;
	end
	y_valid <= vcnt >= V_BLANK_END;
	de <= is_next_pixel_active;
end
	
always @ (posedge clk or negedge rst_n)
if (!rst_n) begin
	next_x <= 0;
	next_y <= 0;
end
else begin
	if (hcnt < H_BLANK_END - 1)
		next_x <= 0;
	else
		next_x <= next_x + 1'b1;
	
	if (vcnt < V_BLANK_END)
		next_y <= 0;
	else if (hcnt == H_PERIOD-1)
		next_y <= next_y + 1'b1;
end

endmodule
   
     
    

