            .ORIG x2000
            JSR     main                  
            HALT
      main: ADD     R6    , R6    , #-1    ; reserve space for the return value
            ADD     R6    , R6    , #-1   
            STR     R7    , R6    , #0     ; save caller's return address
            ADD     R6    , R6    , #-1   
            STR     R5    , R6    , #0     ; save caller's frame pointer
            ADD     R5    , R6    , #-1    ; set up frame pointer
            ADD     R6    , R6    , #-119  ; decrement stack pointer beyond frame
            SET     R11   , #100         
            ADD     R10   , R11   , #0    
            SET     R12   , #100         
            ADD     R13   , R5    , #-115 
            SET     R14   , #10          
            MUL     R15   , R14   , #1    
            ADD     R16   , R13   , R15   
            LDR     R17   , R16   , #0    
            ADD     R18   , R17   , #0    
            STR     R12   , R18   , #0    
            SET     R19   , #0           
            ADD     R8    , R19   , #0    
            SET     R20   , #0           
            ADD     R9    , R20   , #0    
        L2: ADD     R21   , R9    , #0     ; enter for loop
            SET     R22   , #100         
            SUB     R23   , R21   , R22   
            BRn     R23   , L5          
            SET     R23   , #0           
        L5: NOP                            ; less-than label
            BRz     R23   , L4          
            ADD     R24   , R8    , #0    
            ADD     R25   , R9    , #0    
            SET     R26   , #2           
            MUL     R27   , R25   , R26   
            ADD     R28   , R24   , R27   
            ADD     R8    , R28   , #0    
            ADD     R29   , R9    , #0    
            ADD     R30   , R5    , #-115 
            ADD     R31   , R9    , #0    
            MUL     R32   , R31   , #1    
            ADD     R33   , R30   , R32   
            LDR     R34   , R33   , #0    
            ADD     R35   , R34   , #0    
            STR     R29   , R35   , #0    
        L3: ADD     R36   , R9    , #0    
            SET     R37   , #1           
            ADD     R38   , R36   , R37   
            ADD     R9    , R38   , #0    
            BRA     L2                     ; backedge of for-loop
        L4: NOP                           
            ADD     R39   , R8    , #0    
            ADD     R40   , R10   , #0    
            ADD     R41   , R39   , R40   
            ADD     R8    , R41   , #0    
            ADD     R42   , R10   , #0    
            ADD     R0    , R42   , #0    
            OUT
            ADD     R43   , R8    , #0    
            ADD     R0    , R43   , #0    
            OUT
            ADD     R44   , R9    , #0    
            ADD     R0    , R44   , #0    
            OUT
            SET     R45   , #0           
            STR     R45   , R5    , #3    
            BRA     L1                    
        L1: ADD     R6    , R6    , #119   ; remove locals from stack
            LDR     R5    , R6    , #0     ; restore caller's frame pointer
            ADD     R6    , R6    , #1    
            LDR     R7    , R6    , #0     ; restore caller's return address
            ADD     R6    , R6    , #1    
            RET                            ; return to caller
