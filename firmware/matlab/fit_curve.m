% Fit exponential curve y=a*exp(b*x)+c
ave_res = [778, 274.5, 207, 157.75, 155, 58];
mass = [0, 100, 200, 300, 500, 616];

x = linspace(0, 700);
f = -(1152921504606846976*log(x./937))./7422508646658881;

figure(1)
plot(mass, ave_res, 'o')
hold on
plot(x, f);
hold off


