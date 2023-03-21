        ;延时，一般是(3-5)*n个周期（n!=0)         

        EXPORT   __delay_loops

        EXPORT   __delay_loops_0

        

        AREA |.text|,CODE,READONLY

        THUMB
		

__delay_loops

        SUBS     R0,R0,#+1                

        BNE      __delay_loops



__delay_loops_0

        BX       LR               ;; return

        

			