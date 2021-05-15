% bluetooth_Send.m
% 작 성 자: 기계융합공학과 이승신(2018013246)
% 작 성 일: 2021-04-28
% 설    명
% 사용자로 부터 속도, 조향값을 입력받아 프로토콜에 따라 데이터 전송
% 또한 미


clear all; close all;
clc
% 블루투스 리스트 표시
fprintf("******  ALTINO Control Program *****\n");
fprintf("Connecting...\n");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("201603107014")

% 그래프 초기설정
%figure(1)
%Carshape = polyshape([-50 -50 50 50],[ 90 -90 -90 90]);

% 그래프 상수
plot_front = [0, 0, 0];
front_X = [-600 0 600];

plot_side = [-500, 0, 500];
right_X = [0, 0, 0];

while 1
    
    % 속도명령 입력받음
    speed = input("speed: ");
    
    % 알티노 속도 범위 -1000 ~ 1000
    % 시리얼 통신으로 음수는 전송하기 힘들어서
    % 0 ~ 2000으로 변환하여 전송
    speed = speed + 1000;
    
    % 1000을 하나의 바이트에 담기 불가능
    % High byte, Low byte로 데이터를 쪼개서 전송
    Speed_H = bitshift(speed, -7);
    Speed_L = speed - bitshift(Speed_H, 7);
    
    % speed 와 동일
    steer = input("steer: ");
    steer = steer + 127;
    Steer_H = bitshift(steer, -7);
    Steer_L = steer - bitshift(Steer_H, 7);
    
    % 입력받은 명령들을 패킷화
    command = [2 Speed_H Speed_L Steer_H Steer_L 65 65 65 3];
    
    status = 0;
    for COMcount = 1:10 
        
        % 블루투스 시리얼로 int8형식으로 전송
        write(device,command,"uint8");
        
        
        raw_read = read(device,1,"uint8");
        
        % 시작 바이트 인식
        if raw_read == 2
            recieve = read(device, 14, "uint8");
            
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
                disp(result)
         
                    
                % 그래프 생성
                plot_front = [1000-front_L, 1000-front_M, 1000-front_R];
                right_X = [1000-right, 1000-right, 1000-right];
                left_X = [-1000+left, -1000+left, -1000+left];
                plot_rear = [-1000+rear, -1000+rear, -1000+rear,];
                
                plot(front_X,plot_front,'*-', right_X,plot_side,'*-', left_X,plot_side,'*-', front_X, plot_rear,'*-' );
                axis manual;
                axis([-1100 1100 -1100 1100]);
                grid on;
                drawnow;
                
                if status == 4
                    break;
                end
                
            else
                flush(device);
            end
            
        else
            flush(device);
        end
    end
end
    
    
 

