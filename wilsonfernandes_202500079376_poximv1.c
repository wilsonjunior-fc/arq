// How to build and run:
// $ gcc -Wall -O3 nomesobrenome_123456789012_exemplo.c -o nomesobrenome_123456789012_exemplo.elf
// $ ./nomesobrenome_123456789012_exemplo.elf input.hex output.out [terminal.in terminal.out]

// #IA #GEMINI["Enviei os arquivos do projeto Poxim-V. Preciso construir um simulador que leia um .hex e gere um .out. Analise a estrutura dos arquivos em anexo, me explique o objetivo do simulador e seu funcionamento e me guie na estruturação do código em C."]

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// #IA #CLAUDE["Preciso desenvolver um simulador que leia um arquivo .hex e gere um arquivo .out seguindo o padrão de saída do modelo modeloDeSaida_OUTPUT. Me guie na construção do código e na lógica teórica por trás de cada instrução RISC-V"]

/**
 * Main function
 * @param argc	Number of command line arguments
 * @param argv	Command line arguments
 * @return		Returns the program execution status
 */

// #IA #CLAUDE["Como faço para ler strings e dividi-las em C?","Como identificar linhas que começam com um caractere específico em C?"]
void load_memory (FILE* input, uint8_t* mem, uint32_t offset) {

    char linha[256];
    uint32_t endereco = 0;

    while (fgets(linha, sizeof(linha), input) != NULL) {
        if (linha[0] == '@') {
            endereco = (uint32_t) strtoul (linha + 1, NULL, 16);
        } else {
            char* pedaco = strtok(linha, " ");
            while (pedaco != NULL) {
                uint8_t byte = (uint8_t) strtoul (pedaco, NULL, 16);
                mem[endereco - offset] = byte;
                endereco = endereco + 1;
                pedaco = strtok(NULL, " ");
            }
        }
    }
}

int main(int argc, char* argv[]) {
	// Outputting separator
	printf("--------------------------------------------------------------------------------\n");
	// Iterating over arguments
	for(int i = 0; i < argc; i++) {
		// Outputting argument
		printf("argv[%i] = %s\n", i, argv[i]);
	}
	if(argc < 3) {
		printf("Uso: %s input.hex output.out\n", argv[0]);
		return 1;
	}
	// Opening input and output files using proper permissions
	FILE* input = fopen(argv[1], "r");
    if (!input) {
        printf ("Erro: Não foi possível ler o arquivo %s\n", argv[1]);
        return 1;
    }
	FILE* output = fopen(argv[2], "w+");
    if (!output) {
        printf("ERRO: Não foi possível ler o arquivo %s\n", argv[2]);
        return 1;
    }

	// Setting memory offset to 0x80000000
	const uint32_t offset = 0x80000000;
	// Creating 32 registers initialized with zero and labels
	uint32_t x[32] = { 0 };
	const char* x_label[32] = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6" };
	// Creating pc register initialized with memory offset
	uint32_t pc = offset;
	// Creating 32 KiB memory for both data and instructions
	uint8_t* mem = (uint8_t*)(calloc(32 * 1024, 1));	
	
	// Reading memory contents from input hexadecimal file
    load_memory (input, mem, offset);

	// Outputting separator
	printf("--------------------------------------------------------------------------------\n");
	// Setting run condition
	uint8_t run = 1;
	// Loop while condition is true
	while(run) {
		// Reading instruction from memory (4 byte alignment)
		const uint32_t instruction = ((uint32_t*)(mem))[(pc - offset) >> 2];
		// Retrieving instruction opcode (6:0)
		const uint8_t opcode = instruction & 0b1111111;
		// Retrieving instruction fields
		const uint8_t funct7 = instruction >> 25;
		const uint16_t imm = instruction >> 20;
		const uint8_t uimm = (instruction & (0b11111 << 20)) >> 20;
		const uint8_t rs1 = (instruction & (0b11111 << 15)) >> 15;
		const uint8_t rs2 = (instruction & (0b11111 << 20)) >> 20;
		const uint8_t funct3 = (instruction & (0b111 << 12)) >> 12;
		const uint8_t rd = (instruction & (0b11111 << 7)) >> 7;
		const uint32_t imm20 = ((instruction >> 31) << 19) | (((instruction & (0b11111111 << 12)) >> 12) << 11) | (((instruction & (0b1 << 20)) >> 20) << 10) | ((instruction & (0b1111111111 << 21)) >> 21);
		char campo[32];
		uint32_t data = 0;

		// Checking instruction opcode
		switch(opcode) {

			// #IA #CLAUDE["Me ajude a estruturar a lógica de decodificação e extração das instruções do RISC-V"] 

			// R type (0110011)
			case 0b0110011:
				{
					if(funct7 == 0b0000000 || funct7 == 0b0100000) {
						if(funct3 == 0b000 && funct7 == 0b0000000) { // add
							data = x[rs1] + x[rs2];
							snprintf(campo, sizeof(campo), "add    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x+0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b000 && funct7 == 0b0100000) { // sub
							data = x[rs1] - x[rs2];
							snprintf(campo, sizeof(campo), "sub    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x-0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b100) { // xor
							data = x[rs1] ^ x[rs2];
							snprintf(campo, sizeof(campo), "xor    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x^0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b110) { // or
							data = x[rs1] | x[rs2];
							snprintf(campo, sizeof(campo), "or     %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x|0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b111) { // and
							data = x[rs1] & x[rs2];
							snprintf(campo, sizeof(campo), "and    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x&0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b001) { // sll
							data = x[rs1] << (x[rs2] & 0x1F);
							snprintf(campo, sizeof(campo), "sll    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x<<%u=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2] & 0x1F, data);
						} else if(funct3 == 0b101 && funct7 == 0b0000000) { // srl
							data = x[rs1] >> (x[rs2] & 0x1F);
							snprintf(campo, sizeof(campo), "srl    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x>>%u=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2] & 0x1F, data);
						} else if(funct3 == 0b101 && funct7 == 0b0100000) { // sra
							data = (uint32_t)((int32_t)x[rs1] >> (x[rs2] & 0x1F));
							snprintf(campo, sizeof(campo), "sra    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x>>>%u=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2] & 0x1F, data);
						} else if(funct3 == 0b010) { // slt
							data = ((int32_t)x[rs1] < (int32_t)x[rs2]) ? 1 : 0;
							snprintf(campo, sizeof(campo), "slt    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s(0x%08x<0x%08x)=%u\n", pc, campo, x[rs1], x[rs2], data);
						} else if(funct3 == 0b011) { // sltu
							data = (x[rs1] < x[rs2]) ? 1 : 0;
							snprintf(campo, sizeof(campo), "sltu   %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s(0x%08x<0x%08x)=%u\n", pc, campo, x[rs1], x[rs2], data);
						}
						if(rd != 0) x[rd] = data;
					} else if(funct7 == 0b0000001) {
						const int32_t s_rs1 = (int32_t)x[rs1];
						const int32_t s_rs2 = (int32_t)x[rs2];
						const int64_t full_signed = (int64_t)s_rs1 * (int64_t)s_rs2;
						const uint64_t full_unsigned = (uint64_t)x[rs1] * (uint64_t)x[rs2];
						const int64_t full_mixed = (int64_t)s_rs1 * (int64_t)x[rs2];
						if(funct3 == 0b000) { // mul
							data = (uint32_t)(full_signed & 0xFFFFFFFF);
							snprintf(campo, sizeof(campo), "mul    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x*0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b001) { // mulh
							data = (uint32_t)((uint64_t)full_signed >> 32);
							snprintf(campo, sizeof(campo), "mulh   %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x*0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b010) { // mulhsu
							data = (uint32_t)((uint64_t)full_mixed >> 32);
							snprintf(campo, sizeof(campo), "mulhsu %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x*0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b011) { // mulhu
							data = (uint32_t)(full_unsigned >> 32);
							snprintf(campo, sizeof(campo), "mulhu  %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x*0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b100) { // div
							if(x[rs2] == 0) {data = 0xFFFFFFFF;
							} else if(x[rs1] == 0x80000000 && x[rs2] == 0xFFFFFFFF) {data = 0x80000000;
							} else {data = (uint32_t)(s_rs1 / s_rs2);}							
							snprintf(campo, sizeof(campo), "div    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x/0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b101) { // divu
							data = (x[rs2] == 0) ? 0xFFFFFFFF : (x[rs1] / x[rs2]);
							snprintf(campo, sizeof(campo), "divu   %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x/0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b110) { // rem
							if(x[rs2] == 0) {data = x[rs1];
							} else if(x[rs1] == 0x80000000 && x[rs2] == 0xFFFFFFFF) {data = 0;
							} else {data = (uint32_t)(s_rs1 % s_rs2);}						
							snprintf(campo, sizeof(campo), "rem    %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x%%0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						} else if(funct3 == 0b111) { // remu
							data = (x[rs2] == 0) ? x[rs1] : (x[rs1] % x[rs2]);
							snprintf(campo, sizeof(campo), "remu   %s,%s,%s", x_label[rd], x_label[rs1], x_label[rs2]);
							fprintf(output, "0x%08x:%-27s%s=0x%08x%%0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], x[rs2], data);
						}
						if(rd != 0) x[rd] = data;
					}
				}
				break;

			// I type (0010011) - arithmetic
			case 0b0010011:
				{
					const uint32_t simm = (imm >> 11) ? (0xFFFFF000 | imm) : imm;
					if(funct3 == 0b000) { // addi
						data = x[rs1] + simm;
						snprintf(campo, sizeof(campo), "addi   %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27s%s=0x%08x+0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], simm, data);
					} else if(funct3 == 0b100) { // xori
						data = x[rs1] ^ simm;
						snprintf(campo, sizeof(campo), "xori   %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27s%s=0x%08x^0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], simm, data);
					} else if(funct3 == 0b110) { // ori
						data = x[rs1] | simm;
						snprintf(campo, sizeof(campo), "ori    %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27s%s=0x%08x|0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], simm, data);
					} else if(funct3 == 0b111) { // andi
						data = x[rs1] & simm;
						snprintf(campo, sizeof(campo), "andi   %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27s%s=0x%08x&0x%08x=0x%08x\n", pc, campo, x_label[rd], x[rs1], simm, data);
					} else if(funct3 == 0b010) { // slti
						data = ((int32_t)x[rs1] < (int32_t)simm) ? 1 : 0;
						snprintf(campo, sizeof(campo), "slti   %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x<0x%08x)=%u\n", pc, campo, x[rs1], simm, data);
					} else if(funct3 == 0b011) { // sltiu
						data = (x[rs1] < simm) ? 1 : 0;
						snprintf(campo, sizeof(campo), "sltiu  %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x<0x%08x)=%u\n", pc, campo, x[rs1], simm, data);
					} else if(funct3 == 0b001 && funct7 == 0b0000000) { // slli
						data = x[rs1] << uimm;
						snprintf(campo, sizeof(campo), "slli   %s,%s,%u", x_label[rd], x_label[rs1], uimm);
						fprintf(output, "0x%08x:%-27s%s=0x%08x<<%u=0x%08x\n", pc, campo, x_label[rd], x[rs1], uimm, data);
					} else if(funct3 == 0b101 && funct7 == 0b0000000) { // srli
						data = x[rs1] >> uimm;
						snprintf(campo, sizeof(campo), "srli   %s,%s,%u", x_label[rd], x_label[rs1], uimm);
						fprintf(output, "0x%08x:%-27s%s=0x%08x>>%u=0x%08x\n", pc, campo, x_label[rd], x[rs1], uimm, data);
					} else if(funct3 == 0b101 && funct7 == 0b0100000) { // srai
						data = (uint32_t)((int32_t)x[rs1] >> uimm);
						snprintf(campo, sizeof(campo), "srai   %s,%s,%u", x_label[rd], x_label[rs1], uimm);
						fprintf(output, "0x%08x:%-27s%s=0x%08x>>>%u=0x%08x\n", pc, campo, x_label[rd], x[rs1], uimm, data);
					}
					if(rd != 0) x[rd] = data;
				}
				break;

			// I type (0000011) — loads
			case 0b0000011:
				{
					const uint32_t simm = (imm >> 11) ? (0xFFFFF000 | imm) : imm;
					const uint32_t address = x[rs1] + simm;
					if(funct3 == 0b000) { // lb (load byte, sign-extended)
						const uint8_t byte = mem[address - offset];
						data = (byte & 0x80) ? (0xFFFFFF00 | byte) : byte;
						snprintf(campo, sizeof(campo), "lb     %s,0x%03x(%s)", x_label[rd], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27s%s=mem[0x%08x]=0x%08x\n", pc, campo, x_label[rd], address, data);
					} else if(funct3 == 0b001) { // lh (load half, sign-extended)
						const uint16_t half = mem[address - offset] | (mem[address - offset + 1] << 8);
						data = (half & 0x8000) ? (0xFFFF0000 | half) : half;
						snprintf(campo, sizeof(campo), "lh     %s,0x%03x(%s)", x_label[rd], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27s%s=mem[0x%08x]=0x%08x\n", pc, campo, x_label[rd], address, data);
					} else if(funct3 == 0b010) { // lw (load word, full 32 bits, no extension needed)
						data = mem[address - offset] | (mem[address - offset + 1] << 8) |
						       (mem[address - offset + 2] << 16) | (mem[address - offset + 3] << 24);
						snprintf(campo, sizeof(campo), "lw     %s,0x%03x(%s)", x_label[rd], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27s%s=mem[0x%08x]=0x%08x\n", pc, campo, x_label[rd], address, data);
					} else if(funct3 == 0b100) { // lbu (load byte, zero-extended)
						data = mem[address - offset];
						snprintf(campo, sizeof(campo), "lbu    %s,0x%03x(%s)", x_label[rd], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27s%s=mem[0x%08x]=0x%08x\n", pc, campo, x_label[rd], address, data);
					} else if(funct3 == 0b101) { // lhu (load half, zero-extended)
						data = mem[address - offset] | (mem[address - offset + 1] << 8);
						snprintf(campo, sizeof(campo), "lhu    %s,0x%03x(%s)", x_label[rd], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27s%s=mem[0x%08x]=0x%08x\n", pc, campo, x_label[rd], address, data);
					}
					if(rd != 0) x[rd] = data;
				}
				break;

			// S type (0100011) — stores
			case 0b0100011:
				{
					const uint32_t imm_s = (funct7 << 5) | rd;
					const uint32_t simm = (funct7 >> 6) ? (0xFFFFF000 | imm_s) : imm_s;
					const uint32_t address = x[rs1] + simm;
					if(funct3 == 0b000) { // sb (store byte)
						mem[address - offset] = x[rs2] & 0xFF;
						snprintf(campo, sizeof(campo), "sb     %s,0x%03x(%s)", x_label[rs2], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27smem[0x%08x]=0x%02x\n", pc, campo, address, x[rs2] & 0xFF);
					} else if(funct3 == 0b001) { // sh (store half)
						mem[address - offset]     = (x[rs2] >> 0) & 0xFF;
						mem[address - offset + 1] = (x[rs2] >> 8) & 0xFF;
						snprintf(campo, sizeof(campo), "sh     %s,0x%03x(%s)", x_label[rs2], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27smem[0x%08x]=0x%04x\n", pc, campo, address, x[rs2] & 0xFFFF);
					} else if(funct3 == 0b010) { // sw (store word)
						mem[address - offset]     = (x[rs2] >>  0) & 0xFF;
						mem[address - offset + 1] = (x[rs2] >>  8) & 0xFF;
						mem[address - offset + 2] = (x[rs2] >> 16) & 0xFF;
						mem[address - offset + 3] = (x[rs2] >> 24) & 0xFF;
						snprintf(campo, sizeof(campo), "sw     %s,0x%03x(%s)", x_label[rs2], simm & 0xFFF, x_label[rs1]);
						fprintf(output, "0x%08x:%-27smem[0x%08x]=0x%08x\n", pc, campo, address, x[rs2]);
					}
				}
				break;
		
			// B type (1100011) — branches
			case 0b1100011:
				{
					const uint32_t bit12 = (instruction >> 31) & 0x1;
					const uint32_t bit11 = (instruction >> 7) & 0x1;
					const uint32_t bits10_5 = (instruction >> 25) & 0x3F;
					const uint32_t bits4_1 = (instruction >> 8) & 0xF;
					const uint32_t imm_b = (bit12 << 12) | (bit11 << 11) | (bits10_5 << 5) | (bits4_1 << 1);
					const uint32_t simm = bit12 ? (0xFFFFE000 | imm_b) : imm_b;
					uint8_t taken = 0;
					if(funct3 == 0b000) { // beq
						taken = (x[rs1] == x[rs2]) ? 1 : 0;
						snprintf(campo, sizeof(campo), "beq    %s,%s,0x%03x", x_label[rs1], x_label[rs2], (imm_b >> 1) & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x==0x%08x)=%u->pc=0x%08x\n", pc, campo, x[rs1], x[rs2], taken, taken ? (pc + simm) : (pc + 4));
					} else if(funct3 == 0b001) { // bne
						taken = (x[rs1] != x[rs2]) ? 1 : 0;
						snprintf(campo, sizeof(campo), "bne    %s,%s,0x%03x", x_label[rs1], x_label[rs2], (imm_b >> 1) & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x!=0x%08x)=%u->pc=0x%08x\n", pc, campo, x[rs1], x[rs2], taken, taken ? (pc + simm) : (pc + 4));
					} else if(funct3 == 0b100) { // blt (signed)
						taken = ((int32_t)x[rs1] < (int32_t)x[rs2]) ? 1 : 0;
						snprintf(campo, sizeof(campo), "blt    %s,%s,0x%03x", x_label[rs1], x_label[rs2], (imm_b >> 1) & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x<0x%08x)=%u->pc=0x%08x\n", pc, campo, x[rs1], x[rs2], taken, taken ? (pc + simm) : (pc + 4));
					} else if(funct3 == 0b101) { // bge (signed)
						taken = ((int32_t)x[rs1] >= (int32_t)x[rs2]) ? 1 : 0;
						snprintf(campo, sizeof(campo), "bge    %s,%s,0x%03x", x_label[rs1], x_label[rs2], (imm_b >> 1) & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x>=0x%08x)=%u->pc=0x%08x\n", pc, campo, x[rs1], x[rs2], taken, taken ? (pc + simm) : (pc + 4));
					} else if(funct3 == 0b110) { // bltu (unsigned)
						taken = (x[rs1] < x[rs2]) ? 1 : 0;
						snprintf(campo, sizeof(campo), "bltu   %s,%s,0x%03x", x_label[rs1], x_label[rs2], (imm_b >> 1) & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x<0x%08x)=%u->pc=0x%08x\n", pc, campo, x[rs1], x[rs2], taken, taken ? (pc + simm) : (pc + 4));
					} else if(funct3 == 0b111) { // bgeu (unsigned)
						taken = (x[rs1] >= x[rs2]) ? 1 : 0;
						snprintf(campo, sizeof(campo), "bgeu   %s,%s,0x%03x", x_label[rs1], x_label[rs2], (imm_b >> 1) & 0xFFF);
						fprintf(output, "0x%08x:%-27s(0x%08x>=0x%08x)=%u->pc=0x%08x\n", pc, campo, x[rs1], x[rs2], taken, taken ? (pc + simm) : (pc + 4));
					}
					if(taken) pc = pc + simm - 4;
				}
				break;

			// J type (1101111)
			case 0b1101111:
				{
					const uint32_t simm = (imm20 >> 19) ? (0xFFF00000 | imm20) : (imm20);
					const uint32_t address = pc + (simm << 1);
					data = pc + 4;
					snprintf(campo, sizeof(campo), "jal    %s,0x%05x", x_label[rd], imm20);
					fprintf(output, "0x%08x:%-27spc=0x%08x,%s=0x%08x\n", pc, campo, address, x_label[rd], data);
					if(rd != 0) x[rd] = data;
					pc = address - 4;
				}
				break;

			// U type (0110111) — lui
			case 0b0110111:
				{
					const uint32_t u_imm = instruction >> 12;
					data = u_imm << 12;
					snprintf(campo, sizeof(campo), "lui    %s,0x%05x", x_label[rd], u_imm);
					fprintf(output, "0x%08x:%-27s%s=0x%05x000\n", pc, campo, x_label[rd], u_imm);
					if(rd != 0) x[rd] = data;
				}
				break;

			// I type (1110011) — ebreak
			case 0b1110011:
				{
					if(funct3 == 0b000 && imm == 1) { // ebreak
						fprintf(output, "0x%08x:ebreak\n", pc);
						// Retrieving previous and next instructions
						const uint32_t previous = ((uint32_t*)(mem))[(pc - 4 - offset) >> 2];
						const uint32_t next = ((uint32_t*)(mem))[(pc + 4 - offset) >> 2];
						if(previous == 0x01f01013 && next == 0x40705013) run = 0;
					}
				}
				break;
			
			// I type (1100111) — jalr
			case 0b1100111:
				{
					if(funct3 == 0b000) {
						const uint32_t simm = (imm >> 11) ? (0xFFFFF000 | imm) : imm;
						const uint32_t address = (x[rs1] + simm) & 0xFFFFFFFE;
						data = pc + 4;
						snprintf(campo, sizeof(campo), "jalr   %s,%s,0x%03x", x_label[rd], x_label[rs1], simm & 0xFFF);
						fprintf(output, "0x%08x:%-27spc=0x%08x+0x%08x,%s=0x%08x\n", pc, campo, x[rs1], simm, x_label[rd], data);
						if(rd != 0) x[rd] = data;
						pc = address - 4;
					}
				}
				break;


			// U type (0010111) — auipc
			case 0b0010111:
				{
					const uint32_t u_imm = instruction >> 12;
					data = pc + (u_imm << 12);
					snprintf(campo, sizeof(campo), "auipc  %s,0x%05x", x_label[rd], u_imm);
					fprintf(output, "0x%08x:%-27s%s=0x%08x+0x%05x000=0x%08x\n", pc, campo, x_label[rd], pc, u_imm, data);
					if(rd != 0) x[rd] = data;
				}
				break;

			default:
				run = 0;
		}
		pc = pc + 4;
	}

	// Removing the final newline character to match reference output exactly
	// #IA #CLAUDE["Como remover a quebra de linha final de um arquivo em C depois de fechar de escrever nele?"]
	long tamanho = ftell(output);
	if(tamanho > 0) {
		fseek(output, -1, SEEK_END);
		int ultimo = fgetc(output);
		if(ultimo == '\n') {
			fflush(output);
			if(ftruncate(fileno(output), tamanho - 1) != 0) {
				printf("aviso: nao foi possivel truncar o arquivo de saida\n");
			}
		}
	}

	// Closing input and output files
	fclose(input);
	fclose(output);

	// Outputting separator
	printf("--------------------------------------------------------------------------------\n");
	// Returning success status
	return 0;
}
