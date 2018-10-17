#include <stdlib.h>
#include <stdio.h>
#include "MPIe.h"
#include "tasks_comm.h"

#define NUM_NODES                  16		//16 for small input; 160 for large input; 30 for medium input;
#define MAXPROCESSORS			   64		//The amount of processor
#define NPROC 					   143

int PROCESSORS;
int pthread_n_workers;
int paths;
int tasks[MAXPROCESSORS][2];
int nodes_tasks[NUM_NODES*(NUM_NODES-1)/2][2];
int AdjMatrix[NUM_NODES][NUM_NODES];
int *result;
int ended = 0;

void startThreads(void) {
	int i, j;
	Message msg;

	/* SEND nodes_tasks[NUM_NODES*(NUM_NODES-1)/2][2] */
	msg.length = NUM_NODES*(NUM_NODES-1)/2;
	// Send X of nodes_tasks
	for (i=0; i<(NUM_NODES*(NUM_NODES-1)/2); i++)
		msg.msg[i] = nodes_tasks[i][0];
	Send(msg, dijkstra_0);
	Send(msg, dijkstra_1);
	Send(msg, dijkstra_2);
	Send(msg, dijkstra_3);
	Send(msg, dijkstra_4);
	Send(msg, dijkstra_5);
	Send(msg, dijkstra_6);
	Send(msg, dijkstra_7);
	Send(msg, dijkstra_8);
	Send(msg, dijkstra_9);
	Send(msg, dijkstra_10);
	Send(msg, dijkstra_11);
	Send(msg, dijkstra_12);
	Send(msg, dijkstra_13);
	Send(msg, dijkstra_14);
	Send(msg, dijkstra_15);
	Send(msg, dijkstra_16);
	Send(msg, dijkstra_17);
	Send(msg, dijkstra_18);
	Send(msg, dijkstra_19);
	Send(msg, dijkstra_20);
	Send(msg, dijkstra_21);
	Send(msg, dijkstra_22);
	Send(msg, dijkstra_23);
	Send(msg, dijkstra_24);
	Send(msg, dijkstra_25);
	Send(msg, dijkstra_26);
	Send(msg, dijkstra_27);
	Send(msg, dijkstra_28);
	Send(msg, dijkstra_29);
	Send(msg, dijkstra_30);
	Send(msg, dijkstra_31);
	Send(msg, dijkstra_32);
	Send(msg, dijkstra_33);
	Send(msg, dijkstra_34);
	Send(msg, dijkstra_35);
	Send(msg, dijkstra_36);
	Send(msg, dijkstra_37);
	Send(msg, dijkstra_38);
	Send(msg, dijkstra_39);
	Send(msg, dijkstra_40);
	Send(msg, dijkstra_41);
	Send(msg, dijkstra_42);
	Send(msg, dijkstra_43);
	Send(msg, dijkstra_44);
	Send(msg, dijkstra_45);
	Send(msg, dijkstra_46);
	Send(msg, dijkstra_47);
	Send(msg, dijkstra_48);
	Send(msg, dijkstra_49);
	Send(msg, dijkstra_50);
	Send(msg, dijkstra_51);
	Send(msg, dijkstra_52);
	Send(msg, dijkstra_53);
	Send(msg, dijkstra_54);
	Send(msg, dijkstra_55);
	Send(msg, dijkstra_56);
	Send(msg, dijkstra_57);
	Send(msg, dijkstra_58);
	Send(msg, dijkstra_59);
	Send(msg, dijkstra_60);
	Send(msg, dijkstra_61);
	Send(msg, dijkstra_62);
	Send(msg, dijkstra_63);
	Send(msg, dijkstra_64);
	Send(msg, dijkstra_65);
	Send(msg, dijkstra_66);
	Send(msg, dijkstra_67);
	Send(msg, dijkstra_68);
	Send(msg, dijkstra_69);
	Send(msg, dijkstra_70);
	Send(msg, dijkstra_71);
	Send(msg, dijkstra_72);
	Send(msg, dijkstra_73);
	Send(msg, dijkstra_74);
	Send(msg, dijkstra_75);
	Send(msg, dijkstra_76);
	Send(msg, dijkstra_77);
	Send(msg, dijkstra_78);
	Send(msg, dijkstra_79);
	Send(msg, dijkstra_80);
	Send(msg, dijkstra_81);
	Send(msg, dijkstra_82);
	Send(msg, dijkstra_83);
	Send(msg, dijkstra_84);
	Send(msg, dijkstra_85);
	Send(msg, dijkstra_86);
	Send(msg, dijkstra_87);
	Send(msg, dijkstra_88);
	Send(msg, dijkstra_89);
	Send(msg, dijkstra_90);
	Send(msg, dijkstra_91);
	Send(msg, dijkstra_92);
	Send(msg, dijkstra_93);
	Send(msg, dijkstra_94);
	Send(msg, dijkstra_95);
	Send(msg, dijkstra_96);
	Send(msg, dijkstra_97);
	Send(msg, dijkstra_98);
	Send(msg, dijkstra_99);
	Send(msg, dijkstra_100);
	Send(msg, dijkstra_101);
	Send(msg, dijkstra_102);
	Send(msg, dijkstra_103);
	Send(msg, dijkstra_104);
	Send(msg, dijkstra_105);
	Send(msg, dijkstra_106);
	Send(msg, dijkstra_107);
	Send(msg, dijkstra_108);
	Send(msg, dijkstra_109);
	Send(msg, dijkstra_110);
	Send(msg, dijkstra_111);
	Send(msg, dijkstra_112);
	Send(msg, dijkstra_113);
	Send(msg, dijkstra_114);
	Send(msg, dijkstra_115);
	Send(msg, dijkstra_116);
	Send(msg, dijkstra_117);
	Send(msg, dijkstra_118);
	Send(msg, dijkstra_119);
	Send(msg, dijkstra_120);
	Send(msg, dijkstra_121);
	Send(msg, dijkstra_122);
	Send(msg, dijkstra_123);
	Send(msg, dijkstra_124);
	Send(msg, dijkstra_125);
	Send(msg, dijkstra_126);
	Send(msg, dijkstra_127);
	Send(msg, dijkstra_128);
	Send(msg, dijkstra_129);
	Send(msg, dijkstra_130);
	Send(msg, dijkstra_131);
	Send(msg, dijkstra_132);
	Send(msg, dijkstra_133);
	Send(msg, dijkstra_134);
	Send(msg, dijkstra_135);
	Send(msg, dijkstra_136);
	Send(msg, dijkstra_137);
	Send(msg, dijkstra_138);
	Send(msg, dijkstra_139);
	Send(msg, dijkstra_140);
	Send(msg, dijkstra_141);
	Send(msg, dijkstra_142);

	// Send Y of nodes_tasks
	for (i=0; i<(NUM_NODES*(NUM_NODES-1)/2); i++)
		msg.msg[i] = nodes_tasks[i][1];
	Send(msg, dijkstra_0);
	Send(msg, dijkstra_1);
	Send(msg, dijkstra_2);
	Send(msg, dijkstra_3);
	Send(msg, dijkstra_4);
	Send(msg, dijkstra_5);
	Send(msg, dijkstra_6);
	Send(msg, dijkstra_7);
	Send(msg, dijkstra_8);
	Send(msg, dijkstra_9);
	Send(msg, dijkstra_10);
	Send(msg, dijkstra_11);
	Send(msg, dijkstra_12);
	Send(msg, dijkstra_13);
	Send(msg, dijkstra_14);
	Send(msg, dijkstra_15);
	Send(msg, dijkstra_16);
	Send(msg, dijkstra_17);
	Send(msg, dijkstra_18);
	Send(msg, dijkstra_19);
	Send(msg, dijkstra_20);
	Send(msg, dijkstra_21);
	Send(msg, dijkstra_22);
	Send(msg, dijkstra_23);
	Send(msg, dijkstra_24);
	Send(msg, dijkstra_25);
	Send(msg, dijkstra_26);
	Send(msg, dijkstra_27);
	Send(msg, dijkstra_28);
	Send(msg, dijkstra_29);
	Send(msg, dijkstra_30);
	Send(msg, dijkstra_31);
	Send(msg, dijkstra_32);
	Send(msg, dijkstra_33);
	Send(msg, dijkstra_34);
	Send(msg, dijkstra_35);
	Send(msg, dijkstra_36);
	Send(msg, dijkstra_37);
	Send(msg, dijkstra_38);
	Send(msg, dijkstra_39);
	Send(msg, dijkstra_40);
	Send(msg, dijkstra_41);
	Send(msg, dijkstra_42);
	Send(msg, dijkstra_43);
	Send(msg, dijkstra_44);
	Send(msg, dijkstra_45);
	Send(msg, dijkstra_46);
	Send(msg, dijkstra_47);
	Send(msg, dijkstra_48);
	Send(msg, dijkstra_49);
	Send(msg, dijkstra_50);
	Send(msg, dijkstra_51);
	Send(msg, dijkstra_52);
	Send(msg, dijkstra_53);
	Send(msg, dijkstra_54);
	Send(msg, dijkstra_55);
	Send(msg, dijkstra_56);
	Send(msg, dijkstra_57);
	Send(msg, dijkstra_58);
	Send(msg, dijkstra_59);
	Send(msg, dijkstra_60);
	Send(msg, dijkstra_61);
	Send(msg, dijkstra_62);
	Send(msg, dijkstra_63);
	Send(msg, dijkstra_64);
	Send(msg, dijkstra_65);
	Send(msg, dijkstra_66);
	Send(msg, dijkstra_67);
	Send(msg, dijkstra_68);
	Send(msg, dijkstra_69);
	Send(msg, dijkstra_70);
	Send(msg, dijkstra_71);
	Send(msg, dijkstra_72);
	Send(msg, dijkstra_73);
	Send(msg, dijkstra_74);
	Send(msg, dijkstra_75);
	Send(msg, dijkstra_76);
	Send(msg, dijkstra_77);
	Send(msg, dijkstra_78);
	Send(msg, dijkstra_79);
	Send(msg, dijkstra_80);
	Send(msg, dijkstra_81);
	Send(msg, dijkstra_82);
	Send(msg, dijkstra_83);
	Send(msg, dijkstra_84);
	Send(msg, dijkstra_85);
	Send(msg, dijkstra_86);
	Send(msg, dijkstra_87);
	Send(msg, dijkstra_88);
	Send(msg, dijkstra_89);
	Send(msg, dijkstra_90);
	Send(msg, dijkstra_91);
	Send(msg, dijkstra_92);
	Send(msg, dijkstra_93);
	Send(msg, dijkstra_94);
	Send(msg, dijkstra_95);
	Send(msg, dijkstra_96);
	Send(msg, dijkstra_97);
	Send(msg, dijkstra_98);
	Send(msg, dijkstra_99);
	Send(msg, dijkstra_100);
	Send(msg, dijkstra_101);
	Send(msg, dijkstra_102);
	Send(msg, dijkstra_103);
	Send(msg, dijkstra_104);
	Send(msg, dijkstra_105);
	Send(msg, dijkstra_106);
	Send(msg, dijkstra_107);
	Send(msg, dijkstra_108);
	Send(msg, dijkstra_109);
	Send(msg, dijkstra_110);
	Send(msg, dijkstra_111);
	Send(msg, dijkstra_112);
	Send(msg, dijkstra_113);
	Send(msg, dijkstra_114);
	Send(msg, dijkstra_115);
	Send(msg, dijkstra_116);
	Send(msg, dijkstra_117);
	Send(msg, dijkstra_118);
	Send(msg, dijkstra_119);
	Send(msg, dijkstra_120);
	Send(msg, dijkstra_121);
	Send(msg, dijkstra_122);
	Send(msg, dijkstra_123);
	Send(msg, dijkstra_124);
	Send(msg, dijkstra_125);
	Send(msg, dijkstra_126);
	Send(msg, dijkstra_127);
	Send(msg, dijkstra_128);
	Send(msg, dijkstra_129);
	Send(msg, dijkstra_130);
	Send(msg, dijkstra_131);
	Send(msg, dijkstra_132);
	Send(msg, dijkstra_133);
	Send(msg, dijkstra_134);
	Send(msg, dijkstra_135);
	Send(msg, dijkstra_136);
	Send(msg, dijkstra_137);
	Send(msg, dijkstra_138);
	Send(msg, dijkstra_139);
	Send(msg, dijkstra_140);
	Send(msg, dijkstra_141);
	Send(msg, dijkstra_142);

	/* SEND tasks[MAXPROCESSORS][2] */
	msg.length = MAXPROCESSORS;
	// Send X of tasks
	for (i=0; i<MAXPROCESSORS; i++)
		msg.msg[i] = tasks[i][0];
	Send(msg, dijkstra_0);
	Send(msg, dijkstra_1);
	Send(msg, dijkstra_2);
	Send(msg, dijkstra_3);
	Send(msg, dijkstra_4);
	Send(msg, dijkstra_5);
	Send(msg, dijkstra_6);
	Send(msg, dijkstra_7);
	Send(msg, dijkstra_8);
	Send(msg, dijkstra_9);
	Send(msg, dijkstra_10);
	Send(msg, dijkstra_11);
	Send(msg, dijkstra_12);
	Send(msg, dijkstra_13);
	Send(msg, dijkstra_14);
	Send(msg, dijkstra_15);
	Send(msg, dijkstra_16);
	Send(msg, dijkstra_17);
	Send(msg, dijkstra_18);
	Send(msg, dijkstra_19);
	Send(msg, dijkstra_20);
	Send(msg, dijkstra_21);
	Send(msg, dijkstra_22);
	Send(msg, dijkstra_23);
	Send(msg, dijkstra_24);
	Send(msg, dijkstra_25);
	Send(msg, dijkstra_26);
	Send(msg, dijkstra_27);
	Send(msg, dijkstra_28);
	Send(msg, dijkstra_29);
	Send(msg, dijkstra_30);
	Send(msg, dijkstra_31);
	Send(msg, dijkstra_32);
	Send(msg, dijkstra_33);
	Send(msg, dijkstra_34);
	Send(msg, dijkstra_35);
	Send(msg, dijkstra_36);
	Send(msg, dijkstra_37);
	Send(msg, dijkstra_38);
	Send(msg, dijkstra_39);
	Send(msg, dijkstra_40);
	Send(msg, dijkstra_41);
	Send(msg, dijkstra_42);
	Send(msg, dijkstra_43);
	Send(msg, dijkstra_44);
	Send(msg, dijkstra_45);
	Send(msg, dijkstra_46);
	Send(msg, dijkstra_47);
	Send(msg, dijkstra_48);
	Send(msg, dijkstra_49);
	Send(msg, dijkstra_50);
	Send(msg, dijkstra_51);
	Send(msg, dijkstra_52);
	Send(msg, dijkstra_53);
	Send(msg, dijkstra_54);
	Send(msg, dijkstra_55);
	Send(msg, dijkstra_56);
	Send(msg, dijkstra_57);
	Send(msg, dijkstra_58);
	Send(msg, dijkstra_59);
	Send(msg, dijkstra_60);
	Send(msg, dijkstra_61);
	Send(msg, dijkstra_62);
	Send(msg, dijkstra_63);
	Send(msg, dijkstra_64);
	Send(msg, dijkstra_65);
	Send(msg, dijkstra_66);
	Send(msg, dijkstra_67);
	Send(msg, dijkstra_68);
	Send(msg, dijkstra_69);
	Send(msg, dijkstra_70);
	Send(msg, dijkstra_71);
	Send(msg, dijkstra_72);
	Send(msg, dijkstra_73);
	Send(msg, dijkstra_74);
	Send(msg, dijkstra_75);
	Send(msg, dijkstra_76);
	Send(msg, dijkstra_77);
	Send(msg, dijkstra_78);
	Send(msg, dijkstra_79);
	Send(msg, dijkstra_80);
	Send(msg, dijkstra_81);
	Send(msg, dijkstra_82);
	Send(msg, dijkstra_83);
	Send(msg, dijkstra_84);
	Send(msg, dijkstra_85);
	Send(msg, dijkstra_86);
	Send(msg, dijkstra_87);
	Send(msg, dijkstra_88);
	Send(msg, dijkstra_89);
	Send(msg, dijkstra_90);
	Send(msg, dijkstra_91);
	Send(msg, dijkstra_92);
	Send(msg, dijkstra_93);
	Send(msg, dijkstra_94);
	Send(msg, dijkstra_95);
	Send(msg, dijkstra_96);
	Send(msg, dijkstra_97);
	Send(msg, dijkstra_98);
	Send(msg, dijkstra_99);
	Send(msg, dijkstra_100);
	Send(msg, dijkstra_101);
	Send(msg, dijkstra_102);
	Send(msg, dijkstra_103);
	Send(msg, dijkstra_104);
	Send(msg, dijkstra_105);
	Send(msg, dijkstra_106);
	Send(msg, dijkstra_107);
	Send(msg, dijkstra_108);
	Send(msg, dijkstra_109);
	Send(msg, dijkstra_110);
	Send(msg, dijkstra_111);
	Send(msg, dijkstra_112);
	Send(msg, dijkstra_113);
	Send(msg, dijkstra_114);
	Send(msg, dijkstra_115);
	Send(msg, dijkstra_116);
	Send(msg, dijkstra_117);
	Send(msg, dijkstra_118);
	Send(msg, dijkstra_119);
	Send(msg, dijkstra_120);
	Send(msg, dijkstra_121);
	Send(msg, dijkstra_122);
	Send(msg, dijkstra_123);
	Send(msg, dijkstra_124);
	Send(msg, dijkstra_125);
	Send(msg, dijkstra_126);
	Send(msg, dijkstra_127);
	Send(msg, dijkstra_128);
	Send(msg, dijkstra_129);
	Send(msg, dijkstra_130);
	Send(msg, dijkstra_131);
	Send(msg, dijkstra_132);
	Send(msg, dijkstra_133);
	Send(msg, dijkstra_134);
	Send(msg, dijkstra_135);
	Send(msg, dijkstra_136);
	Send(msg, dijkstra_137);
	Send(msg, dijkstra_138);
	Send(msg, dijkstra_139);
	Send(msg, dijkstra_140);
	Send(msg, dijkstra_141);
	Send(msg, dijkstra_142);

	// Send Y of tasks
	for (i=0; i<MAXPROCESSORS; i++)
		msg.msg[i] = tasks[i][1];
	Send(msg, dijkstra_0);
	Send(msg, dijkstra_1);
	Send(msg, dijkstra_2);
	Send(msg, dijkstra_3);
	Send(msg, dijkstra_4);
	Send(msg, dijkstra_5);
	Send(msg, dijkstra_6);
	Send(msg, dijkstra_7);
	Send(msg, dijkstra_8);
	Send(msg, dijkstra_9);
	Send(msg, dijkstra_10);
	Send(msg, dijkstra_11);
	Send(msg, dijkstra_12);
	Send(msg, dijkstra_13);
	Send(msg, dijkstra_14);
	Send(msg, dijkstra_15);
	Send(msg, dijkstra_16);
	Send(msg, dijkstra_17);
	Send(msg, dijkstra_18);
	Send(msg, dijkstra_19);
	Send(msg, dijkstra_20);
	Send(msg, dijkstra_21);
	Send(msg, dijkstra_22);
	Send(msg, dijkstra_23);
	Send(msg, dijkstra_24);
	Send(msg, dijkstra_25);
	Send(msg, dijkstra_26);
	Send(msg, dijkstra_27);
	Send(msg, dijkstra_28);
	Send(msg, dijkstra_29);
	Send(msg, dijkstra_30);
	Send(msg, dijkstra_31);
	Send(msg, dijkstra_32);
	Send(msg, dijkstra_33);
	Send(msg, dijkstra_34);
	Send(msg, dijkstra_35);
	Send(msg, dijkstra_36);
	Send(msg, dijkstra_37);
	Send(msg, dijkstra_38);
	Send(msg, dijkstra_39);
	Send(msg, dijkstra_40);
	Send(msg, dijkstra_41);
	Send(msg, dijkstra_42);
	Send(msg, dijkstra_43);
	Send(msg, dijkstra_44);
	Send(msg, dijkstra_45);
	Send(msg, dijkstra_46);
	Send(msg, dijkstra_47);
	Send(msg, dijkstra_48);
	Send(msg, dijkstra_49);
	Send(msg, dijkstra_50);
	Send(msg, dijkstra_51);
	Send(msg, dijkstra_52);
	Send(msg, dijkstra_53);
	Send(msg, dijkstra_54);
	Send(msg, dijkstra_55);
	Send(msg, dijkstra_56);
	Send(msg, dijkstra_57);
	Send(msg, dijkstra_58);
	Send(msg, dijkstra_59);
	Send(msg, dijkstra_60);
	Send(msg, dijkstra_61);
	Send(msg, dijkstra_62);
	Send(msg, dijkstra_63);
	Send(msg, dijkstra_64);
	Send(msg, dijkstra_65);
	Send(msg, dijkstra_66);
	Send(msg, dijkstra_67);
	Send(msg, dijkstra_68);
	Send(msg, dijkstra_69);
	Send(msg, dijkstra_70);
	Send(msg, dijkstra_71);
	Send(msg, dijkstra_72);
	Send(msg, dijkstra_73);
	Send(msg, dijkstra_74);
	Send(msg, dijkstra_75);
	Send(msg, dijkstra_76);
	Send(msg, dijkstra_77);
	Send(msg, dijkstra_78);
	Send(msg, dijkstra_79);
	Send(msg, dijkstra_80);
	Send(msg, dijkstra_81);
	Send(msg, dijkstra_82);
	Send(msg, dijkstra_83);
	Send(msg, dijkstra_84);
	Send(msg, dijkstra_85);
	Send(msg, dijkstra_86);
	Send(msg, dijkstra_87);
	Send(msg, dijkstra_88);
	Send(msg, dijkstra_89);
	Send(msg, dijkstra_90);
	Send(msg, dijkstra_91);
	Send(msg, dijkstra_92);
	Send(msg, dijkstra_93);
	Send(msg, dijkstra_94);
	Send(msg, dijkstra_95);
	Send(msg, dijkstra_96);
	Send(msg, dijkstra_97);
	Send(msg, dijkstra_98);
	Send(msg, dijkstra_99);
	Send(msg, dijkstra_100);
	Send(msg, dijkstra_101);
	Send(msg, dijkstra_102);
	Send(msg, dijkstra_103);
	Send(msg, dijkstra_104);
	Send(msg, dijkstra_105);
	Send(msg, dijkstra_106);
	Send(msg, dijkstra_107);
	Send(msg, dijkstra_108);
	Send(msg, dijkstra_109);
	Send(msg, dijkstra_110);
	Send(msg, dijkstra_111);
	Send(msg, dijkstra_112);
	Send(msg, dijkstra_113);
	Send(msg, dijkstra_114);
	Send(msg, dijkstra_115);
	Send(msg, dijkstra_116);
	Send(msg, dijkstra_117);
	Send(msg, dijkstra_118);
	Send(msg, dijkstra_119);
	Send(msg, dijkstra_120);
	Send(msg, dijkstra_121);
	Send(msg, dijkstra_122);
	Send(msg, dijkstra_123);
	Send(msg, dijkstra_124);
	Send(msg, dijkstra_125);
	Send(msg, dijkstra_126);
	Send(msg, dijkstra_127);
	Send(msg, dijkstra_128);
	Send(msg, dijkstra_129);
	Send(msg, dijkstra_130);
	Send(msg, dijkstra_131);
	Send(msg, dijkstra_132);
	Send(msg, dijkstra_133);
	Send(msg, dijkstra_134);
	Send(msg, dijkstra_135);
	Send(msg, dijkstra_136);
	Send(msg, dijkstra_137);
	Send(msg, dijkstra_138);
	Send(msg, dijkstra_139);
	Send(msg, dijkstra_140);
	Send(msg, dijkstra_141);
	Send(msg, dijkstra_142);

	/* SEND AdjMatrix[NUM_NODES][NUM_NODES] */
	msg.length = NUM_NODES;
	for (i=0; i<NUM_NODES; i++) {
		//MADA ECHO(" SEND: ");
		for (j=0; j<NUM_NODES; j++) {
			msg.msg[j] = AdjMatrix[j][i];
			//MADA ECHO(itoa(msg.msg[j]));
			//MADA ECHO(" ");
		}
		Send(msg, dijkstra_0);
		Send(msg, dijkstra_1);
		Send(msg, dijkstra_2);
		Send(msg, dijkstra_3);
		Send(msg, dijkstra_4);
		Send(msg, dijkstra_5);
		Send(msg, dijkstra_6);
		Send(msg, dijkstra_7);
		Send(msg, dijkstra_8);
		Send(msg, dijkstra_9);
		Send(msg, dijkstra_10);
		Send(msg, dijkstra_11);
		Send(msg, dijkstra_12);
		Send(msg, dijkstra_13);
		Send(msg, dijkstra_14);
		Send(msg, dijkstra_15);
		Send(msg, dijkstra_16);
		Send(msg, dijkstra_17);
		Send(msg, dijkstra_18);
		Send(msg, dijkstra_19);
		Send(msg, dijkstra_20);
		Send(msg, dijkstra_21);
		Send(msg, dijkstra_22);
		Send(msg, dijkstra_23);
		Send(msg, dijkstra_24);
		Send(msg, dijkstra_25);
		Send(msg, dijkstra_26);
		Send(msg, dijkstra_27);
		Send(msg, dijkstra_28);
		Send(msg, dijkstra_29);
		Send(msg, dijkstra_30);
		Send(msg, dijkstra_31);
		Send(msg, dijkstra_32);
		Send(msg, dijkstra_33);
		Send(msg, dijkstra_34);
		Send(msg, dijkstra_35);
		Send(msg, dijkstra_36);
		Send(msg, dijkstra_37);
		Send(msg, dijkstra_38);
		Send(msg, dijkstra_39);
		Send(msg, dijkstra_40);
		Send(msg, dijkstra_41);
		Send(msg, dijkstra_42);
		Send(msg, dijkstra_43);
		Send(msg, dijkstra_44);
		Send(msg, dijkstra_45);
		Send(msg, dijkstra_46);
		Send(msg, dijkstra_47);
		Send(msg, dijkstra_48);
		Send(msg, dijkstra_49);
		Send(msg, dijkstra_50);
		Send(msg, dijkstra_51);
		Send(msg, dijkstra_52);
		Send(msg, dijkstra_53);
		Send(msg, dijkstra_54);
		Send(msg, dijkstra_55);
		Send(msg, dijkstra_56);
		Send(msg, dijkstra_57);
		Send(msg, dijkstra_58);
		Send(msg, dijkstra_59);
		Send(msg, dijkstra_60);
		Send(msg, dijkstra_61);
		Send(msg, dijkstra_62);
		Send(msg, dijkstra_63);
		Send(msg, dijkstra_64);
		Send(msg, dijkstra_65);
		Send(msg, dijkstra_66);
		Send(msg, dijkstra_67);
		Send(msg, dijkstra_68);
		Send(msg, dijkstra_69);
		Send(msg, dijkstra_70);
		Send(msg, dijkstra_71);
		Send(msg, dijkstra_72);
		Send(msg, dijkstra_73);
		Send(msg, dijkstra_74);
		Send(msg, dijkstra_75);
		Send(msg, dijkstra_76);
		Send(msg, dijkstra_77);
		Send(msg, dijkstra_78);
		Send(msg, dijkstra_79);
		Send(msg, dijkstra_80);
		Send(msg, dijkstra_81);
		Send(msg, dijkstra_82);
		Send(msg, dijkstra_83);
		Send(msg, dijkstra_84);
		Send(msg, dijkstra_85);
		Send(msg, dijkstra_86);
		Send(msg, dijkstra_87);
		Send(msg, dijkstra_88);
		Send(msg, dijkstra_89);
		Send(msg, dijkstra_90);
		Send(msg, dijkstra_91);
		Send(msg, dijkstra_92);
		Send(msg, dijkstra_93);
		Send(msg, dijkstra_94);
		Send(msg, dijkstra_95);
		Send(msg, dijkstra_96);
		Send(msg, dijkstra_97);
		Send(msg, dijkstra_98);
		Send(msg, dijkstra_99);
		Send(msg, dijkstra_100);
		Send(msg, dijkstra_101);
		Send(msg, dijkstra_102);
		Send(msg, dijkstra_103);
		Send(msg, dijkstra_104);
		Send(msg, dijkstra_105);
		Send(msg, dijkstra_106);
		Send(msg, dijkstra_107);
		Send(msg, dijkstra_108);
		Send(msg, dijkstra_109);
		Send(msg, dijkstra_110);
		Send(msg, dijkstra_111);
		Send(msg, dijkstra_112);
		Send(msg, dijkstra_113);
		Send(msg, dijkstra_114);
		Send(msg, dijkstra_115);
		Send(msg, dijkstra_116);
		Send(msg, dijkstra_117);
		Send(msg, dijkstra_118);
		Send(msg, dijkstra_119);
		Send(msg, dijkstra_120);
		Send(msg, dijkstra_121);
		Send(msg, dijkstra_122);
		Send(msg, dijkstra_123);
		Send(msg, dijkstra_124);
		Send(msg, dijkstra_125);
		Send(msg, dijkstra_126);
		Send(msg, dijkstra_127);
		Send(msg, dijkstra_128);
		Send(msg, dijkstra_129);
		Send(msg, dijkstra_130);
		Send(msg, dijkstra_131);
		Send(msg, dijkstra_132);
		Send(msg, dijkstra_133);
		Send(msg, dijkstra_134);
		Send(msg, dijkstra_135);
		Send(msg, dijkstra_136);
		Send(msg, dijkstra_137);
		Send(msg, dijkstra_138);
		Send(msg, dijkstra_139);
		Send(msg, dijkstra_140);
		Send(msg, dijkstra_141);
		Send(msg, dijkstra_142);
		//MADA ECHO("\n");
	}
}

void divide_task_group(int task) {
	int i;
	for(i=0;i<PROCESSORS;i++){
		tasks[i][0] = task/PROCESSORS* (i);
		tasks[i][1] = task/PROCESSORS* (i+1) + (i+1==PROCESSORS&task%PROCESSORS!=0?task%PROCESSORS:0);
	}
}

void ProcessMessage() {
	int paths = 0;

	if (result[0] == -1) {
		ended++;
		printf("\nENDED %d", ended);
		//MADA ECHO(itoa(GetTick()));
		//MADA ECHO("Divider finished.");
	}
	else {
	printf("From %d to %d, shortest path is %d in cost. Path is: %d", result[0]+1, result[1]+1, result[2], result[0]+1);		//MADA ECHO("From ");
		//MADA ECHO(itoa(result[0]+1));
		//MADA ECHO(" to ");
		//MADA ECHO(itoa(result[1]+1));
		//MADA ECHO(", shortest path is ");
		//MADA ECHO(itoa(result[2]));
		//MADA ECHO(" in cost. ");
		//MADA ECHO("Path is: ");
		//MADA ECHO(itoa(result[0]+1));
		for(paths=3; paths<33; paths++) {
			if (result[paths] > 0) {
				printf(" %d", result[paths]);
			}
		}
		printf("\n");
	}
}

int main0(int argc, char *argv[]) {
	int i=0,j=0,k=0;

	int fpTrix[NUM_NODES*NUM_NODES] = { 0,    6,    3,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										6,    0,    2,    5,    9999, 9999, 1,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										3,    2,    0,    3,    4,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										9999, 5,    3,    0,    2,    3,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										9999, 9999, 4,    2,    0,    5,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										9999, 9999, 9999, 3,    5,    0,    3,    2,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										9999, 1,    9999, 9999, 9999, 3,    0,    4,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
										9999, 9999, 9999, 9999, 9999, 2,    4,    0,    7,    9999, 9999, 9999, 9999, 9999, 9999, 9999,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 7,    0,    5,    1,    9999, 9999, 9999, 9999, 9999,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 5,    0,    9999, 3,    9999, 9999, 9999, 9999,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 1,    9999, 0,    9999, 4,    9999, 9999, 8,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 3,    9999, 0,    9999, 2,    9999, 9999,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 4,    9999, 0,    1,    9999, 2,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 2,    1,    0,    6,    9999,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 6,    0,    3,
										9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 8,    9999, 2,    9999, 3,    0 };

	/* Step 1: geting the working vertexs and assigning values */
	for (i=0;i<NUM_NODES;i++) {
		//MADA ECHO(" MATRIX: ");
		for (j=0;j<NUM_NODES;j++) {
			AdjMatrix[i][j]= fpTrix[k];
			//MADA ECHO(itoa(AdjMatrix[i][j]));
			//MADA ECHO(" ");
			k++;
		}
		//MADA ECHO("\n");
	}

	int tasks = NUM_NODES*(NUM_NODES-1)/2;

	int x=0;
	for(i=0;i<NUM_NODES-1;i++){ //small:15; large:159
		for(j=i+1;j<NUM_NODES;j++){	//small:16; large:160
			nodes_tasks[x][0] = i;
			nodes_tasks[x][1] = j;
			x++;
		}
	}

	divide_task_group(tasks);
	startThreads();

	//MADA ECHO("\n");
	Message msg;
	msg.length = 33;
	while (1) {
		msg = Receive(); //MADAmsg, dijkstra_0);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_1);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_2);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_3);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_4);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_5);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_6);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_7);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_8);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_9);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_10);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_11);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_12);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_13);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_14);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_15);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_16);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_17);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_18);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_19);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_20);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_21);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_22);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_23);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_24);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_25);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_26);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_27);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_28);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_29);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_30);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_31);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_32);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_33);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_34);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_35);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_36);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_37);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_38);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_39);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_40);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_41);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_42);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_43);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_44);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_45);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_46);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_47);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_48);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_49);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_50);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_51);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_52);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_53);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_54);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_55);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_56);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_57);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_58);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_59);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_60);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_61);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_62);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_63);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_64);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_65);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_66);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_67);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_68);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_69);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_70);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_71);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_72);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_73);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_74);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_75);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_76);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_77);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_78);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_79);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_80);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_81);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_82);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_83);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_84);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_85);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_86);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_87);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_88);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_89);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_90);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_91);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_92);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_93);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_94);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_95);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_96);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_97);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_98);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_99);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_100);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_101);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_102);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_103);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_104);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_105);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_106);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_107);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_108);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_109);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_110);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_111);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_112);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_113);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_114);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_115);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_116);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_117);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_118);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_119);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_120);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_121);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_122);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_123);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_124);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_125);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_126);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_127);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_128);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_129);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_130);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_131);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_132);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_133);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_134);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_135);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_136);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_137);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_138);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_139);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_140);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_141);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();
		msg = Receive(); //MADAmsg, dijkstra_142);
		for (k=0; k<33; k++)
			result[k] = msg.msg[k];
		ProcessMessage();

		if (ended == (NPROC)) {
			return 0;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
MemoryWrite(INITIALIZE_ROUTER, 0);	int m_argc,size,i;
	char *m_argv[3];

	pthread_n_workers = NPROC;

	PROCESSORS = pthread_n_workers;

	m_argc = 2;
	m_argv[1] = "input_small.dat";

	main0(m_argc,m_argv);
MemoryWrite(END_SIM, 0);
	return 0;
}
