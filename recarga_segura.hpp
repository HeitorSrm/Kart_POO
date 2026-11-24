#ifndef ECOP13A_LAB11_RECARGA_SEGURA_HPP
#define ECOP13A_LAB11_RECARGA_SEGURA_HPP

#include "catalogo_sensores.hpp" // LAB10: EspecificacaoSensor, EntradaCatalogo e instalacao
#include <memory>              // std::unique_ptr
#include <string>              // caminho e mensagens
#include <vector>              // entradas nao proprietarias do catalogo

// FORNECIDO: o estado do Cometa reune, na mesma vida util, os sensores
// proprietarios (em configuracao) e os acessos emprestados (em catalogo).
// A ordem dos membros importa: catalogo morre antes de configuracao.
struct EstadoCometa {
    KartConfiguravel configuracao{"Cometa Solar", "Mario", "CH-011", "azul"};
    std::vector<EntradaCatalogo> catalogo;
};

// LAB11 1a-1c — ALUNO. Prepare um NOVO estado sem tocar no ativo.
// Se a entrada ou a instalacao falhar, lance std::runtime_error com a causa.
// Se tudo funcionar, devolva o proprietario da configuracao candidata.
std::unique_ptr<EstadoCometa> preparar_candidato(const std::string& caminho);

// LAB11 2a-2b — ALUNO. Recarregar substitui o ativo SOMENTE APOS preparar.
// Se preparar_candidato lancar, esta funcao nao deve alterar 'ativo'.
void recarregar(std::unique_ptr<EstadoCometa>& ativo,
               const std::string& caminho);

// FORNECIDO para o experimento 3: exibe apenas quantidade e IDs instalados.
// Nao e uma prova de todos os atributos internos do Kart.
std::string resumo_ativo(const EstadoCometa& estado);

#endif
