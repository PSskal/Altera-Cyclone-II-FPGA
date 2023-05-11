library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity control_8horas is
port(
	xtal_50M, reset: in std_logic;
	luces: out std_logic_vector(1 downto 0);
	pulsadores: in std_logic_Vector(7 downto 0);
	lcd_rs, lcd_rw, lcd_e: out std_logic;
	lcd_data: inout std_logic_Vector(7 downto 0)
);
end control_8horas;

architecture estructural of control_8horas is

signal clk_1hz: std_logic;

	component freq_div is
		port ( clk50mhz: 	in STD_LOGIC;
					clk:		out STD_LOGIC);
	end component freq_div;

		  
    component covid is
        port (
            clk_clk        : in    std_logic                    := 'X';             -- clk
            botones_export : in    std_logic_vector(7 downto 0) := (others => 'X'); -- export
            clk_1hz_export : in    std_logic                    := 'X';             -- export
            lcd1602_RS     : out   std_logic;                                       -- RS
            lcd1602_RW     : out   std_logic;                                       -- RW
            lcd1602_data   : inout std_logic_vector(7 downto 0) := (others => 'X'); -- data
            lcd1602_E      : out   std_logic;                                       -- E
            reset_reset_n  : in    std_logic                    := 'X';             -- reset_n
            led_export     : out   std_logic_vector(1 downto 0)                     -- export
        );
    end component covid;
begin
    u0 : component covid
        port map (
            clk_clk        => xtal_50M,        --     clk.clk
            botones_export => pulsadores, -- botones.export
            clk_1hz_export => clk_1hz, -- clk_1hz.export
            lcd1602_RS     => lcd_rs,     -- lcd1602.RS
            lcd1602_RW     => lcd_rw,     --        .RW
            lcd1602_data   => lcd_data,   --        .data
            lcd1602_E      => lcd_e,      --        .E
            reset_reset_n  => reset,  --   reset.reset_n
            led_export     => luces     --     led.export
        );
		  
	 u1: component freq_div
	 
			port map( 
				clk50mhz => xtal_50M,
				clk => clk_1hz
				);
				
end estructural;