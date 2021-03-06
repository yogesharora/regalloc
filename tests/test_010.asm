            .ORIG x2000
            JSR     main                  
            HALT
      main: ADD     R6    , R6    , #-1    ; reserve space for the return value
            ADD     R6    , R6    , #-1   
            STR     R7    , R6    , #0     ; save caller's return address
            ADD     R6    , R6    , #-1   
            STR     R5    , R6    , #0     ; save caller's frame pointer
            ADD     R5    , R6    , #-1    ; set up frame pointer
            ADD     R6    , R6    , #-30   ; decrement stack pointer beyond frame
            SET     R12   , #0           
            ADD     R11   , R12   , #0    
            SET     R13   , #10          
            ADD     R8    , R13   , #0    
            SET     R14   , #100         
            ADD     R9    , R14   , #0    
            SET     R15   , #1000        
            ADD     R10   , R15   , #0    
            ADD     R16   , R8    , #0    
            ADD     R17   , R5    , #-28  
            SET     R18   , #0           
            MUL     R19   , R18   , #1    
            ADD     R20   , R17   , R19   
            STR     R16   , R20   , #0    
            ADD     R21   , R9    , #0    
            ADD     R22   , R5    , #-28  
            SET     R23   , #1           
            MUL     R24   , R23   , #1    
            ADD     R25   , R22   , R24   
            STR     R21   , R25   , #0    
            ADD     R26   , R10   , #0    
            ADD     R27   , R5    , #-28  
            SET     R28   , #2           
            MUL     R29   , R28   , #1    
            ADD     R30   , R27   , R29   
            STR     R26   , R30   , #0    
            ADD     R31   , R8    , #0    
            ADD     R32   , R5    , #-28  
            SET     R33   , #3           
            MUL     R34   , R33   , #1    
            ADD     R35   , R32   , R34   
            STR     R31   , R35   , #0    
            ADD     R36   , R9    , #0    
            ADD     R37   , R5    , #-28  
            SET     R38   , #4           
            MUL     R39   , R38   , #1    
            ADD     R40   , R37   , R39   
            STR     R36   , R40   , #0    
            ADD     R41   , R10   , #0    
            ADD     R42   , R5    , #-28  
            SET     R43   , #5           
            MUL     R44   , R43   , #1    
            ADD     R45   , R42   , R44   
            STR     R41   , R45   , #0    
            ADD     R46   , R8    , #0    
            ADD     R0    , R46   , #0    
            OUT
            ADD     R47   , R11   , #0    
            ADD     R0    , R47   , #0    
            OUT
            SET     R48   , #0           
            STR     R48   , R5    , #3    
            BRA     L1                    
        L1: ADD     R6    , R6    , #30    ; remove locals from stack
            LDR     R5    , R6    , #0     ; restore caller's frame pointer
            ADD     R6    , R6    , #1    
            LDR     R7    , R6    , #0     ; restore caller's return address
            ADD     R6    , R6    , #1    
            RET                            ; return to caller
