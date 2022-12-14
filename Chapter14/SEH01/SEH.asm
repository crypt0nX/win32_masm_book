;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Sample code for < Win32ASM Programming 4th Edition>
; by 罗云彬, luoyunbin@hotmail.com
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; SEH.asm
; 使用 SEH 进行错误截获
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 使用 nmake 或下列命令进行编译和链接:
; ml /c /coff SEH.asm
; Link /subsystem:windows SEH.obj
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.const

szMsg		db	'异常发生位置：%08X，异常代码：%08X，标志：%08X',0
szSafe		db	'回到了安全的地方!',0
szCaption	db	'SEH例子',0

		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; SEH Handler 异常处理程序
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Handler	proc	C _lpExceptionRecord,_lpSEH,_lpContext,_lpDispatcherContext
		local	@szBuffer[256]:byte

		pushad
		mov	esi,_lpExceptionRecord
		mov	edi,_lpContext
		assume	esi:ptr EXCEPTION_RECORD,edi:ptr CONTEXT
		invoke	wsprintf,addr @szBuffer,addr szMsg,\
			[edi].regEip,[esi].ExceptionCode,[esi].ExceptionFlags
		invoke	MessageBox,NULL,addr @szBuffer,NULL,MB_OK
		mov	[edi].regEip,offset _SafePlace
		assume	esi:nothing,edi:nothing
		popad
		mov	eax,ExceptionContinueExecution
		ret

_Handler	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
;********************************************************************
; 在堆栈中构造一个 EXCEPTION_REGISTRATION 结构
;********************************************************************
		assume	fs:nothing
		push	offset _Handler
		push	fs:[0]
		mov	fs:[0],esp
;********************************************************************
; 会引发异常的指令
;********************************************************************
		xor	eax,eax
		mov	dword ptr [eax],0	;产生异常，然后_Handler被调用
;		...
; 如果这中间有指令，这些指令将不会被执行!
;		...
_SafePlace:
		invoke	MessageBox,NULL,addr szSafe,addr szCaption,MB_OK
;********************************************************************
; 恢复原来的 SEH 链
;********************************************************************
		pop	fs:[0]
		pop	eax
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start
