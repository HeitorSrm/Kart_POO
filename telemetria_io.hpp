#pragma once
// LAB09 | ECOP13A - arquivo acrescentado ao Kart dos laboratorios anteriores.
// Aqui ficam os TIPOS e os CONTRATOS. Os corpos das funcoes ficam no .cpp.
#include "kart.hpp"
#include <string>
#include <vector>

// LAB09 1a - Represente UMA leitura aprovada, como TEMP_MOTOR,108.
// As duas entradas desta estrutura sao: canal (texto) e valor (numero).
struct LeituraValidada {
    // LAB09 1a | Entrada 1: nome do canal, por exemplo "TEMP_MOTOR".
    std::string canal;

    // LAB09 1a | Entrada 2: medicao numerica, por exemplo 108.
    double valor{};
};

// FORNECIDO - interpretar_linha() devolve a decisao de UMA linha.
// Em caso de erro, nao use leitura: consulte ok e erro.
struct ResultadoInterpretacao {
    bool ok{false};
    LeituraValidada leitura{};
    std::string erro{};
};

// LAB09 1b - Represente a carga de TODO o arquivo.
// Preencha as QUATRO entradas: dois indicadores e dois vetores.
struct ResultadoCarga {
    // LAB09 1b | Indicadores: ambos comecam em false.
    bool arquivo_aberto{false};
    bool falha_leitura{false};

    // LAB09 1b | Colecoes: dados aceitos (aceitas) e explicacoes das recusas (erros).
    std::vector<LeituraValidada> aceitas;
    std::vector<std::string> erros;
};

// ALUNO | LAB09 2a-2c: valida o campo numérico completo.
bool numero_finito(const std::string& texto, double& valor);
// FORNECIDO: chama numero_finito; nao reimplementar o parser.
ResultadoInterpretacao interpretar_linha(const std::string& linha);
// ALUNO | LAB09 3a-3d: coordena o arquivo e coleciona decisoes.
ResultadoCarga carregar_arquivo(const std::string& caminho);
// ALUNO | LAB09 4a-4c: atualiza o MESMO Kart conforme o canal.
void aplicar_no_kart(Kart& kart, const LeituraValidada& leitura);
// FORNECIDO: consulta Kart e ResultadoCarga, escreve e confere a saida.
bool escrever_boletim(const std::string& caminho,
                      const Kart& kart, const ResultadoCarga& carga);