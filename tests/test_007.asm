            .ORIG x2000
            JSR     main                  
            HALT
      main: ADD     R6    , R6    , #-1    ; reserve space for the return value
            ADD     R6    , R6    , #-1   
            STR     R7    , R6    , #0     ; save caller's return address
            ADD     R6    , R6    , #-1   
            STR     R5    , R6    , #0     ; save caller's frame pointer
            ADD     R5    , R6    , #-1    ; set up frame pointer
            ADD     R6    , R6    , #-122  ; decrement stack pointer beyond frame
            SET     R14   , #4           
            ADD     R11   , R14   , #0    
            SET     R15   , #10          
            ADD     R12   , R15   , #0    
            SET     R16   , #13          
            ADD     R13   , R16   , #0    
            SET     R17   , #0           
            ADD     R8    , R17   , #0    
        L2: ADD     R18   , R8    , #0     ; enter for loop
            SET     R19   , #100         
            SUB     R20   , R18   , R19   
            BRn     R20   , L5          
            SET     R20   , #0           
        L5: NOP                            ; less-than label
            BRz     R20   , L4          
            ADD     R21   , R8    , #0    
            ADD     R9    , R21   , #0    
        L6: ADD     R22   , R9    , #0     ; enter for loop
            ADD     R23   , R8    , #0    
            ADD     R24   , R13   , #0    
            ADD     R25   , R23   , R24   
            SUB     R26   , R22   , R25   
            BRn     R26   , L9          
            SET     R26   , #0           
        L9: NOP                            ; less-than label
            ADD     R27   , R9    , #0    
            SET     R28   , #100         
            SUB     R29   , R27   , R28   
            BRn     R29   , L10         
            SET     R29   , #0           
       L10: NOP                            ; less-than label
            ANDL    R30   , R26   , R29   
            BRz     R30   , L8          
            ADD     R31   , R9    , #0    
            ADD     R10   , R31   , #0    
       L11: ADD     R32   , R10   , #0     ; enter for loop
            ADD     R33   , R9    , #0    
            ADD     R34   , R11   , #0    
            ADD     R35   , R33   , R34   
            SUB     R36   , R32   , R35   
            BRn     R36   , L14         
            SET     R36   , #0           
       L14: NOP                            ; less-than label
            ADD     R37   , R10   , #0    
            SET     R38   , #100         
            SUB     R39   , R37   , R38   
            BRn     R39   , L15         
            SET     R39   , #0           
       L15: NOP                            ; less-than label
            ANDL    R40   , R36   , R39   
            BRz     R40   , L13         
            ADD     R41   , R5    , #-121 
            ADD     R42   , R10   , #0    
            MUL     R43   , R42   , #1    
            ADD     R44   , R41   , R43   
            LDR     R45   , R44   , #0    
            ADD     R46   , R8    , #0    
            ADD     R47   , R45   , R46   
            ADD     R48   , R9    , #0    
            SET     R49   , #3           
            DIV     R50   , R48   , R49   
            SUB     R51   , R47   , R50   
            ADD     R52   , R13   , #0    
            ADD     R53   , R51   , R52   
            ADD     R54   , R5    , #-121 
            ADD     R55   , R10   , #0    
            MUL     R56   , R55   , #1    
            ADD     R57   , R54   , R56   
            STR     R53   , R57   , #0    
       L12: ADD     R58   , R10   , #0    
            SET     R59   , #1           
            ADD     R60   , R58   , R59   
            ADD     R10   , R60   , #0    
            BRA     L11                    ; backedge of for-loop
       L13: NOP                           
        L7: ADD     R61   , R9    , #0    
            SET     R62   , #1           
            ADD     R63   , R61   , R62   
            ADD     R9    , R63   , #0    
            BRA     L6                     ; backedge of for-loop
        L8: NOP                           
        L3: ADD     R64   , R8    , #0    
            SET     R65   , #1           
            ADD     R66   , R64   , R65   
            ADD     R8    , R66   , #0    
            BRA     L2                     ; backedge of for-loop
        L4: NOP                           
            ADD     R67   , R5    , #-121 
            SET     R68   , #50          
            MUL     R69   , R68   , #1    
            ADD     R70   , R67   , R69   
            LDR     R71   , R70   , #0    
            ADD     R11   , R71   , #0    
            ADD     R72   , R11   , #0    
            ADD     R0    , R72   , #0    
            OUT
            SET     R73   , #0           
            STR     R73   , R5    , #3    
            BRA     L1                    
        L1: ADD     R6    , R6    , #122   ; remove locals from stack
            LDR     R5    , R6    , #0     ; restore caller's frame pointer
            ADD     R6    , R6    , #1    
            LDR     R7    , R6    , #0     ; restore caller's return address
            ADD     R6    , R6    , #1    
            RET                            ; return to caller
