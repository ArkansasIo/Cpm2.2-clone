; CP/M-style 8080/Z80 bootloader skeleton for the emulator.
; This is a clean-room boot protocol, not a copy of proprietary CP/M source.
;
; Entry: 0000h
; Loads a boot sector through BIOS service 0 and transfers control to CCP.

        ORG 0000H

BOOT:
        DI
        XOR A
        LD (BOOT_STATUS),A

        ; BIOS disk-select/read services are exposed by the emulator firmware.
        LD C,00H          ; service: select drive
        CALL BIOS
        LD C,01H          ; service: read boot sector
        CALL BIOS
        JR C,BOOT_ERROR

        LD HL,CCP_LOAD
        JP (HL)

BOOT_ERROR:
        LD HL,ERROR_MSG
PRINT:  LD A,(HL)
        OR A
        JR Z,HALT
        CALL CONSOLE_OUT
        INC HL
        JR PRINT

HALT:   HALT
        JR HALT

BIOS:   ; Emulator-provided BIOS entry point.
        RET

CONSOLE_OUT:
        RET

BOOT_STATUS: DB 00H
CCP_LOAD:   EQU 0100H
ERROR_MSG:  DB "CP/M BOOT ERROR",0DH,0AH,0
