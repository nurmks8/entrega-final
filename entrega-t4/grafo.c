#include "grafo.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct aresta {
  int origem;
  int destino;
  void *dado;
};

struct grafo {
  int tam_no;
  int tam_aresta;

  int tipo_consulta;
  int no_consulta;
  int proxima_aresta;

  void **nos;
  int n_nos;

  struct aresta *arestas;
  int n_arestas;
};

static void g_invalida_consulta(Grafo self)
{
  if (self != NULL) {
      self->tipo_consulta = 0;
      self->no_consulta = -1;
      self->proxima_aresta = 0;
  }
}

// cria um grafo vazio que suporta dados do tamanho fornecido (em bytes)
//   nos nós e nas arestas
Grafo g_cria(int tam_nó, int tam_aresta)
{
    if (tam_nó <= 0 || tam_aresta <= 0) {
        return NULL;
    }
    Grafo self = malloc(sizeof(struct grafo));
    
    if (self == NULL) {
        return  NULL;
    }
    self->tam_no = tam_nó;
    self->tam_aresta = tam_aresta;
    self->n_nos = 0;
    self->nos = NULL;
    self->n_arestas = 0;
    self->arestas = NULL;
    self->tipo_consulta = 0;
    self->no_consulta = -1;
    self->proxima_aresta = 0;
    return self;
}

// libera a memória ocupada por um grafo
void g_destrói(Grafo self)
{
  if (self == NULL) {
        return;
  }
  int i;
  for (i = 0; i < self->n_nos; i++) {
    free(self->nos[i]);
  }
  free(self->nos);
  
  for (i = 0; i < self->n_arestas; i++) {
    free(self->arestas[i].dado);
  }
  free(self->arestas);
  free(self);
}

int g_insere_nó(Grafo self, void *p_dado)
{
  if (self == NULL || p_dado == NULL || 
    self->n_nos == INT_MAX) {
    return -1;
  }

  void *novo_dado = malloc(self->tam_no);

  if ( novo_dado == NULL) {
    return -1;
  }
  memcpy(novo_dado, p_dado, self->tam_no);

  void **novo_vetor = realloc(
       self->nos, (size_t)(self->n_nos + 1) * sizeof( void *));

  if (novo_vetor == NULL) {
    free(novo_dado);
    return -1;
  }

  self->nos = novo_vetor;
  int id = self->n_nos;

  self->nos[id] = novo_dado;
  self->n_nos++;

  return id;
  
}

void g_remove_nó(Grafo self, int nó)
{
  if (self == NULL || nó < 0 || nó >= self->n_nos) {
    return;
  }

  g_invalida_consulta(self);

  int i, j;
  for (i = 0; i < self->n_arestas; i++) {
     if (self->arestas [i].origem == nó ||
        self->arestas[i].destino == nó) {
        free(self->arestas[i].dado);

      for (j = i + 1; j < self->n_arestas; j++) {
        self->arestas[j - 1] = self->arestas[j];
      }
    self->n_arestas--;
    i--;
     }
   }
   for (i = 0; i < self->n_arestas; i++) {
      if (self->arestas [i].origem > nó) {
        self->arestas[i].origem--;
      }
      if (self->arestas[i].destino > nó) {
        self->arestas[i].destino--;
      }
   }
   free(self->nos[nó]);
   for (j = nó + 1; j < self->n_nos; j++) {
     self->nos[j - 1] = self->nos[j];
   }
    self->n_nos--;
}

void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado)
{
  if (self == NULL || nó < 0 || nó >= self->n_nos || p_dado == NULL) {
    return;
  }
  memcpy(self->nos[nó], p_dado, self->tam_no);
}

void *g_valor_do_nó(Grafo self, int nó)
{
  if (self == NULL || nó < 0 || nó >= self->n_nos) {
    return NULL;
  }
    return self->nos[nó];
  
}

int g_num_nós(Grafo self) 
{
  if (self == NULL) {
    return -1;
  }
  return self->n_nos;
}

void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino,
                              void *p_dado)
{
  if (self == NULL || nó_origem < 0 || nó_origem >= self->n_nos ||
      nó_destino < 0 || nó_destino >= self->n_nos) {
    return;
  }
  int i, j;
   for (i = 0; i < self->n_arestas; i++) {
     if (self->arestas[i].origem == nó_origem &&
        self->arestas[i].destino == nó_destino) {
        if (p_dado != NULL) {
            memcpy(self->arestas[i].dado, p_dado, self->tam_aresta);
            return;
        }
    free(self->arestas[i].dado);
    for (j = i + 1; j < self->n_arestas; j++) {
        self->arestas[j - 1] = self->arestas[j];
    }
    self->n_arestas--;
    g_invalida_consulta(self);
    return;
    }
   }
   if (p_dado == NULL || self->n_arestas == INT_MAX) {
    return;
   }
   void *novo_dado = malloc(self->tam_aresta);
   
   if (novo_dado == NULL) {
     g_invalida_consulta(self);
     return;
   }
   memcpy(novo_dado, p_dado, self->tam_aresta);

   struct aresta *novas = realloc(
    self->arestas,
    (size_t)(self->n_arestas + 1) * sizeof(struct aresta)
   );

    if (novas == NULL) {
      free(novo_dado);
      g_invalida_consulta(self);
      return;
    }
    self->arestas = novas;
    
    self->arestas[self->n_arestas].origem = nó_origem;
    self->arestas[self->n_arestas].destino = nó_destino;
    self->arestas[self->n_arestas].dado = novo_dado;

    self->n_arestas++;

    g_invalida_consulta(self);
}

void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino)
{
  if (self == NULL || nó_origem < 0 || nó_origem >= self->n_nos ||
      nó_destino < 0 || nó_destino >= self->n_nos) {
    return NULL;
  }
  int i;
  for (i = 0; i < self->n_arestas; i++) {
    if (self->arestas[i].origem == nó_origem &&
        self->arestas[i].destino == nó_destino) {
      return self->arestas[i].dado;
    }
  }
  return NULL;
}

void g_arestas_que_partem(Grafo self, int nó_origem)
{
  if (self == NULL) {
    return;
  }
  if (nó_origem < 0 || nó_origem >= self->n_nos) {
    g_invalida_consulta(self);
    return;
  }
  self->tipo_consulta = 1;
  self->no_consulta = nó_origem;
  self->proxima_aresta = 0; 
}
void g_arestas_que_chegam(Grafo self, int nó_destino)
{
  if (self == NULL) {
    return;
  }
  if (nó_destino < 0 || nó_destino >= self->n_nos) {
    g_invalida_consulta(self);
    return;
  }
  self->tipo_consulta = 2;
  self->no_consulta = nó_destino;
  self->proxima_aresta = 0;
}

void *g_próxima_aresta(Grafo self, int *p_nó_vizinho)
{
  if (self == NULL || self->tipo_consulta == 0) {
    return NULL;
  }
  int i;
  for (i = self->proxima_aresta; i < self->n_arestas; i++) {
    if (self->tipo_consulta == 1 &&
        self->arestas[i].origem == self->no_consulta) {
      if (p_nó_vizinho != NULL) {
        *p_nó_vizinho = self->arestas[i].destino;
      }
      self->proxima_aresta = i + 1;
      return self->arestas[i].dado;
    }
    if (self->tipo_consulta == 2 &&
        self->arestas[i].destino == self->no_consulta) {
      if (p_nó_vizinho != NULL) {
        *p_nó_vizinho = self->arestas[i].origem;
      }
      self->proxima_aresta = i + 1;
      return self->arestas[i].dado;
    }
  }
  g_invalida_consulta(self);
  return NULL;
}

bool g_tem_ciclo_recursivo(Grafo self, int atual, bool *visitado, bool *em_caminho)
{
  visitado[atual] = true;
  em_caminho[atual] = true;
  
  for ( int i = 0; i < self->n_arestas; i++) {
    if (self->arestas[i].origem == atual) {
      int vizinho = self->arestas[i].destino;
      if (!visitado[vizinho]) {
        if (g_tem_ciclo_recursivo(self, vizinho, visitado, em_caminho)) {
          return true;
        }
      } else if (em_caminho[vizinho]) {
        return true;
      }
    }
  }
  em_caminho[atual] = false;

  return false;
}

bool g_tem_ciclo(Grafo self)
{
  if (self == NULL || self->n_nos == 0) {
    return false;
  }

  bool *visitado = calloc(self->n_nos, sizeof(bool));
  bool *em_caminho = calloc(self->n_nos, sizeof(bool));

  if (visitado == NULL || em_caminho == NULL) {
    free(visitado);
    free(em_caminho);
    return false;
  }
  bool encontrou_ciclo = false;

  for (int i = 0; i < self->n_nos; i++) {
    if (!visitado[i]) {
      if (g_tem_ciclo_recursivo(self, i, visitado, em_caminho)) {
        encontrou_ciclo = true;
        break;
      }
    } 
  }
  free(visitado);
  free(em_caminho);

  return encontrou_ciclo;
}
Fila g_ordem_topológica(Grafo self)
{
  if (self == NULL) {
    return NULL;
  }
  int n = self->n_nos;

  Fila ordem = f_cria(sizeof(int));
  Fila disponiveis = f_cria(sizeof(int));

  if (ordem == NULL || disponiveis == NULL) {
    f_destrói(ordem);
    f_destrói(disponiveis);
    return NULL;
  }

  if (n == 0) {
    f_destrói(disponiveis);
    return ordem;
  }

  int *grau_entrada = calloc(n, sizeof(int));
  if (grau_entrada == NULL) {
    f_destrói(ordem);
    f_destrói(disponiveis);
    return NULL;
  }
  for (int i = 0; i < self->n_arestas; i++) {
    int destino = self->arestas[i].destino;
    grau_entrada[destino]++;
  }
  for (int i = 0; i < n; i++) {
    if (grau_entrada[i] == 0) {
      f_insere(disponiveis, &i);
    }
  }
  int processados = 0;  
  int atual;
  
  while (!f_tá_vazia(disponiveis)) {
    f_remove(disponiveis, &atual);
    f_insere(ordem, &atual);
    processados++;

    for (int i = 0; i < self->n_arestas; i++) {
      if (self->arestas[i].origem == atual) {
        int vizinho = self->arestas[i].destino;
        grau_entrada[vizinho]--;
        if (grau_entrada[vizinho] == 0) {
          f_insere(disponiveis, &vizinho);
        }
      }
    }
  }
    free(grau_entrada);
    f_destrói(disponiveis);

  if (processados != n) {
    while (!f_tá_vazia(ordem)) {
      f_remove(ordem, &atual);
    }
  }
  return ordem;
}