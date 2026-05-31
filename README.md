
# Aplicação de Redes Neurais Artificiais na Estimação do Empuxo Gerado por um Conjunto Propulsor de Drone

Este repositório contém a base de dados experimentais, os scripts de treinamento e os modelos de Redes Neurais Artificiais (RNA) desenvolvidos para a estimação de empuxo em conjuntos motopropulsores de Veículos Aéreos Não Tripulados (VANTs). 

Este trabalho foi submetido e aceito preliminarmente para publicação na **Revista Principia - ISSN: 1517-0306**.

## Visão Geral do Projeto

O objetivo principal desta pesquisa é o desenvolvimento de um **sensor virtual** baseado em redes neurais artificiais. Utilizando apenas grandezas de fácil monitoramento, como corrente elétrica e sinal PWM, associadas ao tempo decorrido. O modelo é capaz de inferir dinamicamente o empuxo gerado pelo motor, eliminando a necessidade de células de carga físicas na aeronave, reduzindo peso estrutural e custos. Além de possibilitar análises futuras de relação de empuxo vs potência em motopropulsores de drones.

A inclusão da variável temporal atua como um estimador indireto para contornar o efeito de histerese térmica sofrido pelo conjunto propulsor durante a operação em bancada estática.

## 📂 Estrutura do Repositório

*   `/data`: Matriz de dados experimentais contendo as coletas realizadas na bancada estática.
*   `/src`: Scripts utilizados para o tratamento dos dados, arquitetura e treinamento da rede neural.
*   `/models`: Arquivos dos modelos/pesos já treinados e prontos para execução.
*   `/figures`: Gráficos de desempenho do modelo (regressão, análise de erro e épocas de treinamento).

## 📊 Variáveis do Modelo

*   **Entradas (Inputs):**
    1. Corrente Elétrica ($I$)
    2. Sinal de Controle ($PWM$)
    3. Tempo Decorrido ($t$)
*   **Saída (Output):**
    1. Empuxo Estimado ($T_{est}$)

 ## 📈 Métricas de Avaliação de Modelos
 * MAE (Erro Médio Absoluto).
 * RMSE (Raiz do Erro Quadrático Médio).
 * MAPE (Erro Médio Percentual Absoluto) com tratamento para divisão por zero.
 * $R^2$ (Coeficiente de Determinação / Ajuste).

## 💻 Requisitos e Tecnologias

Os scripts foram desenvolvidos utilizando:
*   [MATLAB]
*   Principais bibliotecas: [Deep Learning Toolbox / Statistics and Machine Learning Toolbox]
