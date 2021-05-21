clear all; close all;
clc
format short;

% 블루투스 리스트 표시
fprintf("******  ALTINO COM Test Program *****\n");
fprintf("Connecting...\n");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("201603107014",1,"ByteOrder","little-endian","Timeout",1)

% 실험 횟수
ExperimentINDEX = 200;
time = zeros(1,100);
log = zeros(1,ExperimentINDEX);
disp("Test Start");

success = 0;
fail = 0;
error_dataloss = 0;
error_etx = 0;
error_stx = 0;


% 실험 횟수
ExperimentINDEX = 200;

% 통신 테스트 100 회 실시
for jj = 1:ExperimentINDEX
    fprintf("Test %d, Success:%d Fail %d\n" , jj, success, fail);
    % 전송할 명령
    command = [2 7 104 0 127 65 65 0 3];
    
    % 블루투스 시리얼로 int8형식으로 전송
    tic
    status = 0;
    for COMcount = 1:10 
        
        % 블루투스 시리얼로 int8형식으로 전송
        write(device,command,"uint8");
       
        raw_read = read(device,1,"uint8");
        
        % 시작 바이트 인식
        if raw_read == 2
            recieve = read(device, 14, "uint8");
            if length(recieve) == 14
                % 종료 바이트 인식 -> 데이터 패킷 이상 유무 확인
                if recieve(14) == 3
                    % 센서값 추출 부분
                    status = recieve(1);
                    front_L = bitshift(recieve(2), 8) + recieve(3);
                    front_M = bitshift(recieve(4), 8) + recieve(5);
                    front_R = bitshift(recieve(6), 8) + recieve(7);
                    right = bitshift(recieve(8), 8) + recieve(9);
                    left = bitshift(recieve(10), 8) + recieve(11);
                    rear = bitshift(recieve(12), 8) + recieve(13);
                
                    % 센서값 표시
                    result = [status, front_L, front_M, front_R, right, left, rear];
                    log(jj) = status;
                    
                    if status == 4
                        success = success + 1;
                        break;
                    else
                        switch status
                            case 1
                                error_dataloss = error_dataloss + 1;
                                
                            case 2
                                error_stx = error_stx + 1;
                            case 3
                                error_etx = error_etx + 1;
                            otherwise
                                fail = fail + 1;
                        end
                    end
                
                    
                % if recieve(14 == 3
                else
                    flush(device);
                end
            % if length(14) == 3    
            else
                flush(device);
            end
            
        % if raw_read    
        else
            flush(device);
        end
    end
    time(jj) = toc;
end

x = 1:ExperimentINDEX;
% ms 단위로 변환
time = time .* 1000;

% 평균 최대 최소 구하기
avg = sum(time) / ExperimentINDEX;
max = max(time);
min = min(time);

fprintf("------------- 실험 결과 ---------------\n");
fprintf(" 평균 레이턴시 : %d\n" , avg);
fprintf(" 최대 레이턴시: %d\n" , max);
fprintf(" 최소 레이턴시: %d\n" , min);
fprintf("Success: %d \t Fail: %d\n", success, fail);
fprintf("Dataloss: %d \t MissingSTX: %d \t MissingETX:%d\n", error_dataloss, error_stx, error_etx);
fprintf("손실율: %d %\n", (error_dataloss + error_etx + error_stx)/ExperimentINDEX * 100);

plot(x,time, '-x')
grid on;
title(" 블루투스 통신 레이턴시 ");
ylabel(" 지연 속도 ms ");
xlabel(" 통신 횟수 " );
