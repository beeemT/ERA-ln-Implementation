global asm_ext_ln

section .data
;######################################################################################
;Tabelle e^x: Notation im exponent(1. zahl zaehler, 2. zahl nenner), in ieee standard
;      		e¹					e¹²  					e¹⁴ 				e¹⁸ 				e¹⁽¹⁶⁾ 				e¹⁽³²⁾			e¹⁽⁶⁴⁾			e¹⁽¹²⁸⁾			e¹⁽²⁵⁶)			e¹⁽⁵¹²⁾
ex: dd  0x402df854,  0x3fd3094c,  0x3fa45af2,  0x3f910b02,  0x3f88415b,  0x3f84102b, 0x3f820405, 0x3f810101, 0x3f808040, 0x3f804010
;######################################################################################

;######################################################################################
;Tabelle Brueche:
;						1/1					1/2					1/4						1/8					1/16				1/32				1/64				1/128				1/256				1/512
br: dd  0x3f800000,  0x3f000000,  0x3e800000,  0x3e000000, 0x3d800000,  0x3d000000, 0x3c800000, 0x3c000000, 0x3b800000, 0x3affffd5
;######################################################################################

;######################################################################################
;						-1
minus: dd 0xbf800000
zero: dd 0x00000000
;######################################################################################

;################program################
global asm_ext_ln
extern addressInput
extern addressResult
section .text

asm_ext_ln:

pushad
finit
;init register

mov eax, 0
mov ecx, br ;speicheradresse brueche
mov ebx, ex ; adressierung der ln tabelle, spaeter fehlerregister
mov esi, 1 ; laufvariable l
mov edx, 8 ; grenze m
mov edi, 0 ; flag fuer zahl kleiner 1 als input
fld dword [zero]
fld dword [addressInput] ;eingabe

;wenn eingabe kleiner 0 oder gleich 0 dann fehler
fcom dword [zero]
fstsw ax
and ax, 0000000100000000b; jl
cmp ax, 0
jne enderror
and ax, 0100000000000000b; je
cmp ax, 0
jne enderror

;check if input = 1
fcom dword [ecx]
fstsw ax
and ax, 0000000100000000b; jl
cmp ax, 0
jne initnegativ
and ax, 0100000000000000b; je
cmp ax, 0
jne endinpone

;check if input = e
fcom dword [ebx]
fstsw ax
and ax, 0100000000000000b; je
cmp ax, 0
jne endone
and ax, 0000000100000000b;jl
cmp ax, 0
jne precschleife

; teile durch e solange st0 >= e und inc st2
eschleife:
	fcom dword [ebx]
	fstsw ax
	and ax, 0000000100000000b;jl
	cmp ax, 0
	jne precschleife
	fdiv dword [ebx]
	fxch
	fadd dword [ecx] ;error to nan
	fxch
	jmp eschleife

precschleife:
	cmp esi, edx ; schleifenabbruchbedingung -> grenze m
	jg endnormchecknegativ

	fcom dword [ecx] ; schleifenabbruchbedingung -> vorzeitiges beenden da ln(inp) fertig berechnet
	fstsw ax
	and ax, 0100000000000000b; je
	cmp ax, 0
	jne endnormchecknegativ

	;works till here
	fcom dword [ebx + esi*4] ; compare st0 mit naechstem tabelleneintrag ex fuer teilen
	fstsw ax
	and ax, 0000000100000000b;jl
	cmp ax, 0
	jne incjmprecschleife ; wenn st0 kleiner naechster eintag ueberspringe teilen und vergleiche mit naechstem eintrag
	fdiv dword [ebx + esi*4] ; teile st0 durch groeßten tabelleneintrag der kleiner ist als st0
	fxch
	fadd dword [ecx + esi*4] ; addiere den richtigen exponenten auf ergebnis
	fxch

incjmprecschleife:
	inc esi
	jmp precschleife

initnegativ:
	mov edi, 1
	fld dword [ecx]
	fdiv st0, st1; st0 = 1 / st0 fuer zahlen kleiner 1
	fld dword [zero]
	fxch
	jmp eschleife
;################returns################
enderror:
	popad
	mov dword [addressResult], 0x7fffffff ;NaN
	jmp return

endinpone:
	popad
	mov dword [addressResult], 0
	jmp return

endone:
	popad
	mov dword [addressResult], 1
	jmp return

endnormchecknegativ:
	cmp edi, 1
	je mkneg

endnorm:
	popad
	fxch
	fst dword [addressResult]
	mov ebx, 0
	jmp return

return:
	finit
	ret

mkneg:
	fxch
	fmul dword [minus]  ;st0(ergebnis) * -1
	fxch
	mov edi, 0
	jmp endnorm
