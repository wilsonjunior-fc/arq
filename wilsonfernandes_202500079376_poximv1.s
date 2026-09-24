#
# Poxim-V - programa de teste de cobertura (teste.s)
#
# #IA
# #Claude["Preciso escrever um arquivo .s para testar o meu simulador. Analise o poxim-v.py (cobertura das 45 instrucoes, 5 enderecos e 8 operandos por instrucao, 13 usos por registrador) e os exemplos fatorial.s e mul_div_rem.s, e me guie em blocos na construcao do arquivo .s ao mesmo tempo que explica a teoria por tras das instrucoes de cada bloco"]
#

# Code section
.section .text
.global main
main:
    # Setup: ponteiros seguros (memoria: 0x80000000 a 0x80007fff)
    # #Claude["Bloco 1: Como declaro os ponteiros sp e gp como ponteiros seguros?"]
    lui    sp, 0x80008
    addi   sp, sp, -256
    lui    gp, 0x80008
    addi   gp, gp, -2048

    # Setup: valores iniciais nao nulos (positivos e negativos)
    addi   ra,  zero, 3
    addi   tp,  zero, 7
    addi   t0,  zero, 1
    addi   t1,  zero, 2
    addi   t2,  zero, -3
    addi   s0,  zero, 5
    addi   s1,  zero, -8
    addi   a0,  zero, 10
    addi   a1,  zero, 21
    addi   a2,  zero, -17
    addi   a3,  zero, 100
    addi   a4,  zero, 33
    addi   a5,  zero, -1
    addi   a6,  zero, 4
    addi   a7,  zero, 27
    addi   s2,  zero, 64
    addi   s3,  zero, 255
    addi   s4,  zero, -100
    addi   s5,  zero, 12
    addi   s6,  zero, 9
    addi   s7,  zero, 1000
    addi   s8,  zero, -7
    addi   s9,  zero, 31
    addi   s10, zero, 17
    addi   s11, zero, 2047
    addi   t3,  zero, 6
    addi   t4,  zero, -2048
    addi   t5,  zero, 15
    addi   t6,  zero, 8

    # Bloco 1: instrucoes tipo R (ALU registrador-registrador)
    # #Claude["Bloco 1: Qual seria a estrutura para criar as intrução do tipo R (add, sub, and, or, xor, sll, srl, sra, slt, sltu)?"]

    # add: soma
    add    t4, zero, tp
    add    a6, s6, t0
    add    a4, s7, t5
    add    s2, ra, t3
    add    s1, t2, t6

    # sub: subtracao
    sub    a3, zero, t1
    sub    a5, s5, s4
    sub    a0, a2, a1
    sub    s0, s10, s3
    sub    a7, s9, s8

    # and: E bit a bit
    and    s11, t4, tp
    and    a6, s6, t0
    and    a4, s7, t5
    and    s2, ra, t3
    and    s1, t2, t6

    # or: OU bit a bit
    or     a3, t1, a5
    or     s5, s4, a0
    or     a2, a1, s0
    or     s10, s3, a7
    or     s9, s8, s11

    # xor: OU exclusivo
    xor    t4, tp, a6
    xor    s6, t0, a4
    xor    s7, t5, s2
    xor    ra, sp, t3
    xor    s1, t2, t6

    # sll: deslocamento logico a esquerda
    sll    a3, t1, a5
    sll    s5, s4, a0
    sll    a2, a1, s0
    sll    s10, s3, a7
    sll    s9, gp, s8

    # srl: deslocamento logico a direita
    srl    s11, t4, tp
    srl    a6, s6, t0
    srl    a4, s7, t5
    srl    s2, ra, sp
    srl    t3, s1, t2

    # sra: deslocamento aritmetico a direita
    sra    t6, a3, t1
    sra    a5, s5, s4
    sra    a0, a2, a1
    sra    s0, s10, s3
    sra    a7, gp, s9

    # slt: menor que (com sinal)
    slt    s8, s11, t4
    slt    tp, a6, s6
    slt    t0, a4, s7
    slt    t5, s2, ra
    slt    t3, sp, s1

    # sltu: menor que (sem sinal)
    sltu   t2, zero, t6
    sltu   a3, t1, a5
    sltu   s5, s4, a0
    sltu   a2, a1, s0
    sltu   s10, s3, a7

    # Bloco 2: instrucoes de memoria (Loads/Stores)
    # #Claude["Bloco 2: Qual a estrutura para criar as instruções de memória"?"]
    # sb: armazena byte
    sb     gp, 64(sp)
    sb     s1, 65(gp)
    sb     s9, 67(sp)
    sb     s4, -17(gp)
    sb     s8, 70(sp)

    # sh: armazena meia palavra (16 bits)
    sh     s11, 32(sp)
    sh     t6, 36(gp)
    sh     t4, 40(sp)
    sh     s1, -12(gp)
    sh     tp, 44(sp)

    # sw: armazena palavra (32 bits)
    sw     a6, 0(sp)
    sw     s2, 4(gp)
    sw     s6, 8(sp)
    sw     ra, -8(gp)
    sw     t0, 16(sp)

    # lb: carrega byte com extensao de sinal
    lb     a4, -17(gp)
    lb     s7, 64(sp)
    lb     t5, 65(gp)
    lb     t3, 70(sp)
    lb     t2, 67(sp)

    # lh: carrega meia palavra com extensao de sinal
    lh     a3, 40(sp)
    lh     t1, -12(gp)
    lh     a5, 32(sp)
    lh     s5, 44(sp)
    lh     a0, 36(gp)

    # lw: carrega palavra
    lw     a2, 4(gp)
    lw     a1, 16(sp)
    lw     s0, 0(sp)
    lw     s10, -8(gp)
    lw     s3, 8(sp)

    # lbu: carrega byte sem sinal
    lbu    a7, 1(sp)
    lbu    s9, 3(sp)
    lbu    s8, 64(sp)
    lbu    s11, 65(gp)
    lbu    t4, -17(gp)

    # lhu: carrega meia palavra sem sinal
    lhu    tp, 2(sp)
    lhu    a6, 6(gp)
    lhu    s6, 36(gp)
    lhu    t0, 40(sp)
    lhu    a4, 44(sp)

    # Bloco 3: ALU com imediato, deslocamentos por imediato, lui e auipc
    # #Claude["Bloco 3: Estrutura para as intruções addi, andi, slti, sltiu, ori, xori, slli, srli, srai, lui, auipc"]

    # addi: soma com imediato
    addi   s7, sp, 5
    addi   t5, gp, -9
    addi   s2, ra, 1000
    addi   t3, t2, -2048
    addi   t6, a3, 2047

    # andi: E bit a bit com imediato
    andi   t1, sp, 0xff
    andi   a5, s5, 0xf
    andi   s4, a0, -16
    andi   a2, a1, 0x7ff
    andi   s0, s10, 0x55

    # slti: menor que imediato (com sinal)
    slti   s3, a7, 50
    slti   s9, gp, -50
    slti   s8, s11, 1
    slti   t4, tp, 2047
    slti   a6, s6, -2048

    # sltiu: menor que imediato (sem sinal)
    sltiu  t0, a4, 100
    sltiu  s7, t5, -1
    sltiu  s2, ra, 1
    sltiu  t3, sp, 2047
    sltiu  s1, t2, 5

    # ori: OU bit a bit com imediato
    ori    t6, a3, 0x1
    ori    t1, a5, 0xf0
    ori    s5, s4, -256
    ori    a0, a2, 0x7ff
    ori    a1, s0, 0x100

    # xori: OU exclusivo com imediato
    xori   s10, s3, -1
    xori   a7, gp, 0x55
    xori   s9, s8, 0x2aa
    xori   s11, t4, 0x1
    xori   tp, a6, 0x7ff

    # slli: deslocamento logico a esquerda por imediato
    slli   s6, t0, 1
    slli   a4, s7, 4
    slli   t5, s2, 9
    slli   ra, sp, 17
    slli   t3, s1, 31

    # srli: deslocamento logico a direita por imediato
    srli   t2, t6, 1
    srli   a3, t1, 3
    srli   a5, s5, 8
    srli   s4, a0, 15
    srli   a2, a1, 31

    # srai: deslocamento aritmetico a direita por imediato
    srai   s0, s10, 2
    srai   s3, a7, 5
    srai   s9, gp, 12
    srai   s8, s11, 20
    srai   t4, tp, 31

    # lui: carrega imediato nos 20 bits superiores
    lui    a6, 0x12345
    lui    s6, 0xfffff
    lui    t0, 0x00001
    lui    a4, 0x7abcd
    lui    s7, 0x80000

    # auipc: soma imediato (20 bits superiores) ao pc
    auipc  t5, 0x00001
    auipc  s2, 0x00010
    auipc  ra, 0xfffff
    auipc  t3, 0x00100
    auipc  s1, 0x7ffff

    # Bloco 4: extensao M (multiplicacao e divisao)
    # #Claude["Bloco 4: Estrutura para as intruções com extensão M (multiplicacao e divisao)"]

    addi   t2, zero, 7
    addi   t6, zero, -3
    lui    a3, 0x80000
    addi   a3, a3, -1
    lui    t1, 0x80000
    addi   a5, zero, -1
    addi   s5, zero, 1000
    lui    s4, 0x12345
    addi   s4, s4, 1656
    lui    a0, 0xdeadc
    addi   a0, a0, -273
    lui    a2, 0x00010
    addi   a2, a2, 1
    addi   a1, zero, -2048

    # mul: multiplicacao (32 bits inferiores)
    mul    s0, sp, s10
    mul    s3, a7, gp
    mul    s9, s8, s11
    mul    t4, tp, a6
    mul    s6, t0, a4

    # mulh: multiplicacao com sinal (32 bits superiores)
    mulh   s7, a3, a0
    mulh   t5, s2, ra
    mulh   t3, sp, s1
    mulh   s0, t2, t6
    mulh   s10, t1, a5

    # mulhsu: multiplicacao com sinal x sem sinal (32 bits superiores)
    mulhsu s3, t6, a5
    mulhsu a7, s5, a1
    mulhsu s9, gp, s8
    mulhsu s11, t4, tp
    mulhsu a6, s6, t0

    # mulhu: multiplicacao sem sinal (32 bits superiores)
    mulhu  a4, t1, a5
    mulhu  s7, t5, s2
    mulhu  ra, sp, t3
    mulhu  s1, t2, s5
    mulhu  s0, a1, s10

    # div: divisao com sinal
    div    s3, t1, a5
    div    a7, gp, t2
    div    s9, s8, t6
    div    s11, t4, s5
    div    tp, a6, s4

    # divu: divisao sem sinal
    divu   s6, a0, t2
    divu   t0, a4, a2
    divu   s7, t5, a1
    divu   s2, ra, t6
    divu   t3, sp, a3

    # rem: resto da divisao com sinal
    rem    s1, t1, a5
    rem    s0, s10, s5
    rem    s3, a7, s4
    rem    s9, gp, a2
    rem    s8, s11, a1

    # remu: resto da divisao sem sinal
    remu   t4, tp, t2
    remu   a6, s6, t6
    remu   t0, a4, a3
    remu   s7, t5, t1
    remu   s2, ra, s5

    # Bloco 5: desvios condicionais, jal e jalr
    # #Claude["Bloco 5: Estrutura correta para as intruções de beq, bne, blt, bge, bltu, bgeu, jal e jalr"]

    # beq: desvia se igual
    beq    s0, s1, 1f
1:
    beq    sp, t3, 1f
1:
    beq    s10, t4, 1f
1:
    beq    a7, s3, 1f
1:
    beq    a6, s6, 1f
1:

    # bne: desvia se diferente
    bne    gp, s9, 1f
1:
    bne    tp, t0, 1f
1:
    bne    s8, s11, 1f
1:
    bne    ra, s2, 1f
1:
    bne    a4, s7, 1f
1:

    # blt: desvia se menor (com sinal)
    blt    t1, a3, 1f
1:
    blt    a3, t1, 1f
1:
    blt    sp, t5, 1f
1:
    blt    t3, s1, 1f
1:
    blt    a0, s4, 1f
1:

    # bge: desvia se maior ou igual (com sinal)
    bge    a3, t1, 1f
1:
    bge    t1, a3, 1f
1:
    bge    a2, a1, 1f
1:
    bge    s3, s0, 1f
1:
    bge    s10, a7, 1f
1:

    # bltu: desvia se menor (sem sinal)
    bltu   a3, t1, 1f
1:
    bltu   t1, a3, 1f
1:
    bltu   gp, s9, 1f
1:
    bltu   s8, s11, 1f
1:
    bltu   t4, a6, 1f
1:

    # bgeu: desvia se maior ou igual (sem sinal)
    bgeu   t1, a3, 1f
1:
    bgeu   a3, t1, 1f
1:
    bgeu   s6, tp, 1f
1:
    bgeu   t0, a4, 1f
1:
    bgeu   t5, s7, 1f
1:

    # jal: salto com link (para a instrucao seguinte)
    jal    ra, 1f
1:
    jal    zero, 1f
1:
    jal    s2, 1f
1:
    jal    t3, 1f
1:
    jal    s1, 1f
1:
    jal    t2, 1f
1:
    jal    t6, 1f
1:
    jal    a5, 1f
1:

    # jalr: salto indireto com link
    auipc  s5, 0
    jalr   ra, s5, 8
    auipc  s4, 0
    jalr   zero, s4, 8
    auipc  a0, 0
    jalr   a2, a0, 8
    auipc  a1, 0
    jalr   s0, a1, 8
    auipc  s10, 0
    jalr   s3, s10, 8

    slli   zero, zero, 31
    ebreak
    srai   zero, zero, 7
