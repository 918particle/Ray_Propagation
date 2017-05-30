upper = load('MalcomMellorFig13AUpper.dat');
lower = load('MalcomMellorFig13ALower.dat');
middle = load('MalcomMellorFig13AMiddle.dat');

upperCompressibility = zeros(size(upper));
lowerCompressibility = zeros(size(lower));
middleCompressibility = zeros(size(middle));

for i=2:length(upperCompressibility)
	upperCompressibility(i,1) = upper(i,1);
	upperCompressibility(i,2) = (upper(i,1)-upper(i-1,1))/(upper(i,2)-upper(i-1,2))/(upper(i,1));
end
upperCompressibility(1,1) = upper(1,1);
upperCompressibility(1,2) = upperCompressibility(2,2);

for i=2:length(lowerCompressibility)
	lowerCompressibility(i,1) = lower(i,1);
	lowerCompressibility(i,2) = (lower(i,1)-lower(i-1,1))/(lower(i,2)-lower(i-1,2))/(lower(i,1));
end
lowerCompressibility(1,1) = lower(1,1);
lowerCompressibility(1,2) = lowerCompressibility(2,2);

for i=2:length(middleCompressibility)
	middleCompressibility(i,1) = middle(i,1);
	middleCompressibility(i,2) = (middle(i,1)-middle(i-1,1))/(middle(i,2)-middle(i-1,2))/(middle(i,1));
end
middleCompressibility(1,1) = middle(1,1);
middleCompressibility(1,2) = middleCompressibility(2,2);

figure(1);
hold on;
semilogy(upperCompressibility(:,1),upperCompressibility(:,2),'o');
semilogy(lowerCompressibility(:,1),lowerCompressibility(:,2),'o');
semilogy(middleCompressibility(:,1),middleCompressibility(:,2),'o');

print -dpdf May27_plot1.pdf

