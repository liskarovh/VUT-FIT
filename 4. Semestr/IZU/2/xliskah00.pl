% Zadani c. 24:
% Napiste program resici ukol dany predikatem u24(LIN,VIN1,VIN2,VOUT), kde 
% LIN je vstupni ciselny seznam, promenne VIN1 a VIN2 obsahuji cisla 
% splnujici podminku VIN1>VIN2 a VOUT je promenna, ve ktere se vraci prvni 
% cislo seznamu LIN splnujici podminku VIN1>VOUT>VIN2. Pokud zadne takove 
% cislo neexistuje je predikat nepravdivy (vraci hodnotu false).  

% Testovaci predikaty:                         			% VOUT        
u24_1:- u24([15,2,4,9,12,17],10,2,VOUT),write(VOUT).		% 4
u24_2:- u24([15,2,-14,9,12,17],10,2,VOUT),write(VOUT).		% 9
u24_3:- u24([-10,-20.8,-5.3,0,7],0,-10,VOUT),write(VOUT).	% -5.3
u24_r:- write('Zadej LIN: '),read(LIN),
        write('Zadej VIN1: '),read(VIN1),
        write('Zadej VIN2: '),read(VIN2),
        u24(LIN,VIN1,VIN2,LOUT),write(LOUT).

% Reseni:
u24(LIN,VIN1,VIN2,VOUT):-
    ( LIN = [FIRST|TAIL] ->
        ( FIRST < VIN1,
          FIRST > VIN2 ->
              VOUT = FIRST
          ;
              u24(TAIL, VIN1, VIN2, VOUT)
        )
    ;
        false
    ).
