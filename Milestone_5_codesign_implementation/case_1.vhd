

library ieee;
use ieee.std_logic_1164.all;
entity Grid1 is
    
    port(
        A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P  : in bit;
        x,Y, Z: out bit); 
end entity;

architecture Grid1_arch of Grid1 is 
begin
X <='1' when (A='1' and B='1' and C='1' and D='1' and E='0'and F='0' and G='0' and H='0' and I='0' and J='0'and K='0'and L='0'and M='0'and N='0'and P='0')  else
     '0';
Y <='1' when (A='0' and B='0' and C='0' and D='0' and E='0'and F='0' and G='0' and H='0' and I='0' and J='0'and K='0'and L='0'and M='0'and N='0'and P='1')  else
     '0';
Z <='1' when (A='0' and B='0' and C='0' and D='0' and E='0'and F='0' and G='0' and H='0' and I='0' and J='0'and K='0'and L='0'and M='1'and N='0'and P='0')  else
     '0';
 end architecture;