function WM_plot(r,c,ext_wm,orig_wm, value)
store = [];
for k=1:1000
    wm=randn(r,c);%depending on the size of the watermark
    wm=double(wm);%if necessary
    store(k)=WM_detect(ext_wm,wm);%wrong watermarks
    if k == value
        store(k)=WM_detect(ext_wm,orig_wm);%original watermark detection
    end
end
figure(1),plot(1:k,[store]),ylabel('Watermark detector response'),xlabel('random watermarks');
hold on
%threshold calculation
[peak,ind]=sort(store,'descend');
threshold=peak(2)+(peak(2)*0.1);%T=second highest peak+10percentof the same
figure(1),plot(1:1000,[threshold],'red');
hold on
figure(1),plot(1:1000,peak(2),'green');