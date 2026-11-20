
// teste_str.c
// programa com testes do TAD str

#include "str.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  char *a = "Rábica";
  Str s = s_cria(a);
  printf("Deve escrever [%s] ", a);
  s_imprime(s);
  Str ss = s_cria_substring(s, 3, 2);
  printf("\nDeve escrever [ic] ");
  s_imprime(ss);
  s_substring(ss, s, 1, 2);
  printf("\nDeve escrever [áb] ");
  s_imprime(ss);
  if (s_tam(ss) != 2) printf("\ntamanho de 'áb' (%d) deveria ser 2\n", s_tam(ss));
  s_substitui(ss, -2, 1, NULL);
  s_substitui(s, -7, 3, ss);
  printf("Deve escrever [áica] ");
  s_imprime(s);
  s_destroi(ss);
  s_destroi(s);
  Str t1 = s_cria("abácaxi");
  Str t2 = s_cria("te");
  s_substitui(t1, 5, 2, t2);
  printf ("\nDeve escrever [abácate] ");
  s_imprime(t1);
  printf("\n");
  s_destroi(t2);
  s_destroi(t1);
  Str t3 = s_cria("abácaxi");
  Str t4 = s_cria (".");
  s_substitui(t3, 7, 0, t4);
  printf("Deve escrever [abácaxi.] ");
  s_imprime(t3);
  printf("\n");
  s_destroi(t3);
  s_destroi(t4);
  Str t5 = s_cria("abácaxi");
  Str t6 = s_cria("123");
  s_substitui(t5, -4, 0, t6);
  printf("\nDeve escrever [abác123axi] ");
  s_imprime(t5);
  s_destroi(t5);
  s_destroi(t6);
  printf("\n");
  Str t7 = s_cria("abácaxi");
  s_substitui(t7, 2, 200, NULL);
  printf("\nDeve escrever [ab] ");
  s_imprime(t7);
  printf("\n");
  s_destroi(t7);
  Str t8 = s_cria("bárcó");
  Str t9 = s_cria("");
  s_substring(t9, t8, -3, 1);
  printf("\nDeve escrever [c] ");
  s_imprime(t9);
  printf("\n");
  s_destroi(t9);
  s_destroi(t8);
  Str t10 = s_cria("Rábica");
  unichar c1 = s_ch(t10, 1);
  unichar c2 = s_ch(t10, -2);
  if (c1 != 0x00E1) {
    printf("\ns_ch(t10, 1) Deveria retornar á\n");
  }
  if (c2 != 0x0061) {
    printf("\ns_ch(t10, -2) Deveria retornar a\n");
  }
  s_destroi(t10);
  Str t11 = s_cria("banana");
  Str t12 = s_cria("ana");
  int posicao = s_busca_s(t11, 0, t12);
  if (posicao != 1) {
    printf("\ns_busca_s Deveria retornar 1, mas retornou %d\n", posicao);
  }
  s_destroi(t12);
  s_destroi(t11);
  Str t13 = s_cria("   teste   ");
  Str t14 = s_cria(" ");
  s_apara(t13, t14);
  printf("\nDeve escrever [teste] ");
  s_imprime(t13);
  printf("\n");
  s_destroi(t14);
  s_destroi(t13);
  Str t15 = s_cria("Olá UTF8");
  s_grava_arquivo(t15, "teste_str.txt");
  Str t16 = s_cria_de_arquivo("teste_str.txt");
  if (!s_igual(t15, t16)) {
    printf("\nErro: string lida do arquivo diferente da gravada\n");
  }
  s_destroi(t16);
  s_destroi(t15);
  remove("teste_str.txt");
  Str t17 = s_cria("Olá");
  char *cstr = s_strc(t17);
  printf("\nDeve escrever [Olá] %s\n", cstr);
  free(cstr);
  s_destroi(t17);
  Str t18 = s_cria("Rica");
  s_insere_c(t18, 1, 0x00E1);
  printf("\nDeve escrever [Ráica] ");
  s_imprime(t18);
  printf("\n");
  s_destroi(t18);
  Str t19 = s_cria("banana");
  Str t20 = s_cria("an");
  int b1 = s_busca_c(t19, 0, t20);
  if (b1 != 1) {
    printf("\ns_busca_c deveria retornar 1, mas retornou %d\n", b1);
  }
  int b2 = s_busca_nc(t19, 0, t20);
  if (b2 != 0) {
    printf("\ns_busca_nc deveria retornar 0, mas retornou %d\n", b2);
  }
  int b3 = s_busca_rc(t19, -1, t20);
  if (b3 != 5) {
    printf("\ns_busca_rc deveria retornar 5, mas retornou %d\n", b3);
  }
  int b4 = s_busca_rnc(t19, -1, t20);
  if (b4 != 0) {
    printf("\ns_busca_rnc deveria retornar 0, mas retornou %d\n", b4);
  }
  s_destroi(t20);
  s_destroi(t19);
  Str t21 = s_cria("original");
  Str t22 = s_cria_cópia(t21);
  s_insere_c(t21, -1, '!');
  printf("\nDeve escrever [original] ");
  s_imprime(t22);
  printf("\n");
  s_destroi(t22);
  s_destroi(t21);
}Z