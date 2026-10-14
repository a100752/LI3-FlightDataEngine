# Flight & Airport Data Engine (C / GLib)

Motor de alto desempenho para processamento, indexação, validação e consulta analítica de grandes volumes de dados aeronáuticos (voos, passageiros, reservas, aeroportos e aeronaves), desenvolvido em **Linguagem C** com recurso à biblioteca **GLib**.

Projeto desenvolvido no âmbito da unidade curricular de Laboratórios de Informática III (LEI) da **Universidade do Minho**.

---

## 🚀 Destaques Técnicos

- **Processamento em Larga Escala**: Leitura e parsing eficiente de ficheiros CSV com centenas de milhares de registos sem fugas de memória (zero memory leaks auditado com Valgrind).
- **Indexação em Memória Otimizada**: Utilização de tabelas de dispersão (`GHashTable`), árvores binárias balanceadas e listas dinâmicas para pesquisas com complexidade temporal média $O(1)$ e $O(\log N)$.
- **Validação Estrita de Dados**: Módulo dedicado à validação de integridade referencial, datas, formatos de códigos IATA/ICAO, lotações de aeronaves e estados de reserva.
- **Motor de Consultas Analíticas (Queries 1 a 6)**:
  - **Query 1**: Resumo de entidade (utilizador, voo, aeroporto).
  - **Query 2**: Listagem ordenada de voos de passageiro ou aeronave.
  - **Query 3**: Classificação de aeroportos por volume e métricas de operação.
  - **Query 4**: Reservas e histórico associado a hotéis e aeroportos.
  - **Query 5**: Estatísticas de atrasos e estimativas temporais de voo.
  - **Query 6**: Aeroportos com maior tráfego num determinado período temporal.
- **Módulo de Testes e Benchmarking**: Binário específico (`programa-testes`) que mede tempos de execução por query e valida saídas contra oráculos de dados esperados.

---

## 📁 Estrutura do Repositório

```text
├── include/           # Ficheiros de cabeçalho (.h) modularizados
│   ├── logica/        # Entidades, comandos, gestor de queries e validações
│   ├── parsers/       # Parsers de CSV e sanitização de dados
│   ├── repositorios/  # Tabelas de índice e estruturas de armazenamento
│   └── utils.h        # Utilitários de manipulação de strings e datas
├── src/               # Implementação (.c)
│   ├── apresentacao/  # Pontos de entrada: programa-principal e programa-testes
│   ├── logica/        # Algoritmos das queries e regras de domínio
│   ├── parsers/       # Parsing com buffers otimizados
│   └── repositorios/  # Gestão das coleções e ciclo de vida dos registos
├── relatorio-fase2/   # Fontes do relatório técnico em LaTeX
├── Relatorio_Fase1.pdf# Relatório da Fase 1 (especificação e arquitetura)
└── Makefile           # Script de compilação com flags de otimização (-Ofast, -flto)
```

---

## 🛠️ Compilação e Execução

### Pré-requisitos
- Compilador GCC compatível com C11.
- Biblioteca `glib-2.0` instalada (`sudo apt install libglib2.0-dev` em Debian/Ubuntu, ou `brew install glib` em macOS).

### Compilar
Para compilar ambos os binários (`programa-principal` e `programa-testes`):

```bash
make
```

### Executar o Programa Principal
O executável recebe a diretoria dos dados CSV e o ficheiro de comandos/queries:

```bash
./programa-principal <caminho_para_csvs> <ficheiro_de_comandos.txt>
```

As respostas são geradas na diretoria `Resultados/` no formato `command<N>_output.txt`.

### Executar o Programa de Testes (Benchmarking)
Para avaliar os tempos de resposta e verificar a correção:

```bash
./programa-testes <caminho_para_csvs> <ficheiro_de_comandos.txt> <ficheiro_de_resultados_esperados/>
```

---

## 📄 Documentação

- [Relatório Fase 1 (PDF)](Relatorio_Fase1.pdf)
- Código fonte e estrutura do relatório da Fase 2 disponível na pasta [`relatorio-fase2/`](relatorio-fase2/).
