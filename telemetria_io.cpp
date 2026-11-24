#include "telemetria_io.hpp"
#include <cmath>      // isfinite: distingue numero de NaN/Inf.
#include <cstddef>    // size_t: conta linhas fisicas.
#include <fstream>    // ifstream, ofstream.
#include <iomanip>    // setprecision no boletim fornecido.
#include <locale>     // ponto decimal na conversao.
#include <sstream>    // istringstream sobre UMA string.
#include <string>
#include <iostream>

// O programa de estudo da Aula 11 foi dividido sem alterar o contrato:
// aluno: numero_finito, carregar_arquivo e aplicar_no_kart;
// fornecido: interpretar_linha e escrever_boletim.

// LAB09 2a-2c - Seu codigo confirmara um numero COMPLETO e finito.
bool numero_finito(const std::string& texto, double& valor) {
    // 2a: fluxo sobre o texto, com ponto decimal (locale classico).
    std::istringstream entrada{texto};
    entrada.imbue(std::locale::classic());

    // 2b: tenta extrair um double; se falhar (ex.: "abc"), recusa.
    entrada >> valor;
    if (!entrada) return false;

    // 2c: consome espacos finais e exige que o campo tenha acabado
    // (descarta "108abc") e que o numero seja finito (descarta NaN/Inf).
    entrada >> std::ws;
    return entrada.eof() && std::isfinite(valor);
}

// FORNECIDO - interpreta UMA linha; nao modifica o Kart.
ResultadoInterpretacao interpretar_linha(const std::string& linha) {
    // 1. Verifica se existe exatamente uma virgula na linha
    const auto virgula = linha.find(',');
    if (virgula == std::string::npos ||
        linha.find(',', virgula + 1) != std::string::npos) {
        return {false, {}, "estrutura: esperados 2 campos"};
    }

    // 2. Rejeita o uso de aspas
    if (linha.find('"') != std::string::npos) {
        return {false, {}, "aspas fora do formato didatico"};
    }

    // 3. Separa os dois campos: canal (antes da virgula) e valor (depois da virgula)
    const std::string canal = linha.substr(0, virgula);
    const std::string texto_valor = linha.substr(virgula + 1);

    // 4. Valida se o canal nao esta em branco
    if (canal.empty())
        return {false, {}, "canal vazio"};

    // 5. Valida se o canal pertence a lista de canais suportados pelo Kart
    if (canal != "PRESSAO_PNEU" &&
        canal != "TEMP_MOTOR" &&
        canal != "LUZ_AMBIENTE") {
        return {false, {}, "canal desconhecido: " + canal};
    }

    // 6. Converte e valida o campo numérico
    double valor{};
    if (!numero_finito(texto_valor, valor)) {
        return {false, {}, "valor nao e um numero finito completo"};
    }

    // Se passou por todas as checagens, retorna sucesso com a leitura estruturada
    return {true, {canal, valor}, {}};
}

// LAB09 3a-3d - Coordene o arquivo; NAO atualize os sensores aqui.
ResultadoCarga carregar_arquivo(const std::string& caminho) {
    // 3a: tenta abrir; se nao abriu, devolve carga "fechada" com a causa.
    std::ifstream entrada{caminho};
    if (!entrada) {
        return {false, false, {}, {"nao abriu: " + caminho}};
    }

    // 3b: a abertura funcionou.
    ResultadoCarga resultado;
    resultado.arquivo_aberto = true;
    std::string linha;
    std::size_t numero = 0;

    // 3c: getline na condicao (nunca !eof()); conta ANTES de ignorar vazias.
    while (std::getline(entrada, linha)) {
        ++numero;
        if (linha.empty()) continue;

        // 3d: uma linha, uma decisao, uma colecao.
        const auto item = interpretar_linha(linha);
        if (item.ok) {
            resultado.aceitas.push_back(item.leitura);
        } else {
            resultado.erros.push_back(
                "linha " + std::to_string(numero) + ": " + item.erro);
        }
    }

    // Sair do laco nao prova fim normal: bad() indica falha grave.
    if (entrada.bad()) {
        resultado.falha_leitura = true;
        resultado.erros.push_back("falha de leitura");
    }
    return resultado;
}

// LAB09 4a-4c - Aplique cada leitura JA VALIDADA ao mesmo Kart.
void aplicar_no_kart(Kart& kart, const LeituraValidada& leitura) {
    if (leitura.canal == "PRESSAO_PNEU") {                 // 4a
        kart.atualizarPressao(leitura.valor);
    } else if (leitura.canal == "TEMP_MOTOR") {            // 4b
        kart.atualizarTemperatura(leitura.valor);
    } else if (leitura.canal == "LUZ_AMBIENTE") {          // 4c
        kart.atualizarLuminosidade(leitura.valor);
        kart.atualizarFarois();
    }
    // Os limites de alerta pertencem aos sensores do Kart, nao a esta funcao.
}

// FORNECIDO - o Kart ja foi atualizado; consulte sem mudar seus sensores.
bool escrever_boletim(const std::string& caminho,
                      const Kart& kart, const ResultadoCarga& carga) {
    std::ofstream saida{caminho};
    if (!saida) return false; // Nao foi possivel criar/abrir o arquivo para escrita

    // Usa ponto decimal; setprecision(12) ajusta digitos significativos.
    saida.imbue(std::locale::classic());
    saida << std::setprecision(12);

    // Emite o relatorio formatado
    saida << "BOLETIM DO COMETA\n";
    saida << "Pressao: " << kart.pressaoPneu() << " kPa - "
          << (kart.pressaoSegura() ? "OK" : "ATENCAO") << '\n';
    saida << "Temperatura: " << kart.temperaturaMotor() << " C - "
          << (kart.temperaturaSegura() ? "OK" : "ATENCAO") << '\n';
    saida << "Luminosidade: " << kart.luminosidade() << " lux - "
          << (kart.faroisLigados() ? "ATENCAO" : "OK") << '\n';
    saida << "Farois: " << (kart.faroisLigados() ? "LIGADOS" : "DESLIGADOS")
          << '\n';
    saida << "Aceitas: " << carga.aceitas.size() << '\n';
    saida << "Recusadas: " << carga.erros.size() << '\n';

    // Lista os erros encontrados durante o processamento do arquivo de entrada
    for (const auto& erro : carga.erros) {
        saida << "Erro: " << erro << '\n';
    }

    // O fechamento tenta concluir o envio dos dados e informa erros do stream.
    // Sucesso do stream nao prova persistencia fisica (fsync nao foi solicitado).
    saida.close();
    return static_cast<bool>(saida);
}