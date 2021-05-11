clear all; close all;
clc

% 블루투스 리스트 표시
fprintf("******  ALTINO Control Program *****\n");
fprintf("Connecting...\n");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("201603107014",1,"ByteOrder","little-endian","Timeout",1)


index = 0;
plot_front = [0, 0, 0];
front_X = [-600 0 600];

while 1
    index = index + 1;
    fprintf("Loop %d", index);

    speed = 0;
    
    % 알티노 속도 범위 -1000 ~ 1000
    % 시리얼 통신으로 음수는 전송하기 힘들어서
    % 0 ~ 2000으로 변환하여 전송
    speed = speed + 1000;
    
    % 1000을 하나의 바이트에 담기 불가능
    % High byte, Low byte로 데이터를 쪼개서 전송
    Speed_H = bitshift(speed, -7);
    Speed_L = speed - bitshift(Speed_H, 7);
    
    % speed 와 동일
    steer = 0;
    steer = steer + 127;
    Steer_H = bitshift(steer, -7);
    Steer_L = steer - bitshift(Steer_H, 7);
    
    % 입력받은 명령들을 패킷화
    command = [2 Speed_H Speed_L Steer_H Steer_L 65 65 65 3];
    
    % 블루투스 시리얼로 int8형식으로 전송
    write(device,command,"int8");
    
    % 통신 실패시 5번 정도 재통신 시도
        raw_read = read(device,1,"uint8");
        
        % 시작 바이트 인식
        if raw_read == 2
            recieve = read(device, 13, "uint8");
            
            % 종료 바이트 인식 -> 데이터 패킷 이상 유무 확인
            if recieve(13) == 3
                % 센서값 추출 부분
                front_L = bitshift(recieve(1), 8) + recieve(2) - 100;
                front_M = bitshift(recieve(3), 8) + recieve(4);
                front_R = bitshift(recieve(5), 8) + recieve(6);
                right = bitshift(recieve(7), 8) + recieve(8);
                left = bitshift(recieve(9), 8) + recieve(10);
                rear = bitshift(recieve(11), 8) + recieve(12);
                
                % 센서값 표시
                result = [front_L, front_M, front_R, right, left, rear];
                disp(result);
                
                % 그래프 그리기
                plot_front = [1000-front_L, 1000-front_M, 1000-front_R];
                plot(front_X,plot_front,'*-')
                axis manual;
                axis([-1100 1100 -1100 1100]);
                grid on;
                drawnow;
  
                
            else
                flush(device);
            end
            
        else
            flush(device);
        end
    end
    
