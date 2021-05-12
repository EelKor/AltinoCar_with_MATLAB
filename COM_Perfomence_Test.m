clear all; close all;
clc
% 블루투스 리스트 표시
fprintf("******  ALTINO COM Test Program *****\n");
fprintf("Connecting...\n");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("201603107014",1,"ByteOrder","little-endian","Timeout",1)

time = zeros(1,100);
disp("Test Start");

success = 0;
fail = 0;

% 통신 테스트 100 회 실시
for jj = 1:100
    fprintf("Test %d, Success:%d Fail %d\n" , jj, success, fail);
    % 전송할 명령
    command = [2 0 0 0 127 65 65 65 3];
    
    % 블루투스 시리얼로 int8형식으로 전송
    tic
    write(device,command,"uint8");
    
    % 알티노로 부터 응답 읽기
    for ii = 1:10
        
        reply = read(device,1,"uint8");
        if reply == 2 
           recieve = read(device,13,"uint8");
           
           if recieve(13) == 3
               % 센서값 추출 부분
                front_L = bitshift(recieve(1), 8) + recieve(2);
                front_M = bitshift(recieve(3), 8) + recieve(4);
                front_R = bitshift(recieve(5), 8) + recieve(6);
                right = bitshift(recieve(7), 8) + recieve(8);
                left = bitshift(recieve(9), 8) + recieve(10);
                rear = bitshift(recieve(11), 8) + recieve(12);
                
                % 센서값 표시
                result = [front_L, front_M, front_R, right, left, rear];
                
                success = success + 1;
                break;
           end
            fail = fail + 1;
            flush(device);
        end
        fail = fail + 1;
        flush(device);
    end
    
    time(jj) = toc;
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
fprintf("Success: %d \t Fail: %d", success, fail);


plot(x,time, '-x')
grid on;
title(" 블루투스 통신 레이턴시 ");
ylabel(" 지연 속도 ms ");
xlabel(" 통신 횟수 " );
