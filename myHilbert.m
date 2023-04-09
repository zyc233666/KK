function [y]=myHilbert(xn) 
    [xn,nshifts] = shiftdim(xn);
    n = length(xn); %计算x的长度
    x = fft(xn,n,1);   %对输入的数组进行FFT，将信号从时域转换到频域
    h = zeros(n,~isempty(x));%创建系统函数h
    %%%系统函数
    if n > 0 && 2*fix(n/2) == n
      % even and nonempty
      h([1 n/2+1]) = 1;
      h(2:n/2) = 2;
    elseif n>0
      % odd and nonempty
      h(1) = 1;
      h(2:(n+1)/2) = 2;
    end
    yy = ifft(x.*h(:,ones(1,size(x,2))),[],1);
    % Convert back to the original shape.
    y = shiftdim(yy,-nshifts);
end

