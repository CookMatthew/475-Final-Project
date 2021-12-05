----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/28/2021 10:08:48 PM
-- Design Name: 
-- Module Name: modExpSim - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity modExpSim is
--  Port ( );
end modExpSim;

architecture Behavioral of modExpSim is

signal rst : STD_LOGIC;
signal c : STD_LOGIC_VECTOR (31 downto 0);
signal b : STD_LOGIC_VECTOR (31 downto 0);
signal e : STD_LOGIC_VECTOR (31 downto 0);
signal clk : STD_LOGIC;
signal done : STD_LOGIC_VECTOR (31 downto 0);

begin

UUT : entity work.modExp(behavioral)
    port map(
        rst => rst,
        c => c,
        b => b,
        e => e,
        clk => clk,
        done => done);

process
begin

rst <= '0';
clk <= '0';
wait for 10 ns;
clk <= '1';
wait for 10 ns;
clk <= '0';
rst <= '1';
wait for 10 ns;
b <= std_logic_vector(to_unsigned(57,b'length));
e <= std_logic_vector(to_unsigned(17,e'length));
wait for 10 ns;
clk <= '1';
wait for 10 ns;
for i in 0 to 120 loop
    clk <= '0';
    wait for 10 ns;
    clk <= '1';
    wait for 10 ns;
end loop;

end process;
end Behavioral;
