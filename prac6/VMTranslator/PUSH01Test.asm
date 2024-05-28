	@2 // push constant 2
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1


	@SP // neg
	A=M
	A=A-1
	M=-M


	@3 // push constant 3
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1


	@SP // neg
	A=M
	A=A-1
	M=-M


	@SP // add
	AM=M-1
	D=M
	A=A-1
	M=D+M


	@4 // push constant 4
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1


	@SP // neg
	A=M
	A=A-1
	M=-M


	@SP // add
	AM=M-1
	D=M
	A=A-1
	M=D+M


	@16 // pop static 0
	D=A
	@0
	D=D+A
	@R13
	M=D
	@SP
	AM=M-1
	D=M
	@R13
	A=M
	M=D


