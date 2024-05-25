-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Hana Li?ka?ov· (xliskah00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;




entity UART_RX_FSM is
    port(
        --vstupy
       CLK         : in std_logic; --hodinov˝ sign·l
       RST         : in std_logic; --resetov˝ sign·l
       DIN         : in std_logic; --input bit
       CLK_COUNTER : in std_logic_vector(4 downto 0); --po?Ìta? n·b?hovÈ hrany clk
       BIT_COUNTER : in std_logic_vector(3 downto 0); --po?Ìta? p?ijmut˝ch bit?
       --v˝stupy
       VALIDATE_A  : out std_logic; --validate_allow - kontrola ?i je v stavu VALIDATE_DATA 
       CLK_A       : out std_logic; --validate_allow - kontrola ?çi aktivn?õ pracuje s CLK sign·lem - stavy WAIT_FOR_FIRST_BIT, RECIEVE_BIT, WAIT_FOR_STOP_BIT
       RECIEVE_A   : out std_logic --recieve_allow - kontrola ?çi p?ijÌm· data - stav RECIEVE_BIT
    );
end entity;



architecture behavioral of UART_RX_FSM is
    type state_type is (WAIT_FOR_START_BIT, WAIT_FOR_FIRST_BIT, RECIEVE_BIT, WAIT_FOR_STOP_BIT, VALIDATE_DATA);
    signal current_state : state_type := WAIT_FOR_START_BIT; --default CURRENT_state = WAIT_FOR_START_BIT
    signal next_state :state_type := WAIT_FOR_FIRST_BIT; --n·sledujÌcÌ stav
    
begin

    --Definov·nÌ logickÈho fungov·nÌ fsm

    nstate_logic: process(current_state, CLK_COUNTER, BIT_COUNTER, DIN)
    begin
        case current_state is
            when WAIT_FOR_START_BIT => 
                if DIN = '0' then --zad·n start bit, DIN = 0
                    next_state <= WAIT_FOR_FIRST_BIT; --posunutÌ≠ do dal?Ìho stavu
                end if;
            when WAIT_FOR_FIRST_BIT => 
                if CLK_COUNTER = "10111" then --pokud prob?õhlo 23 CLK cykl? (trefenÌ≠ mid bitu DIN, CLK_COUNTER = 23
                    next_state <= RECIEVE_BIT;
                end if;
            when RECIEVE_BIT => 
                if BIT_COUNTER ="1000" then --pokud ji? bylo p?ôijato v?ech 8 bit?, BIT_COUNTER = 8
                    next_state <= WAIT_FOR_STOP_BIT;
                end if;
            when WAIT_FOR_STOP_BIT =>
                if DIN = '1' then --byl na?çten stop bit, DIN = 1
                    if CLK_COUNTER = "0111" then --zaji?t?nÌ≠ mid bitu, AND CLK_COUNTER = 15
                        next_state <= VALIDATE_DATA;
                    end if;
                end if;
            when VALIDATE_DATA =>
                next_state <= WAIT_FOR_START_BIT; --n·vrat do default stavu
            when others => null; --o?et?enÌ mo?nostÌ jin˝ch stav?
        end case;

    end process  nstate_logic;

    pstatereg: process (RST, CLK)
    begin
        if RST='1' then
            current_state <= WAIT_FOR_START_BIT;
        elsif (rising_edge(CLK)) then
            current_state <= next_state;
        end if;
        
    end process pstatereg;

    --output process
    output_logic: process (current_state)
    begin
        --nastavenÌ moorov˝ch v˝stup?
        case current_state is
            when WAIT_FOR_START_BIT =>
                RECIEVE_A <= '0';
                CLK_A <= '0';
                VALIDATE_A <= '0';
            when WAIT_FOR_FIRST_BIT =>
                RECIEVE_A <= '0';
                CLK_A <= '1';
                VALIDATE_A <= '0';
            when RECIEVE_BIT =>
                RECIEVE_A <= '1';
                CLK_A <= '1';
                VALIDATE_A <= '0';
            when WAIT_FOR_STOP_BIT =>
                RECIEVE_A <= '0';
                CLK_A <= '1';
                VALIDATE_A <= '0';
            when VALIDATE_DATA =>
                RECIEVE_A <= '0';
                CLK_A <= '0';
                VALIDATE_A <= '1';
            when others => null;
        end case;
    end process output_logic;
    
end architecture;
