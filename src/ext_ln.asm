global asm_ext_ln

section .data
;######################################################################################
;Tabelle e^x: Notation im exponent(1. zahl zaehler, 2. zahl nenner), in ieee standard
;      		e¹					e¹²  					e¹⁴ 				e¹⁸ 				e¹⁽¹⁶⁾ 				e¹⁽³²⁾
ex: dd  0x402df851,  0x3fd3094a,  0x3fa45aee,  0x3f910afe,  0x3f884157,  0x3f841028
;######################################################################################

;######################################################################################
;Tabelle Brueche:
;						1/1					1/2					1/4						1/8					1/16				1/32
br: dd  0x3f800000,  0x3f000000,  0x3e800000,  0x3e000000, 0x3d800000,  0x3d000000
;######################################################################################

;######################################################################################
;						-1
minus: dd 0xbf800000

ergebnis: dd 0x00000000
;######################################################################################

;################program################
section .text

asm_ext_ln:
pushad
finit

;init register
fld dword [eax+4] ; eingabe
mov eax, br ;speicheradresse brueche
mov ebx, ex ; adressierung der ln tabelle, spaeter fehlerregister
mov esi, minus ; -1
mov ecx, 1 ; laufvariable l
mov edx, 6 ; grenze m
mov edi, 0 ; flag fuer zahl kleiner 1 als input
fld dword [eax]

;wenn eingabe kleiner 0 oder gleich 0 dann fehler
cmp eax, 0
jle enderror

;check if input = 1
fcomp dword [eax]
je endinpone
jl initnegativ

;check if input = e
fcomp dword [ebx]
je endone
jl precschleife

; teile durch e solange st0 >= e und inc st2
eschleife:
	fcomp dword [ebx]
	jl precschleife
	fdiv dword [ebx]
	fxch
	fadd st2, st0
	fxch
	jmp eschleife

precschleife:
	cmp ecx, edx ; schleifenabbruchbedingung -> grenze m
	jg endnormchecknegativ

	fcomp dword [eax] ; schleifenabbruchbedingung -> vorzeitiges beenden da ln(inp) fertig berechnet
	je endnormchecknegativ

	fcomp dword [ebx + ecx*4] ; compare st0 mit naechstem tabelleneintrag ex fuer teilen
	jl incjmprecschleife ; wenn st0 kleiner naechster eintag ueberspringe teilen und vergleiche mit naechstem eintrag
	fdiv dword [ebx + ecx*4] ; teile st0 durch groeßten tabelleneintrag der kleiner ist als st0
	fxch st2 ; st2 arbeit st0 ergebnis
	fadd dword [eax + ecx*4] ; addiere den richtigen exponenten auf ergebnis
	fxch st2 ;-> st2 = ergebnis st0 = arbeit

incjmprecschleife:
	inc ecx
	jmp precschleife

initnegativ:
	mov edi, 1
	fdiv st1, st0 ; st0 = 1 / st0 fuer zahlen kleiner 1
	fxch
	jmp eschleife
;################returns################
enderror:
	mov eax, 0
	mov ebx, 1
	jmp return

endinpone:
	mov eax, 0
	mov ebx, 0
	jmp return

endone:
	mov eax, 1
	mov ebx, 0
	jmp return

endnormchecknegativ:
	cmp edi, 1
	je mkneg

endnorm:
	fxch st2
	fst dword [eax]
	mov eax, [eax]
	mov [ergebnis], eax
	mov eax, ergebnis
	mov ebx, 0
	jmp return

return:
	finit
	ret

mkneg:
	fxch st2
	fmul dword [esi]  ;st0(ergebnis) * -1
	fxch st2
	mov edi, 0
	jmp endnorm
