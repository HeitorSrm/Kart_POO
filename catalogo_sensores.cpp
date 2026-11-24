#include "catalogo_sensores.hpp"

#include <algorithm>  // sort
#include <cmath>      // isfinite
#include <fstream>    // ifstream, ofstream
#include <locale>     // locale::classic (ponto decimal)
#include <sstream>    // istringstream: converte um campo de texto
#include <unordered_set> // evita duas identidades iguais no catalogo
#include <utility>    // move

namespace {
// FORNECIDO: assim como no LAB09, converta o campo INTEIRO.
// Aceitar "90xyz" como 90 seria aceitar silenciosamente uma linha errada.
bool numero_finito_completo(const std::string& texto, double& numero) {
    std::istringstream entrada{texto};
    entrada.imbue(std::locale::classic());
    entrada >> numero;
    if (!entrada) return false;
    entrada >> std::ws;
    return entrada.eof() && std::isfinite(numero);
}

// FORNECIDO: a primeira fronteira so verifica o texto, NAO a regra do sensor.
// "pressure,P-02,-1" passa aqui: -1 e um numero valido. A fabrica recusara
// a construcao porque -1 nao pode ser maximo acima do minimo 70 kPa.
bool interpretar_configuracao(const std::string& linha,
                            EspecificacaoSensor& spec, std::string& erro) {
    const auto primeira = linha.find(',');
    if (primeira == std::string::npos) {
        erro = "estrutura: esperados 3 campos";
        return false;
    }
    const auto segunda = linha.find(',', primeira + 1);
    if (segunda == std::string::npos ||
        linha.find(',', segunda + 1) != std::string::npos) {
        erro = "estrutura: esperados 3 campos";
        return false;
    }
    if (linha.find('"') != std::string::npos) {
        erro = "aspas fora do formato didatico";
        return false;
    }

    const std::string tipo = linha.substr(0, primeira);
    const std::string id = linha.substr(primeira + 1,
                                       segunda - primeira - 1);
    const std::string limite_texto = linha.substr(segunda + 1);
    if (tipo.empty() || id.empty()) {
        erro = "tipo ou id vazio";
        return false;
    }
    if (tipo.find_first_of(" \t\r") != std::string::npos ||
        id.find_first_of(" \t\r") != std::string::npos) {
        erro = "tipo ou id contem espacos";
        return false;
    }
    double limite{};
    if (!numero_finito_completo(limite_texto, limite)) {
        erro = "limite nao e numero finito completo";
        return false;
    }
    spec = {tipo, id, limite};
    return true;
}
} // namespace

ResultadoConfiguracao carregar_configuracao(const std::string& caminho) {
    std::ifstream entrada{caminho};
    ResultadoConfiguracao resultado;
    if (!entrada) {
        resultado.erros.push_back("nao abriu: " + caminho);
        return resultado;
    }
    resultado.arquivo_aberto = true;

    // FORNECIDO: nao perca a linha fisica na mensagem de erro.
    std::unordered_set<std::string> ids;
    std::string linha;
    std::size_t numero_linha = 0;
    while (std::getline(entrada, linha)) {
        ++numero_linha;
        if (linha.empty()) continue;
        EspecificacaoSensor spec;
        std::string erro;
        if (!interpretar_configuracao(linha, spec, erro)) {
            resultado.erros.push_back("linha " + std::to_string(numero_linha)
                                      + ": " + erro);
            continue;
        }
        if (!ids.insert(spec.id).second) {
            resultado.erros.push_back("linha " + std::to_string(numero_linha)
                                      + ": id duplicado: " + spec.id);
            continue;
        }
        resultado.especificacoes.push_back(std::move(spec));
    }
    if (entrada.bad()) {
        resultado.falha_leitura = true;
        resultado.erros.push_back("falha grave de leitura");
    }
    return resultado;
}

// LAB10 2 - FABRICA: escolhe a classe concreta e valida as regras de dominio.
std::unique_ptr<Sensor> criar_sensor(const EspecificacaoSensor& spec,
                                    std::string& erro) {
    erro.clear();
    // LAB10 2a - guarda: id vazio ou limite nao finito
    if (spec.id.empty()) {
        erro = "id vazio";
        return nullptr;
    }
    if (!std::isfinite(spec.limite)) {
        erro = "limite nao finito para " + spec.id;
        return nullptr;
    }

    if (spec.tipo == "temperature") {
        // LAB10 2b - zero absoluto ou abaixo nao e limiar fisico
        if (spec.limite <= -273.15) {
            erro = "limite de temperatura fisicamente impossivel";
            return nullptr;
        }
        return std::make_unique<SensorTemperaturaMotor>(spec.id, 92.0, spec.limite);
    }
    if (spec.tipo == "pressure") {
        // LAB10 2c - maximo precisa ser maior que o minimo (70 kPa)
        if (spec.limite <= 70.0) {
            erro = "maximo de pressao deve ser maior que 70 kPa";
            return nullptr;
        }
        return std::make_unique<SensorPressaoPneu>(spec.id, 82.0, 70.0, spec.limite);
    }
    // LAB10 2d - tipo desconhecido: nenhuma classe por padrao
    erro = "tipo de sensor nao suportado: " + spec.tipo;
    return nullptr;
}

// LAB10 3 - INSTALACAO: as guardas perifericas ja protegem a configuracao.
bool instalar_sensores(KartConfiguravel& configuracao,
                      const std::vector<EspecificacaoSensor>& specs,
                      std::vector<EntradaCatalogo>& catalogo,
                      std::string& erro) {
    erro.clear();
    // FORNECIDO: instalacao nova, nao mistura sensores de outra chamada.
    if (configuracao.quantidadeSensoresExtras() != 0 || !catalogo.empty()) {
        erro = "instalacao exige configuracao e catalogo inicialmente vazios";
        return false;
    }
    for (const auto& spec : specs) {
        // LAB10 3a - fabrica devolve o proprietario
        std::string erro_fabrica;
        std::unique_ptr<Sensor> dono = criar_sensor(spec, erro_fabrica);
        if (!dono) {
            erro = spec.id + ": " + erro_fabrica;
            return false;
        }
        // LAB10 3b - observador emprestado, so de dono valido
        const Sensor* observador = dono.get();

        // LAB10 3c - entrega por movimento e confere ANTES de publicar
        if (!configuracao.adicionarSensorExtra(std::move(dono))) {
            erro = "falha ao incluir " + spec.id;
            return false;
        }
        const std::string unidade = (spec.tipo == "temperature") ? "C" : "kPa";
        catalogo.push_back({observador, spec.tipo, unidade});
    }
    return true; // Zero especificacoes e uma instalacao vazia bem-sucedida.
}

// LAB10 4 - ESCRITA: fornecemos a protecao de acesso e o fechamento.
bool escrever_catalogo(const std::string& caminho,
                      const std::vector<EntradaCatalogo>& catalogo) {
    // FORNECIDO: nao crie/trunque uma saida que ja se sabe invalida.
    // Nao nulo nao garante vida: configuracao precisa continuar existindo.
    for (const auto& item : catalogo)
        if (item.observador == nullptr) return false;

    // LAB10 4a - abre a saida e confere a abertura
    std::ofstream saida{caminho};
    if (!saida) return false;

    // LAB10 4b - visao de enderecos; os donos e o vector original nao mudam
    std::vector<const EntradaCatalogo*> ordenados;
    ordenados.reserve(catalogo.size());
    for (const auto& item : catalogo) ordenados.push_back(&item);
    std::sort(ordenados.begin(), ordenados.end(),
              [](const EntradaCatalogo* esquerdo, const EntradaCatalogo* direito) {
                  return esquerdo->observador->identificador() <
                         direito->observador->identificador();
              });

    // LAB10 4c - cabecalho literal + uma linha por registro
    saida << "id,type,unit\n";
    for (const EntradaCatalogo* item : ordenados) {
        saida << item->observador->identificador() << ','
              << item->tipo << ',' << item->unidade << '\n';
    }

    // LAB10 4d - a falha pode surgir ao concluir a escrita: feche e confira
    saida.close();
    return static_cast<bool>(saida);
}