clear; clc; close all

% Carregar Dados 
dados = readtable('DataBase_RNA.xlsx');
tempo = dados.TempoDecorrido_s_;
corrente = dados.Corrente_A_;
pwm = dados.PotenciaReal___;
empuxo = dados.Peso_g_;

% DIVISÃO TREINO / TESTE 
cv = cvpartition(height(dados),'HoldOut',0.2);
dados_treino = dados(training(cv),:);
dados_teste  = dados(test(cv),:);

% TREINAMENTO DA REGRESSÃO LINEAR
modelo = fitlm(dados_treino,'linear');
disp('Modelo de regressão linear:')
disp(modelo)

% PREDIÇÃO
empuxo_pred = predict(modelo,dados_teste);
empuxo_real = dados_teste.Peso_g_;

% MÉTRICAS DE ERRO
MAE = mean(abs(empuxo_real - empuxo_pred));
MSE = mean((empuxo_real - empuxo_pred).^2);
RMSE = sqrt(MSE);

%indices_val_mape = reais ~= 0;
MAPE = mean(abs((empuxo_real - empuxo_pred) ./ empuxo_real)) * 100;


%epsilon = 1e-6;
%MAPE = mean(abs((empuxo_real - empuxo_pred) ./ (empuxo_real + epsilon))) * 100;

%MAPE = mean(abs((empuxo_real - empuxo_pred) ./ empuxo_real)) * 100;
SS_res = sum((empuxo_real - empuxo_pred).^2);
SS_tot = sum((empuxo_real - mean(empuxo_real)).^2);
R2 = 1 - SS_res/SS_tot;


% --- EXIBIÇÃO DOS RESULTADOS NO CONSOLE ---
fprintf('\n========================================\n');
fprintf('   RELATÓRIO DE DESEMPENHO DA REDE\n');
fprintf('========================================\n');
fprintf('MAE (Erro Médio Absoluto):      %.4f g\n', MAE);
fprintf('RMSE (Raiz do Erro Quadrático): %.4f g\n', RMSE);
fprintf('MAPE (Erro Médio Percentual):   %.2f %%\n', MAPE);
fprintf('R² (Coeficiente de Ajuste):     %.4f\n', R2);
fprintf('========================================\n');

% GRÁFICO 1 - REAL VS ESTIMADO
figure('Name', 'Real (O) vs. Estimado (X) - Regressão Linear');
plot(empuxo_real,'bo','LineWidth',1)
hold on
plot(empuxo_pred,'gx','LineWidth',1)
legend('Empuxo real','Empuxo estimado')
xlabel('Amostra')
ylabel('Empuxo')
title('Validação (Regressão Linear)')
grid on


% GRÁFICO 2 - DISPERSÃO (PADRÃO DE ARTIGO)
figure('Name', 'Dispersão - Regressão Linear');
scatter(empuxo_real,empuxo_pred,'g', 'filled', 'MarkerFaceAlpha', 0.5)
hold on
min_val = min(empuxo_real);
max_val = max(empuxo_real);
plot([min_val max_val],[min_val max_val],'--b','LineWidth',2)
xlabel('Empuxo real')
ylabel('Empuxo estimado')
title('Dispersão dos Resultados (Regressão Linear)')
grid on

% COEFICIENTES DA REGRESSÃO
coef = modelo.Coefficients;

disp('Coeficientes da regressão:')
disp(coef)

beta0 = coef.Estimate(1);
beta1 = coef.Estimate(2);
beta2 = coef.Estimate(3);
beta3 = coef.Estimate(4);

fprintf('\nEquação estimada:\n')
fprintf('Empuxo = %.4f + %.4f*tempo + %.4f*corrente + %.4f*pwm\n', ...
    beta0,beta1,beta2,beta3);