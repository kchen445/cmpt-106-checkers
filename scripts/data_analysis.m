M = dlmread('../bin/evolution_data.txt');

Time = M(1:end,1);
Generation = M(1:end,2);
Total_Turns = M(1:end,3);
Ties = M(1:end,4);
P1_Wins = M(1:end,5);
P2_Wins = M(1:end,6);
P1_LostPieces = M(1:end,7);
P2_LostPieces = M(1:end,8);

Elo = M(1:end,9:end);
Elo_Average = mean(Elo, 2);

k = 100;

Ties = movmean(Ties, k);
P1_Wins = movmean(P1_Wins, k);
P2_Wins = movmean(P2_Wins, k);
Total_Turns = movmean(Total_Turns, k);
Normalized_Total_Turns = Total_Turns / 400;

figure
subplot(2,1,1)
plot(Generation, Normalized_Total_Turns)
hold on
plot(Generation, 75 * ones(size(Generation)), '--')
hold off
legend('Turns', 'Max')
xlabel('Generations')
ylabel('Number of Turns per Game')
ylim([0, 85])
title('Turns / Generation')

subplot(2,1,2)
plot(Generation, Ties)
hold on
plot(Generation, P1_Wins)
hold on
plot(Generation, P2_Wins)
hold off
legend('Ties', 'P1 Wins', 'P2 Wins')
xlabel('Generations')
ylabel('Win/Loss/Tie Count')
title('Outcome / Generation')
