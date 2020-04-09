# T3: Scheduling com OpenMP

Talles Siqueira Ceolin - [ELC139 - Programação Paralela]

## Programa

- [Link para o programa](https://github.com/elc139/t3-TallesSC/blob/master/OpenMPDemoABC.cpp)

Compilar e rodar com:
    
    g++ -std=c++11 OpenMPDemoABC.app -fopenmp -o OpenMPDemoABC
    
    ./OpenMPDemoABC

O Programa mostra através de uma tabela diversos resultados de acordo com o Schedule estabelecido pela função `omp_set_schedule(schedule)` da biblioteca `omp.h`. Na tabela é possível ver também a diferença entre resultados ao usar ou não o Mutex, como também quando existe especificação do `chunk` (para este teste foi utilizado `chunk = 4`).


## Exemplo de Saída / Tabela de Resultados

    runtime chunkless   MUTEX ABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABC A=20 B=20 C=20 
    runtime chunkless noMUTEX B-CB--B-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-CB-- A=0 B=20 C=18 
    static  chunkless   MUTEX AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC A=20 B=20 C=20 
    static  chunk = 4   MUTEX AAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCC A=20 B=20 C=20 
    static  chunkless noMUTEX BBCABCABCABCABCABCABCABCABCABCABBCABACBACBACBCABCABCABCA-CA- A=19 B=20 C=19 
    static  chunk = 4 noMUTEX BBCABCABCA--CBACBACBAC-BCBACBACBAC-BCBACBACBAC-BCBACBACBAC-- A=15 B=19 C=19 
    dynamic chunkless   MUTEX BACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBAC A=20 B=20 C=20 
    dynamic chunk = 4   MUTEX BBBBAAAACCCCBBBBAAAACCCCBBBBAAAACCCCBBBBAAAACCCCBBBBAAAACCCC A=20 B=20 C=20 
    dynamic chunkless noMUTEX BCABCABCABCABC-BC-BC-BC-BC-BC--C--C-AC-AC-AC-AC-AC-AC-AC-AB- A=12 B=11 C=19 
    dynamic chunk = 4 noMUTEX ACABCABCABC-ACBACBACBACBACBACBCACBACBACBACBACBACBACBACBA-BA- A=20 B=18 C=19 
    guided  chunkless   MUTEX CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAACCCCCCBBBBAAACCBA A=13 B=19 C=28 
    guided  chunk = 4   MUTEX BBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAACCCCCCCCCBBBBBBAAAACCCCBBB A=18 B=29 C=13 
    guided  chunkless noMUTEX CCABCABCABCABCBACBACBACBA-BABABABABAB-BBBBBCCBACBA-BAB-BC-B- A=16 B=25 C=13 
    guided  chunk = 4 noMUTEX ACABCABCABCABCABCABCABCABC-BCBCBCBCBC-CCCCCACBACBACBAC--CCCC A=13 B=16 C=27 
    auto    chunkless   MUTEX AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC A=20 B=20 C=20 
    auto    chunkless noMUTEX ACABCABCABCABCABCABCABCABCABCABCCABCABC-ACABCBACBACBACBA-AB- A=20 B=18 C=19

### runtime
Para a análise do schedule `runtime`, foi necessário aplicar o teste antes de qualquer uso da função `omp_set_schedule(schedule)`, pois assim o Schedule usado será o previamente estabelecido pela variável interna do ambiente. 

### static
Neste schedule nota-se uma distribuição circular pelo vetor de acordo com o chunk estabelecido, portanto existe uma igualdade na quantidade de caracteres `A/B/C`.

### dynamic
As iterações são distribuídas de acordo com o `chunk`, se ele não é especificado ele é estabelecido como 1, por tanto nota-se uma sequência do tipo `BAC BAC BAC...`. Assim como o schedule `static` o número de cada caractere é distribuído igualmente.

### guided
Similar ao `dynamic`, onde o `chunk` reduz ao longo do vetor.

### auto
Neste modo o schedule é definido pelo próprio sistema e o compilador, neste exemplo da tabela aparenta ter sido definido como `static`.

### mutex
Todas ocasiões com ausência do MUTEX apresentam falhas que podem ser vistas pelos caracters `-` ao longo do array, portanto não se pode analisar fielmente os diferentes tipos de schedule quando não se usa o mutex.

## Referências

- [Conceitos OpenMP, UFSC](http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/Conceitos_OpenMP.pdf)
- [OpenMP: For & Scheduling, Jaka's Corner](http://jakascorner.com/blog/2016/06/omp-for-scheduling.html)
- [OpenMP Loop scheduling, Intel](https://software.intel.com/en-us/articles/openmp-loop-scheduling)