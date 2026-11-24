#ifndef ECOP13A_LAB10_CATALOGO_SENSORES_HPP
#define ECOP13A_LAB10_CATALOGO_SENSORES_HPP

#include "kart_configuravel.hpp"
#include <memory>
#include <string>
#include <vector>

// LAB10 1a - FORNECIDO. A descricao do arquivo ainda NAO e um objeto Sensor.
// A terceira coluna e um LIMITE DE CONFIGURACAO, nao uma leitura e
// nao a calibracao do exemplo original da Aula 12.
struct EspecificacaoSensor {
    std::string tipo;         // "temperature" ou "pressure"
    std::string id;           // Identidade: T-02, P-01...
    double limite{};          // C: limite superior; kPa: maximo seguro
};

// LAB10 1b - FORNECIDO. O leitor apenas valida o TEXTO; ele nao cria sensores e nao altera o Kart.
struct ResultadoConfiguracao {
    bool arquivo_aberto{false};
    bool falha_leitura{false};
    std::vector<EspecificacaoSensor> especificacoes;
    std::vector<std::string> erros;

    // Neste laboratorio, qualquer erro de formato bloqueia a instalacao.
    bool pronta() const {
        return arquivo_aberto && !falha_leitura && erros.empty();
    }
};

// LAB10 3c - FORNECIDO. Adaptacao ao Kart real: Sensor::identificador() existe, mas Sensor nao
// oferece type() nem unit(). Guardamos esses dois metadados AO LADO de um
// ponteiro observador. O ponteiro nao possui o objeto.
struct EntradaCatalogo {
    const Sensor* observador{nullptr};
    std::string tipo;
    std::string unidade;
};

// FORNECIDA no LAB10: abre, separa e valida as tres colunas.
// O arquivo tem formato "tipo,id,limite", sem cabecalho e sem aspas.
ResultadoConfiguracao carregar_configuracao(const std::string& caminho);

// LAB10 2a-2d - A fabrica recebe DADOS e escolhe que objeto concreto construir.
// Uma especificacao pode ser textual e numericamente valida, mas violar
// a regra de construcao: nesse caso, retorna nullptr e explica por que.
std::unique_ptr<Sensor> criar_sensor(const EspecificacaoSensor& spec,
                                    std::string& erro);

// LAB10 3a-3c - Reutiliza o dono do LAB08. Apenas apos instalacao bem-sucedida
// acrescenta uma entrada nao proprietaria ao catalogo.
// Contrato: configuracao e catalogo chegam vazios. Em caso de erro, main
// encerra a configuracao local, e seus proprietarios liberam os sensores.
bool instalar_sensores(KartConfiguravel& configuracao,
                      const std::vector<EspecificacaoSensor>& specs,
                      std::vector<EntradaCatalogo>& catalogo,
                      std::string& erro);

// LAB10 4a-4d - Ordena apenas acessos (sem mover os donos) e escreve id,type,unit.
// Os observadores devem continuar vivos ate o fim desta chamada.
bool escrever_catalogo(const std::string& caminho,
                      const std::vector<EntradaCatalogo>& catalogo);

#endif
