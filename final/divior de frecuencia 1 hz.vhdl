library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity freq_div is

	port ( in_clk: in std_logic;
			out_clk: out std_logic
	);
end freq_div;

architecture descripcion of freq_div is

signal cuenta: std_logic_vector(24 downto 0);
signal interno: std_logic;

begin 
	process (in_clk)
	begin
		if rising_edge(in_clk) then
			if cuenta = 25000000 then
				cuenta <= (others => '0');
				interno <= not interno;
			else
				cuenta <= cuenta + 1;
			end if;
		end if;
	end process;
	out_clk <= interno;
	
end descripcion;
		