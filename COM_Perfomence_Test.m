clear all; close all;
clc
% 블루투스 리스트 표시
devlist = bluetoothlist("Timeout",20)
device = bluetooth("0018E43524DC")

time = zeros(1,100);
disp("Test Start");
% 통신 테스트 100 회 실시
for jj = 1:100
    fprintf("Test %d" , jj);
    % 전송할 명령
    command = [2 0 0 1 27 65 65 65 65 3];
    
    % 블루투스 시리얼로 int8형식으로 전송
    tic
    write(device,command,"int8");
    
    % 알티노로 부터 응답 읽기
    for ii = 1:10
        
        reply = read(device,1,"string");
        switch reply
            case '0' 
                fprintf("reply = %s\n", reply);
                time(jj) = toc;
                break;
            
            case 'e'
                fprintf("reply = %s\n", reply);
                time(jj) = toc;
                break;
            
            case 's'
                fprintf("reply = %s\n", reply);
                time(jj) = toc;
                break;
            
        end
    end
    
    % 알티노는 1바이트 응답을 하므로 그 뒤는 모두 가비지 값
    % 삭제
    flush(device);
 
end

x = 1:100;
% ms 단위로 변환
time = time .* 1000;

% 평균 최대 최소 구하기
avg = sum(time) / 100;
max = max(time);
min = min(time);

fprintf("------------- 실험 결과 ---------------\n");
fprintf(" 평균 레이턴시 : %d\n" , avg);
fprintf(" 최대 레이턴시: %d\n" , max);
fprintf(" 최소 레이턴시: %d\n" , min);

plot(x,time, '-x')
grid on;
title(" 블루투스 통신 레이턴시 ");
ylabel(" 지연 속도 ms ");
xlabel(" 통신 횟수 " );
