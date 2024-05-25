-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Name Surname (xliskah00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
    --zavedenÃ­ signÃ¡lÅ¯
    signal clk_counter : std_logic_vector(4 downto 0);
    signal bit_counter : std_logic_vector(3 downto 0);
    signal recieve_a   : std_logic;
    signal clk_a       : std_logic;
    signal validate_a  : std_logic;
    --signal bit_counter_signal : std_logic; --pomocnÃ½ signÃ¡l jako vstup pro bit_counter, vznikÃ¡ za and hradlem
    --pokud je clk_counter 15 a recieve_a = 1, je roven 1
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        --pÅ™emapovÃ¡nÃ­ z fsm na zbytek obvodu
        DIN => DIN,
        CLK_COUNTER => clk_counter,
        BIT_COUNTER => bit_counter,
        RECIEVE_A => recieve_a,
        VALIDATE_A => validate_a,
        CLK_A => clk_a

    );

    --zápis DOUT_VLD do registru

    dout_vdl_to_register: process (CLK, RST)
    begin
        if (RST='1') then
            DOUT_VLD <= '0';
        elsif (rising_edge(CLK)) then
            DOUT_VLD <= validate_a; --do výstupu DOUT_VLD se zapí?e hodnota signálu validate_a
        end if;

    end process dout_vdl_to_register;

    --proces pro clk_counter
    clk_counter_process: process (RST, CLK, clk_a, recieve_a)
    begin
        if (RST = '1') then
            clk_counter <= "00001";
        elsif (rising_edge(CLK)) then
            if (clk_a = '0') then
                clk_counter <= "00001"; --vynulování clk counteru v moment? kdy není pot?eba, nastavení do 1 aby se snímalo skute?n? v mid bitu a ne 9. clk signálu
            else --clk_a = 1
                clk_counter <= clk_counter + 1; --inkrementace clk_counteru
                --implementace cmp a and hradla
                if  (recieve_a = '1') then --p?ijíma? zrovna akceptuje bity
                    --po 15 clk cyklech se clk_counter vynuluje - dosáhne mid bitu, incrementuje se bit_counter
                    if (clk_counter > "01111") then
                        clk_counter <= "00001";
                        
                    end if;
                    
                end if;
                
            end if;
            
        end if;
        
    end process clk_counter_process;

    --proces pro bit_counter
    bit_counter_process: process (CLK, RST, recieve_a, clk_counter)
    begin
        if (RST = '1') then
            bit_counter <= "0000";
        elsif (rising_edge(CLK)) then
            if (recieve_a = '0') then
                bit_counter <= "0000"; --vynulování, kdy? counter nepou?ívá
            
            --hradlo and a cmp
            else --recieve_a = 1
                if (clk_counter = "01111") then --clk_counter = 15, dosa?eno mid_bitu
                    bit_counter <= bit_counter + 1; --inkrementace bit_counteru
                    
                end if;

                if (bit_counter = "1000") then -- na?teny v?echny bity
                    bit_counter <= "0000";
                end if;
                
                
            end if;

        end if;

        
    end process bit_counter_process;






    --proces dekodéru a zápis do registr?
    decoder_process: process (CLK, RST)
    begin
        if (RST = '1') then --reset
            DOUT <= "00000000";
        elsif (rising_edge(CLK)) and (recieve_a = '1') then --pokud p?ijí­má data
            if (clk_counter >"01111") then --aby p?epsání výstupu po?kalo skute?n? do mid bitu a nedocházelo k n?mu p?i zm?nách DIN
                
            
                case bit_counter is
                    when "0000" => DOUT(0) <= DIN;
                    when "0001" => DOUT(1) <= DIN;
                    when "0010" => DOUT(2) <= DIN;
                    when "0011" => DOUT(3) <= DIN;
                    when "0100" => DOUT(4) <= DIN;
                    when "0101" => DOUT(5) <= DIN;
                    when "0110" => DOUT(6) <= DIN;
                    when "0111" => DOUT(7) <= DIN;
                    when others => null;
                end case;  
            end if; 
        end if; 
    end process decoder_process;

end architecture;
