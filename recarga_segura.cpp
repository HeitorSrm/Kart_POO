#include "recarga_segura.hpp"
#include <sstream>      // FORNECIDO: monta um resumo de texto dos IDs
#include <stdexcept>    // std::runtime_error (falha)
#include <utility>      // std::move, quando necessario

// LAB11 1a-1c — preparar uma configuracao candidata, sem alterar a ativa.
std::unique_ptr<EstadoCometa> preparar_candidato(const std::string& caminho) {
    // 1a) le o arquivo. Se a carga nao esta pronta, a operacao nao pode
    // cumprir o contrato: junta os diagnosticos e lanca.
    const ResultadoConfiguracao carga = carregar_configuracao(caminho);
    if (!carga.pronta()) {
        std::string causa;
        for (const std::string& erro : carga.erros) {
            if (!causa.empty()) causa += "; ";
            causa += erro;
        }
        if (causa.empty()) causa = "causa desconhecida";
        throw std::runtime_error("carga invalida: " + causa);
    }
    // 1a) uma lista vazia le "com sucesso", mas esta operacao promete
    // uma NOVA configuracao com sensores, nao uma lista vazia.
    if (carga.especificacoes.empty()) {
        throw std::runtime_error("carga sem sensores: " + caminho);
    }

    // 1b) so agora existe um candidato, independente do estado ativo.
    auto candidato = std::make_unique<EstadoCometa>();

    // 1c) instala no CANDIDATO. Se falhar no meio, o primeiro sensor ja pode
    // ter sido criado; ao sair pelo throw, 'candidato' (dono local) e destruido
    // junto com tudo o que possui. Nenhum delete manual.
    std::string erro;
    if (!instalar_sensores(candidato->configuracao, carga.especificacoes,
                           candidato->catalogo, erro)) {
        throw std::runtime_error("recarga recusada: " + erro);
    }
    return candidato; // sucesso: entrega a posse ao chamador
}

// LAB11 2a-2b — a troca deve ocorrer apenas depois da preparacao completa.
void recarregar(std::unique_ptr<EstadoCometa>& ativo,
               const std::string& caminho) {
    // 2a) se preparar_candidato lancar, a execucao sai daqui e 'ativo'
    // continua exatamente como estava.
    std::unique_ptr<EstadoCometa> candidato = preparar_candidato(caminho);
    // 2b) unica linha que altera o ativo: troca os proprietarios, sem copiar.
    // Depois dela, 'candidato' guarda o estado ANTIGO e o destroi ao sair.
    ativo.swap(candidato);
}

// FORNECIDO — o teste compara valores observaveis do estado; nunca
// armazene observadores para usa-los depois que um EstadoCometa for destruido.
std::string resumo_ativo(const EstadoCometa& estado) {
    std::ostringstream texto;
    texto << "quantidade=" << estado.configuracao.quantidadeSensoresExtras()
          << " ids=";
    for (const EntradaCatalogo& entrada : estado.catalogo) {
        // identificador() consulta o objeto que permanece possuido pela configuracao.
        if (entrada.observador) texto << entrada.observador->identificador() << ' ';
    }
    return texto.str();
}