# -*- coding: utf-8 -*-

numberSlaves = int(input("Quantidade de PEs: "))
numberSlaves -= 2
inputVector = [
	[2,5,1,8,11,7,3],         #1
	[-2,5,-1,-8,11,7,3],      #
	[2,6,5],                  #
	[-5,6,8,-3,8],            #
	[-8,9,6,-2,5,7],          #
	[5,9,6,2,-6,-8,-3],       #
	[4,3,-9,-7,-5,-2,1],      #
	[1,6,9,-2,-8,5,3],        #
	[5,-9,-8,-5,3,4,7,6,2],   #
	[5,9,-6,-4,2,8,-6,-5],    #10
	[5,-9,-6,-3,8,4,3,6,9],   #
	[5,9,6,-4,-2,8,5,4,3],    #
	[-2,-6,-5,9,6,8,4,-2,-3], #
	[2,-3,6,9,-5,-8,-2,4,1],  #
	[-2,6,3,5,-9,-6,-8,1,2],  #
	[2,-6,-8],                #
	[2,6,-9,5],               #
	[3,-2,-6,-4,1,5,6],       #
	[6,-5,-8,1,9,7],          #
	[6,-5,-9,-4,2,6],         #20
	[5,9,-6,-5,],             #
	[5,-9,-5,4,8],            #
	[5,9,3,5],                #
	[2,-3,-6,-5,1],           #
	[5,-6,-8,-5],             #
	[2,9,-5,-4,-2],           #
	[2,-3,-6,-4,1,2,8,5],     #
	[3,6,5,-9],               #
	[5,6,2,-9,-8],            #
	[5,9,6,-4,-2,8,5,4,3],    #30
	[-2,-6,-5,9,6,8,4,-2,-3], #
	[2,-3,6,9,-5,-8,-2,4,1],  #
	[-2,6,3,5,-9,-6,-8,1,2],  #
	[-8,9,6,-2,5,7],          #
	[5,9,6,2,-6,-8,-3],       #
	[4,3,-9,-7,-5,-2,1]       #36
]
fileNameSlave = "slave"   # .c
fileNamePrint = "print"   # .c
fileNameMaster = "master" # .c

master = open(fileNameMaster + ".c", "w+");
master.write("#include <stdlib.h>\n")
master.write("#include <stdio.h>\n")
master.write("#include \"MPIe.h\"\n")
master.write("#include \"tasks_comm.h\"\n\n")
master.write("int main (){\n")
master.write("\tMemoryWrite(INITIALIZE_ROUTER, 0);\n\n")
master.write("\t// Declara a estrutura de mensagem a ser enviada e o vetor\n")
master.write("\tMessage msg;\n")
for slaveid in range(0, numberSlaves):
	master.write("\tint inputVector" + str(slaveid) + "[] = {")
	master.write(str(inputVector[slaveid][0]))
	for x in range(1,len(inputVector[slaveid])):
		master.write(", " + str(inputVector[slaveid][x]))
	master.write("};\n")
	master.write("\tint vectorSize" + str(slaveid) + " = sizeof(inputVector" + str(slaveid) + ") / sizeof(inputVector" + str(slaveid) + "[0]);\n\n")
master.write("\tprintf(\"Inicio da aplicação " + fileNameMaster + ".c \\n\");\n\n")
for slaveid in range(0, numberSlaves):
	master.write("\t// Define o tamanho da mensagem como o tamanho do vetor\n")
	# master.write("\tprintf(\"---------------------\\n\");\n")
	# master.write("\tprintf(\" Tamanho do vetor: %d \\n\", vectorSize" + str(slaveid) + " );\n")
	master.write("\tmsg.length = vectorSize" + str(slaveid) + ";\n")
	master.write("\t// Percorre o vetor e armazena o conteúdo dele na mensagem a ser enviada\n")
	# master.write("\tprintf(\"SLAVE" + str(slaveid) + " - inputVector" + str(slaveid) + " content: \\n\");\n")
	master.write("\tfor (int i=0; i<vectorSize" + str(slaveid) + "; i++){\n")
	master.write("\t\tmsg.msg[i] = inputVector" + str(slaveid) + "[i];\n")
	# master.write("\t\tprintf(\"\\tSLAVE" + slaveid + " - msg.msg[%d] = %d \\n\", i, msg.msg[i]);\n")
	master.write("\t}\n")
	# master.write("\tprintf(\"---------------------\\n\");\n")
	master.write("\t// Envia a mensagem para o escravo\n")
	master.write("\tSend(msg, " + fileNameSlave + str(slaveid) + ");\n\n")
master.write("\t// Aguarda até receber a finalização\n")
master.write("\tmsg = Receive();\n\n")
master.write("\tprintf(\"Fim da aplicacao " + fileNameMaster + ".c\\n\");\n\n")
master.write("\tMemoryWrite(END_SIM, " + str(0) + ");\n\n")
master.write("\treturn 0;\n")
master.write("}")


for slaveid in range(0, numberSlaves):
	slave = open(fileNameSlave + str(slaveid) + ".c", "w+");
	slave.write("#include <stdlib.h>\n")
	slave.write("#include <stdio.h>\n")
	slave.write("#include \"MPIe.h\"\n")
	slave.write("#include \"tasks_comm.h\"\n\n")
	slave.write("int getLivesCount(int inputVector[], int vectorSize){\n")
	slave.write("\t// Inicializa a variável de somatório com 0\n")
	slave.write("\tint sum = 0;\n")
	slave.write("\t// Inicializa o melhor número de vidas com a primeira posição.\n")
	slave.write("\tint livesCount = inputVector[0];\n\n")
	slave.write("\t// Percorre o vetor encontrando o melhor caso\n")
	slave.write("\tfor(int i=0; i<vectorSize; i++){\n")
	slave.write("\t\tfor(int j=i; j<vectorSize; j++){\n")
	slave.write("\t\t\tsum += inputVector[j];\n")
	slave.write("\t\t\t// Se o somatório for maior, atualiza o número de vidas\n")
	slave.write("\t\t\tif(sum > livesCount)\n")
	slave.write("\t\t\t\tlivesCount = sum;\n")
	slave.write("\t\t}\n")
	slave.write("\t\tsum = 0;\n")
	slave.write("\t}\n\n")
	slave.write("\t// Retorna o maior número de vidas encontrado\n")
	slave.write("\treturn livesCount;\n")
	slave.write("}\n\n")
	slave.write("int main(){\n")
	slave.write("\tMemoryWrite(INITIALIZE_ROUTER, " + str(slaveid+2) + ");\n\n")
	slave.write("\t// Declara a estrutura de mensagem a ser recebida e enviada\n")
	slave.write("\tMessage msg;\n\n")
	slave.write("\tprintf(\"" + fileNameSlave + str(slaveid+2) + ".c starting ...\\n\");\n\n")
	slave.write("\t// Aguarda até receber o vetor\n")
	slave.write("\tmsg = Receive();\n\n")
	slave.write("\t// Extrai o maior número de vidas\n")
	slave.write("\tmsg.msg[0] = getLivesCount(msg.msg, msg.length);\n\n")
	slave.write("\t// Envia a mensagem com o resultado na primeira posição do vetor\n")
	slave.write("\tSend(msg, print);\n\n")
	slave.write("\tprintf(\"" + fileNameSlave + str(slaveid+2) + ".c finishing ...\\n\");\n\n")
	slave.write("\t// Aguarda até receber a finalização\n")
	slave.write("\tmsg = Receive();\n\n")
	slave.write("\tMemoryWrite(END_SIM, " + str(slaveid+2) + ");\n\n")
	slave.write("\treturn 0;\n")
	slave.write("}")


printF = open("print.c", "w+");
printF.write("#include <stdlib.h>\n")
printF.write("#include <stdio.h>\n")
printF.write("#include \"MPIe.h\"\n")
printF.write("#include \"tasks_comm.h\"\n\n")
printF.write("int main(){\n")
printF.write("\tMemoryWrite(INITIALIZE_ROUTER, 1);\n\n")
printF.write("\t// Declara a estrutura de mensagem a ser recebida\n")
printF.write("\tMessage msg;\n\n")
printF.write("\tprintf(\"" + fileNamePrint + ".c starting...\\n\");\n\n");
for slaveid in range(0, numberSlaves):
	printF.write("\t// Aguarda até receber o número de vidas\n")
	printF.write("\tmsg = Receive();\n")
	printF.write("\t// Apresenta na tela o número de vidas\n")
	printF.write("\tprintf(\"lives slave" + str(slaveid) + ": %d \\n\", msg.msg[0]); \n\n")
printF.write("\t// Envia sinal de finalização\n")
printF.write("\tmsg.msg[0] = 999;\n")
printF.write("\tSend(msg, " + fileNameMaster + ");\n")
for slaveid in range(0, numberSlaves):
	printF.write("\tSend(msg, " + fileNameSlave + str(slaveid) + ");\n")
printF.write("\n\tprintf(\"" + fileNamePrint + ".c finishing ...\\n\");\n")
printF.write("\tMemoryWrite(END_SIM, " + str(1) + ");\n\n")
printF.write("\treturn 0;\n")
printF.write("}")

