/** 
 * Laboratório de Sistemas Operacionais - 2019/2
 * Integrantes: Gabriel Porto, Guilherme e Rafael Henrique da Silva
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define TAM_STATUS 50
#define DUR_ALARM 10

// Variáveis globais
int opcao = 0;
pid_t 
	pidBrowser 	= 0,
	pidEditor 	= 0,
	pidTerminal = 0;
char 
	url[100],
	statusBrowser[TAM_STATUS] 		= "Não Iniciado",
	statusEditor[TAM_STATUS] 			= "Não Iniciado",
	statusTerminal[TAM_STATUS] 		= "Não Iniciado",
	statusFinalizador[TAM_STATUS]	=" ";
struct sigaction sa;

// Assinaturas das funções
void tratador(int signum);
int gerarMenu (void);
void abrirBrowser();
void abrirEditor();
void abrirTerminal();
void finalizarAplicacao();



int main(void) {
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &tratador;
	if(sigaction(SIGALRM, &sa, NULL) != 0) {
		perror("Falha ao instalar o tratador de sinal");
		exit(-1);
	}
	if(sigaction(SIGINT, &sa, NULL) != 0) {
		perror("Falha ao instalar o tratador de sinal");
		exit(-1);
	}
	if(sigaction(SIGCHLD, &sa, NULL) != 0) {
		perror("Falha ao instalar o tratador de sinal");
		exit(-1);
	}
	while (opcao!=5){  	
		alarm(DUR_ALARM);
		opcao = gerarMenu();
	} 
	return 0;
}
//parte gráfica do código
int gerarMenu (void) {
	printf("\n\t<<<<    Menu    >>>>\n");
	printf("\n1) Web Browser\t\t%s",			statusBrowser);
	printf("\n2) Editor\t\t%s",						statusEditor);
	printf("\n3) Terminal\t\t%s",					statusTerminal);
	printf("\n4) Finalizar Processo\t%s",	statusFinalizador);
	printf("\n5) Quit");
	opcao	=	0;
	printf("\n\nSelecione uma opção: ");
	scanf("%d",&opcao);

	switch(opcao){
		case 1: abrirBrowser(); 							break;
		case 2:	abrirEditor();								break;
		case 3:	abrirTerminal();							break;
		case 4: finalizarAplicacao(); 				break;
		case 5: exit(0); 											break;
		default: printf("\nOpção Inválida");	break;
	}
	return opcao;       
}
// Criando processos filhos e substituindo eles pelas aplicações selecionadas
void abrirBrowser(){
	alarm(0);
	printf("\nDigite a url do site desejado: \n");
	scanf("%s",url);

	pid_t pidb = fork();
	if (pidb < 0){
		perror("Erro no fork!\n");
		sprintf(statusBrowser, "Falhou, PID=%d", pidTerminal);
	}
	if(pidb == 0){
		execlp("/usr/bin/firefox", "firefox", url, NULL);
		sprintf(statusBrowser, "Falhou, PID=%d", pidTerminal);
	}
	else{
		pidBrowser = pidb;
		sprintf(statusBrowser, "Executando, PID=%d", pidBrowser);  
	}
}
void abrirEditor(){
	pid_t pide = fork();
	if (pide<0){
		perror("Erro no fork!\n");
		sprintf(statusEditor, "Falhou, PID=%d", pidTerminal);
	}
	if(pide == 0){
		execlp("/usr/bin/gedit", "gedit", NULL);
		sprintf(statusEditor, "Falhou, PID=%d", pidTerminal);
	}
	else{
		
		pidEditor = pide;
		sprintf(statusEditor, "Executando, PID=%d", pidEditor);  
	}
}
void abrirTerminal(){
	pid_t pidt = fork();
	if (pidt<0){
		perror("Erro no fork!\n");
		sprintf(statusTerminal, "Falhou, PID=%d", pidTerminal);
	}
	if (pidt == 0){
		execlp("/usr/bin/xfce4-terminal", "xfce4-terminal", NULL);
		sprintf(statusTerminal, "Falhou, PID=%d", pidTerminal);
	}
	else{
		
		pidTerminal = pidt;
		sprintf(statusTerminal, "Executando, PID=%d", pidTerminal);
	}
}

// Finaliza caso a aplicação esteja em execução, e aborta o finalizar caso nao esteja sendo executada
void finalizarAplicacao(){
	alarm(0);
	printf("\n\nSelecione a aplicação a ser finalizada");
	printf("\n[1] - Browser");
	printf("\n[2] - Editor");
	printf("\n[3] - Terminal\n");
	int processo;
	scanf("%d",&processo);
	switch(processo){
		case 1:
		if (pidBrowser!=0){
			kill(pidBrowser, SIGTERM);
			sprintf(statusBrowser, "Concluido");
			sprintf(statusFinalizador, "Concluido");	
			pidBrowser=0;
		}else{
			sprintf(statusFinalizador,"Abortado");		
		}
		break;
		case 2:
		if (pidEditor!=0){
			kill(pidEditor, SIGTERM);
			sprintf(statusEditor, "Concluido");
			sprintf(statusFinalizador, "Concluido");
			pidEditor=0;
		}else{
			sprintf(statusFinalizador,"Abortado");		
		}
		break;
		case 3:
		if (pidTerminal!=0){
			kill(pidTerminal, SIGTERM);
			sprintf(statusTerminal, "Concluido");
			sprintf(statusFinalizador, "Concluido");
			pidTerminal=0;
		}else{
			sprintf(statusFinalizador,"Abortado");		
		}
		break;
		
	}
}
// tratando os sinais conforme as exigencias de cada sinal
void tratador(int signum){
	pid_t p = 0;
	switch(signum){
		case SIGINT:
			//gerarMenu();
		break;
		case SIGCHLD:
			p  = wait(NULL);
			if (p==pidBrowser)
				sprintf(statusBrowser, "Abortado");
		else if(p== pidEditor){
				sprintf(statusEditor, "Abortado");
		}else if(p== pidTerminal){
				sprintf(statusTerminal, "Abortado");
		}
	}
}
