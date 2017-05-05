values=importdata('data.txt');
core0=values(:,1);
core1=values(:,2);
core2=values(:,3);
core3=values(:,4);

FigHandle=figure('Position', [100, 100, 1000, 500]);
plot([core0 core1 core2 core3], 'LineWidth', 2);
title('CPUs temperature');
xlabel('time(sec)');
ylabel('temperature(celsius)');
legend('CPU0', 'CPU1', 'CPU2', 'CPU3');
grid on;
pause
