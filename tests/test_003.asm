            .ORIG x2000
            JSR     main                  
            HALT
      main: ADD     R6    , R6    , #-1    ; reserve space for the return value
            ADD     R6    , R6    , #-1   
            STR     R7    , R6    , #0     ; save caller's return address
            ADD     R6    , R6    , #-1   
            STR     R5    , R6    , #0     ; save caller's frame pointer
            ADD     R5    , R6    , #-1    ; set up frame pointer
            ADD     R6    , R6    , #-118  ; decrement stack pointer beyond frame
            SET     R10   , #0           
            ADD     R11   , R5    , #-116 
            STR     R10   , R11   , #0    
            ADD     R12   , R5    , #-116 
            ADD     R9    , R12   , #0    
            ADD     R13   , R5    , #-115 
            SET     R14   , #10          
            MUL     R15   , R14   , #1    
            ADD     R16   , R13   , R15   
            ADD     R9    , R16   , #0    
            SET     R17   , #5           
            ADD     R18   , R9    , #0    
            STR     R17   , R18   , #0    
            SET     R19   , #0           
            STR     R19   , R5    , #3    
            BRA     L1                    
        L1: ADD     R6    , R6    , #118   ; remove locals from stack
            LDR     R5    , R6    , #0     ; restore caller's frame pointer
            ADD     R6    , R6    , #1    
            LDR     R7    , R6    , #0     ; restore caller's return address
            ADD     R6    , R6    , #1    
            RET                            ; return to caller
