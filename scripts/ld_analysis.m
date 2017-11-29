data = dlmread('../bin/learning_data.txt');
k = 10;

gen = data(1:end, 1);
time = data(1:end, 2);
pts_m = data(1:end, 3:end);

pts_mean = mean(pts_m, 2);
pts_max = max(pts_m, [], 2);
pts_min = min(pts_m, [], 2);

pts_mean = movmean(pts_mean, k);
pts_max = movmean(pts_max, k);
pts_min = movmean(pts_min, k);

figure
line(gen, pts_mean, 'Color', 'b')
line(gen, pts_max, 'Color', 'g')
line(gen, pts_min, 'Color', 'r')
legend('Mean', 'Max', 'Min')

ax1 = gca;
ax1_pos = ax1.Position;

ax1.YLim = [0, 30];
xlabel('Generation')
ylabel('Points')

ax2 = axes('Position', ax1_pos,...
    'XAxisLocation','top',...
    'YAxisLocation','right',...
    'Color', 'none');
ax2.YColor = 'none';
ax2.YLim = [0, 30];
xlabel('Time (h)')
time_h = time / 3600;
line(time_h, 10 * ones(size(gen)), 'Parent', ax2, 'LineStyle', '--', 'Color', 'k')
legend('Complete Tie')

