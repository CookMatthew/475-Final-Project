----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/28/2021 10:06:45 PM
-- Design Name: 
-- Module Name: modExp - Behavioral
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

entity modExp is
    Port ( rst : in STD_LOGIC;
           c : out STD_LOGIC_VECTOR (31 downto 0);
           b : in STD_LOGIC_VECTOR (31 downto 0);
           e : in STD_LOGIC_VECTOR (31 downto 0);
           done : out STD_LOGIC_VECTOR (31 downto 0);
           clk: in STD_LOGIC);
end modExp;

architecture Behavioral of modExp is

constant modulous : integer := 3233;

signal result : STD_LOGIC_VECTOR (31 downto 0);
signal exp : STD_LOGIC_VECTOR (31 downto 0);
signal expRef : STD_LOGIC_VECTOR (31 downto 0);
signal base : STD_LOGIC_VECTOR (31 downto 0);
signal baseRef : STD_LOGIC_VECTOR (31 downto 0);
signal state : STD_LOGIC_VECTOR (1 downto 0);

begin

process(clk)
begin
    if(rising_edge(clk)) then
        if(rst = '0') then
            done <= std_logic_vector(to_unsigned(1,done'length));
            result <= std_logic_vector(to_unsigned(0,result'length));
            exp <= std_logic_vector(to_unsigned(0,exp'length));
            expRef <= std_logic_vector(to_unsigned(0,exp'length));
            base <= std_logic_vector(to_unsigned(0,base'length));
            baseRef <= std_logic_vector(to_unsigned(0,baseRef'length));
            state <= std_logic_vector(to_unsigned(0,state'length));
        elsif((b /= baseRef) or (e /= expRef)) then
            done <= std_logic_vector(to_unsigned(0,done'length));
            base <= std_logic_vector(unsigned(b) mod modulous);
            baseRef <= b;
            exp <= e;
            expRef <= e;
            result <= std_logic_vector(to_unsigned(1,result'length));
        else
            if(exp /= std_logic_vector(to_unsigned(0,exp'length))) then
                    if(exp(0) = '1') then
                        result <= std_logic_vector(resize((unsigned(base)*unsigned(result)) mod modulous,result'length));
                    end if;
                    exp <= std_logic_vector(unsigned(exp) srl 1);
                    base <= std_logic_vector(resize((unsigned(base) * unsigned(base)) mod modulous,base'length));
            else
                done <= std_logic_vector(to_unsigned(1,done'length));
            end if;
        end if;
    end if;
end process;

c <= result;

end Behavioral;
