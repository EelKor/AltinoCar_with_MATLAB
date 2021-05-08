% bluetooth_Send.m
% 작 성 자: 기계융합공학과 이승신(2018013246)
% 작 성 일: 2021-04-28
% 설    명
% 사용자로 부터 속도, 조향값을 입력받아 프로토콜에 따라 데이터 전송


clear all; close all;
clc
% 블루투스 리스트 표시
fprintf("Connecting...\n");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("0018E43524DC")

% 그래프 초기설정
figure(1)
Carshape = polyshape([-50 -50 50 50],[ 90 -90 -90 90]);


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
    command = [2 Speed_H Speed_L Steer_H Steer_L 65 65 65 65 3];
    
    % 블루투스 시리얼로 int8형식으로 전송
    write(device,command,"int8");
    
    % 알티노로 부터 응답 읽기
    for ii = 1:10
        
        reply = read(device,1,"string");
        switch reply
            case '0' 
                fprintf("reply = %s\n", reply);
                
                % 센서값 추출 부분
                sensor = read(device,10,"uint8");
                front = bitshift(sensor(2), 8) + sensor(3);
                
                % 그래프 그리기
                plot(front,'*')
                drawnow;
                axis square
                axis([-500 500 -500 500]);
                
                break;
            
            case 'e'
                fprintf("reply = %s\n", reply);
                break;
            
            case 's'
                fprintf("reply = %s\n", reply);
                break;
            
        end
    end
    
    
    % 알티노는 1바이트 응답을 하므로 그 뒤는 모두 가비지 값
    % 삭제
    flush(device);
 
end

