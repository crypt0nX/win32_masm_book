;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Sample code for < Win32ASM Programming 3rd Edition>
; by ���Ʊ�, http://www.win32asm.com.cn
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; GetVersionInfo.inc
; ������ȡ PE �ļ���Դ�а汾��Ϣ���ӳ���
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ͨ�� include ������������Դ������ʹ��
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.const

szRoot		db	'\',0
szVarInfo	db	'\VarFileInfo\Translation',0
szStrInfoFmt	db	'\StringFileInfo\%08x\',0
szArrow		db	' --> ',0
szReturn	db	0dh,0ah,0
szFixFileInfo	db	'�̶��汾��Ϣ���ԣ�',0dh,0ah
		db	'FileVersion: %d.%d.%d.%d',0dh,0ah
		db	'ProductVersion: %d.%d.%d.%d',0dh,0ah
		db	'FileOS: %08x',0dh,0ah
		db	'FileType: %08x',0dh,0ah,0dh,0ah
		db	'�ַ����汾���ԣ�',0dh,0ah,0
;********************************************************************
szStr1		db	'Comments',0
szStr2		db	'CompanyName',0
szStr3		db	'FileDescription',0
szStr4		db	'FileVersion',0
szStr5		db	'InternalName',0
szStr6		db	'LegalCopyright',0
szStr7		db	'LegalTrademarks',0
szStr8		db	'OriginalFilename',0
szStr9		db	'PrivateBuild',0
szStr10		db	'ProductName',0
szStr11		db	'ProductVersion',0
szStr12		db	'SpecialBuild',0
;********************************************************************
lpStr1		dd	szStr1
lpStr2		dd	szStr2
lpStr3		dd	szStr3
lpStr4		dd	szStr4
lpStr5		dd	szStr5
lpStr6		dd	szStr6
lpStr7		dd	szStr7
lpStr8		dd	szStr8
lpStr9		dd	szStr9
lpStr10		dd	szStr10
lpStr11		dd	szStr11
lpStr12		dd	szStr12

		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ��ȡ�ļ��汾��Ϣ
; ��ڲ������ļ�����������Ϣ������
; ���ز�����TRUE = �ɹ���FALSE = ʧ��
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_GetVersionInfo	proc	_lpFileName,_lpBuffer
		local	@dbVerInfo[4096]:byte
		local	@szStringInfo[128]:byte
		local	@szString[128]:byte
		local	@lpBuffer,@dwLen
		local	@dwVer1,@dwVer2,@dwVer3,@dwVer4
		local	@dwVer5,@dwVer6,@dwVer7,@dwVer8

		pushad
		invoke	GetFileVersionInfoSize,_lpFileName,NULL
		.if	!eax
			popad
			xor	eax,eax
			ret
		.endif
		invoke	GetFileVersionInfo,_lpFileName,NULL,sizeof @dbVerInfo,addr @dbVerInfo
;********************************************************************
; ��ȡ�̶�����
;********************************************************************
		invoke	VerQueryValue,addr @dbVerInfo,addr szRoot,addr @lpBuffer,addr @dwLen
		mov	esi,@lpBuffer
		assume	esi:ptr VS_FIXEDFILEINFO
;********************************************************************
; ����汾��
;********************************************************************
		mov	eax,[esi].dwFileVersionMS
		movzx	ecx,ax
		mov	@dwVer2,ecx
		shr	eax,16
		mov	@dwVer1,eax

		mov	eax,[esi].dwFileVersionLS
		movzx	ecx,ax
		mov	@dwVer4,ecx
		shr	eax,16
		mov	@dwVer3,eax

		mov	eax,[esi].dwProductVersionMS
		movzx	ecx,ax
		mov	@dwVer6,ecx
		shr	eax,16
		mov	@dwVer5,eax

		mov	eax,[esi].dwProductVersionLS
		movzx	ecx,ax
		mov	@dwVer8,ecx
		shr	eax,16
		mov	@dwVer7,eax
		invoke	wsprintf,_lpBuffer,addr szFixFileInfo,\
			@dwVer1,@dwVer2,@dwVer3,@dwVer4,\
			@dwVer5,@dwVer6,@dwVer7,@dwVer8,\
			[esi].dwFileOS,[esi].dwFileType

		assume	esi:nothing
;********************************************************************
; ��ȡ���Լ�
;********************************************************************
		invoke	VerQueryValue,addr @dbVerInfo,addr szVarInfo,addr @lpBuffer,addr @dwLen
		mov	eax,@lpBuffer
		mov	eax,[eax]
		ror	eax,16
		invoke	wsprintf,addr @szStringInfo,addr szStrInfoFmt,eax
;********************************************************************
; ��ȡ�ַ����汾��Ϣ
;********************************************************************
		mov	ebx,offset lpStr1
		.while	ebx <=	offset lpStr12
			invoke	lstrcpy,addr @szString,addr @szStringInfo
			mov	eax,[ebx]
			invoke	lstrcat,addr @szString,eax
			mov	eax,[ebx]
			invoke	lstrcat,_lpBuffer,eax
			invoke	lstrcat,_lpBuffer,addr szArrow		;���� -->
			invoke	VerQueryValue,addr @dbVerInfo,addr @szString,addr @lpBuffer,addr @dwLen
			.if	eax
				invoke	lstrcat,_lpBuffer,@lpBuffer	;���ϻ�õİ汾��Ϣ
			.endif
			invoke	lstrcat,_lpBuffer,addr szReturn		;���ϻس�
			add	ebx,4
		.endw
		popad
		mov	eax,TRUE
		ret

_GetVersionInfo	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>