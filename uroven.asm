	;***********************************
	;  генерируем переменное напр€жение
	;  следим за датчик уровн€
	;  включаем оптосемистор
	;**********************************
	list      p=12F683        ; list directive to define processor
	#include <p12F683.inc>    ; processor specific variable definitions

	errorlevel  -302          ; suppress message 302 from list file

	__CONFIG   _FCMEN_ON & _IESO_OFF & _CP_OFF & _CPD_OFF & _BOD_OFF & _MCLRE_ON & _WDT_OFF & _PWRTE_ON & _INTRC_OSC_NOCLKOUT 

; '__CONFIG' directive is used to embed configuration word within .asm file.
; The lables following the directive are located in the respective .inc file.
; See data sheet for additional information on configuration word settings.




;***** VARIABLE DEFINITIONS
w_temp        EQU     0x7E        ; variable used for context saving 
status_temp   EQU     0x7F        ; variable used for context saving

cblock 0x20
RESULTHI		
flag_work	  

temp
temp1
endc




;**********************************************************************
	ORG     0x000             ; processor reset vector
	goto    main              ; go to beginning of program

	ORG     0x004             ; interrupt vector location
	movwf   w_temp            ; save off current W register contents
	movf	STATUS,w          ; move status register into W register
	movwf	status_temp       ; save off contents of STATUS register
	BANKSEL	INTCON
	movf	INTCON,W			;  ƒа
	andlw	0x20	
	btfss	STATUS,Z			;  ќно произошло?
	goto	T0Interrupt		; ќбработка прерывани€ по изменению состо€ни€ порта


	BANKSEL	INTCON
	movf	INTCON,W			;
	andlw	0x40				; разрешались немаскированные прерывани€
	btfss	STATUS,Z			;
	goto	CMPInterrupt		; обработка немаскированного прерывани€
Next2
	goto	EndIsr

T0Interrupt

	BANKSEL	INTCON
	btfss	INTCON,T0IF
	goto	EndIsr
;	movlw	0xF7
;	movwf	TMR0

;	bcf		INTCON,T0IE
	bcf		INTCON,T0IF
;;call	delay
	btfss	temp,0
	goto 	a1
	bcf		temp,0
	bcf		GPIO,5
	bsf		GPIO,4
	goto	a2
a1
	bsf		temp,0
	bcf		GPIO,4
	bsf		GPIO,5
a2
;	call	delay
	
	goto	EndIsr


		;***************************************************
					;
					;	ќбработка прерывани€ компаратора
					;	при падении напр€жени€ на измерительном диоде
					;	более 0,5в
					;
					;***************************************************
CMPInterrupt
	BANKSEL	PIE1
	btfss	PIE1,CMIE
	goto	EndIsr

	BANKSEL	CMCON0
	movfw	CMCON0
;	call	delay
	BANKSEL	PIR1
	bcf		PIR1,CMIF	
;	call	ADC
	BANKSEL	flag_work
	movf	flag_work,w
	btfsc	STATUS, Z	
	goto	EndIsr
	BANKSEL	RESULTHI
	movfw	RESULTHI
;	movlw	0xF0
;	andlw	0xf0
;	movlw	0xf0
	BANKSEL	TMR0
	movwf	TMR0
;;;	call	delay
	BANKSEL	INTCON
	bcf	    INTCON,T0IF
	bsf		INTCON,T0IE
	goto	EndIsr			

EndIsr
	
E1
; isr code can go here or be located as a call subroutine elsewhere


	movf    status_temp,w     ; retrieve copy of STATUS register
	movwf	STATUS            ; restore pre-isr STATUS register contents
	swapf   w_temp,f
	swapf   w_temp,w          ; restore pre-isr W register contents
	retfie                    ; return from interrupt





main
BANKSEL	GPIO
	clrf	flag_work
	clrf	GPIO
	clrf	TMR0
	clrf	temp
	BANKSEL	WPU
	movlw	b'11110111'		; выравнились по левому краю
	movwf	OSCCON
  	movlw	b'11011111'		; выравнились по левому краю
	movwf	OPTION_REG
	BANKSEL TMR0 			;
	CLRWDT 					;Clear WDT
	CLRF TMR0 				;Clear TMR0 and
;BANKSEL	WPU

;movlw	0x34;
;movwf	WPU
; initialize eeprom locations
	BANKSEL	ANSEL
	movlw	B'00000001'				; GP0 как компаратор аналоговый
	movwf	ANSEL
	movlw	B'01001110'				; режим компаратора   
	BANKSEL	CMCON0
	movwf	CMCON0
	movlw	B'10100011'				; установили REF компоратора
	BANKSEL	VRCON
	movwf	VRCON
	BANKSEL	TRISIO
	movlw	B'00001011'	       		;   
	movwf 	TRISIO
	BANKSEL	PIR1
	bcf		PIR1,CMIF				; очистили бит прерывани€ компоратора
	BANKSEL	PIE1
	bsf		PIE1,CMIE				; разрешение компоратора
	BANKSEL	INTCON
									;bsf		INTCON,GPIE 			; разрешить прерывание GPIO	
	bsf		INTCON,PEIE 			; разрешить все немаскированные периферийные прерывани€	
	bcf		INTCON,T0IF
	bsf		INTCON,GIE				; разрешить глобальное прерывание	
	bsf		INTCON,T0IE	

START
	BANKSEL	GPIO
	bsf		GPIO,GP5
	bsf		GPIO,GP4
	bcf		GPIO,GP4
	bcf		GPIO,GP5
START1
	bcf		GPIO,GP2
	bsf		GPIO,GP2

	goto 	START1
	ORG	0x2100
	DE	0x00, 0x01, 0x02, 0x03


	END                       ; directive 'end of program'
